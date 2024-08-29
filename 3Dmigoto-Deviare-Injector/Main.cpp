#include "NktHookLib.h"
#include <iostream>
#include "ini_parser_lite.hpp"

#include "MMTStringUtils.h"


static void wait_keypress(const char* msg)
{
	puts(msg);
	getchar();
}


static void wait_exit(int code = 0, char* msg = _strdup("\nPress enter to close...\n"))
{
	wait_keypress(msg);
	exit(code);
}


static void exit_usage(const char* msg)
{
	//                                                          80 column limit --------> \n
	printf("The Loader is not configured correctly. Please copy the 3DMigoto d3d11.dll\n"
		"and d3dx.ini into this directory, then edit the d3dx.ini's [Loader] section\n"
		"to set the target executable and 3DMigoto module name.\n"
		"\n"
		"%s", msg);

	wait_exit(EXIT_FAILURE);
}


LPWSTR ConvertToLPWSTR(const char* text)
{
	int size = MultiByteToWideChar(CP_ACP, 0, text, -1, nullptr, 0);
	LPWSTR buffer = new WCHAR[size];
	MultiByteToWideChar(CP_ACP, 0, text, -1, buffer, size);
	return buffer;
}

int __CRTDECL wmain(__in int argc, __in wchar_t* argv[]) {

	char* buf, target[MAX_PATH], setting[MAX_PATH], module_path[MAX_PATH];
	wchar_t setting_w[MAX_PATH], working_dir[MAX_PATH], * working_dir_p = NULL;
	DWORD filesize, readsize;
	const char* ini_section;
	wchar_t module_full_path[MAX_PATH];
	int rc = EXIT_FAILURE;
	HANDLE ini_file;
	HMODULE module;
	int hook_proc;
	FARPROC fn;
	HHOOK hook;
	bool launch;

	CreateMutexA(0, FALSE, "Local\\3DMigotoLoader");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		wait_exit(EXIT_FAILURE, _strdup("ERROR: Another instance of the 3DMigoto Loader is already running. Please close it and try again\n"));

	printf("\n------------------------------- 3DMigoto Loader (Stable ByPass ACE version) ------------------------------\n\n");

	ini_file = CreateFile(L"d3dx.ini", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (ini_file == INVALID_HANDLE_VALUE)
		exit_usage("Unable to open d3dx.ini\n");

	filesize = GetFileSize(ini_file, NULL);
	buf = new char[filesize + 1];
	if (!buf)
		wait_exit(EXIT_FAILURE, _strdup("Unable to allocate d3dx.ini buffer\n"));

	if (!ReadFile(ini_file, buf, filesize, &readsize, 0) || filesize != readsize)
		wait_exit(EXIT_FAILURE, _strdup("Error reading d3dx.ini\n"));

	CloseHandle(ini_file);

	ini_section = find_ini_section_lite(buf, "loader");
	if (!ini_section)
		exit_usage("d3dx.ini missing required [Loader] section\n");

	// Check that the target is configured. We don't do anything with this
	// setting from here other than to make sure it is set, because the
	// injection method we are using cannot single out a specific process.
	// Once 3DMigoto has been injected it into a process it will check this
	// value and bail if it is in the wrong one.
	if (!find_ini_setting_lite(ini_section, "target", target, MAX_PATH))
		exit_usage("d3dx.ini [Loader] section missing required \"target\" setting\n");


	if (!find_ini_setting_lite(ini_section, "module", module_path, MAX_PATH))
		exit_usage("d3dx.ini [Loader] section missing required \"module\" setting\n");

	std::wstring module_path_abs = MMTString::GetCurrentWorkingDirectoryPath() + L"\\" + MMTString::ToWideString(std::string(module_path));

	DWORD dwOsErr;

	//目标程序路径
	LPWSTR szExeNameW = ConvertToLPWSTR(target);

	//目标Dll路径
	LPWSTR szDllToInjectNameW = (LPWSTR)module_path_abs.c_str();
	LPSTR szInitFunctionA = (LPSTR)"DllMain";

	STARTUPINFOW sSiW;
	PROCESS_INFORMATION sPi;

	memset(&sSiW, 0, sizeof(sSiW));
	sSiW.cb = (DWORD)sizeof(sSiW);
	memset(&sPi, 0, sizeof(sPi));
	dwOsErr = NktHookLibHelpers::CreateProcessWithDllW(szExeNameW, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sSiW, &sPi,
		szDllToInjectNameW, NULL, NULL);
	if (dwOsErr == ERROR_SUCCESS)
	{
		wprintf_s(L"Process #%lu successfully launched with dll injected!\n", sPi.dwProcessId);
		::CloseHandle(sPi.hThread);
		::CloseHandle(sPi.hProcess);
	}
	else if (dwOsErr == ERROR_PATH_NOT_FOUND) {
		wprintf_s(L"Error %lu: Cannot find target program path.\n", dwOsErr);
	}
	else
	{
		wprintf_s(L"Error %lu: Cannot launch process and inject dll.\n", dwOsErr);
	}

	Sleep(3000);
	delete[] buf;
	return rc;
}

