#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <set>
#include <filesystem>

//3Dmigoto原版解析
#include "ini_parser_lite.hpp"

//退出秒数
uint8_t ExitAfterSeconds = 33;


static void wait_exit(int code = 0, char* msg = _strdup("\nPress enter to close...\n"))
{
	//等待按下按键
	puts(msg);
	getchar();
	//退出
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


static bool verify_injection(PROCESSENTRY32* pe, const wchar_t* module, bool log_name)
{
	HANDLE snapshot;
	MODULEENTRY32 me;
	const wchar_t* basename = wcsrchr(module, '\\');
	bool rc = false;
	static std::set<DWORD> pids;
	wchar_t exe_path[MAX_PATH], mod_path[MAX_PATH];

	if (basename)
		basename++;
	else
		basename = module;

	do {
		snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe->th32ProcessID);
	} while (snapshot == INVALID_HANDLE_VALUE && GetLastError() == ERROR_BAD_LENGTH);
	if (snapshot == INVALID_HANDLE_VALUE) {
		printf("%S (%d): 现在可以进入游戏按小键盘0开关Hunting绿字界面来测试是否注入成功了(如已成功，请忽略此提示): %d\n",
			pe->szExeFile, pe->th32ProcessID, GetLastError());
		return false;
	}

	me.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(snapshot, &me)) {
		printf("%S (%d): 现在可以进入游戏按小键盘0开关Hunting绿字界面来测试是否注入成功了(如已成功，请忽略此提示): %d\n",
			pe->szExeFile, pe->th32ProcessID, GetLastError());
		goto out_close;
	}

	// First module is the executable, and this is how we get the full path:
	if (log_name)
		printf("目标进程已找到 (%i): %S\n", pe->th32ProcessID, me.szExePath);
	wcscpy_s(exe_path, MAX_PATH, me.szExePath);

	rc = false;
	while (Module32Next(snapshot, &me)) {
		if (_wcsicmp(me.szModule, basename))
			continue;

		if (!_wcsicmp(me.szExePath, module)) {
			if (!pids.count(pe->th32ProcessID)) {
				printf("%d: 3DMigoto 加载成功\n", pe->th32ProcessID);
				pids.insert(pe->th32ProcessID);
			}
			rc = true;
		}
		else {
			wcscpy_s(mod_path, MAX_PATH, me.szExePath);
			wcsrchr(exe_path, L'\\')[1] = '\0';
			wcsrchr(mod_path, L'\\')[1] = '\0';
			if (!_wcsicmp(exe_path, mod_path)) {
				printf("\n\n\n"
					"错误提示：游戏目录中也存在一个3Dmigoto的d3d11.dll，你同时只能使用一种注入方式，请删除游戏目录中的所有3Dmigoto文件:\n"
					"%S\n"
					"This may crash - please remove the copy in the game directory and try again\n\n\n",
					me.szExePath);
				wait_exit(EXIT_FAILURE);
			}
		}
	}

out_close:
	CloseHandle(snapshot);
	return rc;
}

static bool check_for_running_target(wchar_t* target, const wchar_t* module)
{
	// https://docs.microsoft.com/en-us/windows/desktop/ToolHelp/taking-a-snapshot-and-viewing-processes
	HANDLE snapshot;
	PROCESSENTRY32 pe;
	bool rc = false;
	wchar_t* basename = wcsrchr(target, '\\');
	static std::set<DWORD> pids;

	if (basename)
		basename++;
	else
		basename = target;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE) {
		printf("现在可以进入游戏按小键盘0开关Hunting绿字界面来测试是否注入成功了(如已成功，请忽略此提示): %d\n", GetLastError());
		return false;
	}

	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &pe)) {
		printf("现在可以进入游戏按小键盘0开关Hunting绿字界面来测试是否注入成功了(如已成功，请忽略此提示): %d\n", GetLastError());
		goto out_close;
	}

	do {
		if (_wcsicmp(pe.szExeFile, basename))
			continue;

		rc = verify_injection(&pe, module, !pids.count(pe.th32ProcessID)) || rc;
		pids.insert(pe.th32ProcessID);
	} while (Process32Next(snapshot, &pe));

out_close:
	CloseHandle(snapshot);
	return rc;
}

static void wait_for_target(const char* target_a, const wchar_t* module_path, bool wait, int delay, bool launched)
{
	wchar_t target_w[MAX_PATH];

	if (!MultiByteToWideChar(CP_UTF8, 0, target_a, -1, target_w, MAX_PATH))
		return;

	int check_count = 0;
	for (int seconds = 0; wait || delay == -1; seconds++) {


		bool check_result = check_for_running_target(target_w, module_path);
		check_count += 1;
		if (check_count > 5) {
			break;
		}

		if (check_result && delay != -1) {
			break;
		}
		Sleep(1000);

		if (launched && seconds == 3) {
			printf("\nStill waiting for the game to start...\n"
				"If the game does not launch automatically, leave this window open and run it manually.\n"
				"You can also adjust/remove the [Loader] launch= option in the d3dx.ini as desired.\n\n");
		}
	}

	for (int i = delay; i > 0; i--) {
		printf("提示:为了节约系统资源, 加载器将于指定读秒后自动退出（可以不用管启动器了） %i...\r", i);
		Sleep(1000);
		check_for_running_target(target_w, module_path);
	}
	printf("\n");
}

static void elevate_privileges()
{
	DWORD size = sizeof(TOKEN_ELEVATION);
	TOKEN_ELEVATION Elevation;
	wchar_t path[MAX_PATH];
	HANDLE token = NULL;
	int rc;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token))
		return;

	if (!GetTokenInformation(token, TokenElevation, &Elevation, sizeof(Elevation), &size)) {
		CloseHandle(token);
		return;
	}

	CloseHandle(token);

	if (Elevation.TokenIsElevated)
		return;

	if (!GetModuleFileName(NULL, path, MAX_PATH))
		return;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	rc = (int)(uintptr_t)ShellExecute(NULL, L"runas", path, NULL, NULL, SW_SHOWNORMAL);
	if (rc > 32) // Success
		exit(0);
	if (rc == SE_ERR_ACCESSDENIED)
		wait_exit(EXIT_FAILURE, _strdup("Unable to run as admin: Access Denied\n"));
	printf("Unable to run as admin: %d\n", rc);
	wait_exit(EXIT_FAILURE);
}

wchar_t* deduce_working_directory(wchar_t* setting, wchar_t dir[MAX_PATH])
{
	DWORD ret;
	wchar_t* file_part = NULL;

	ret = GetFullPathName(setting, MAX_PATH, dir, &file_part);
	if (!ret || ret >= MAX_PATH)
		return NULL;

	ret = GetFileAttributes(dir);
	if (ret == INVALID_FILE_ATTRIBUTES)
		return NULL;

	if (!(ret & FILE_ATTRIBUTE_DIRECTORY) && file_part)
		*file_part = '\0';

	printf("Using working directory: \"%S\"\n", dir);

	return dir;
}

//
//static bool check_file_description(const char* buf, const char* module_path)
//{
//	// https://docs.microsoft.com/en-gb/windows/desktop/api/winver/nf-winver-verqueryvaluea
//	struct LANGANDCODEPAGE {
//		WORD wLanguage;
//		WORD wCodePage;
//	} *translate_query;
//	char id[50];
//	char* file_description = _strdup("");
//	unsigned int query_size, file_desc_size;
//	HRESULT hr;
//	unsigned i;
//
//	if (!VerQueryValueA(buf, "\\VarFileInfo\\Translation", (void**)&translate_query, &query_size))
//		wait_exit(EXIT_FAILURE, _strdup("3DMigoto file information query failed\n"));
//
//	// Look for the 3DMigoto file description in all language blocks... We
//	// could likely skip the loop since we know which language it should be
//	// in, but for some reason we included it in the German section which
//	// we might want to change, so this way we won't need to adjust this
//	// code if we do:
//	for (i = 0; i < (query_size / sizeof(struct LANGANDCODEPAGE)); i++) {
//		hr = _snprintf_s(id, 50, 50, "\\StringFileInfo\\%04x%04x\\FileDescription",
//			translate_query[i].wLanguage,
//			translate_query[i].wCodePage);
//		if (FAILED(hr))
//			wait_exit(EXIT_FAILURE, _strdup("3DMigoto file description query bugged\n"));
//
//		if (!VerQueryValueA(buf, id, (void**)&file_description, &file_desc_size))
//			wait_exit(EXIT_FAILURE, _strdup("3DMigoto file description query failed\n"));
//
//		// Only look for the 3Dmigoto prefix. We've had a whitespace
//		// error in the description for all this time that we want to
//		// ignore, and we later might want to add other 3DMigoto DLLs
//		// like d3d9 and d3d12 with injection support
//		printf("%s description: \"%s\"\n", module_path, file_description);
//		if (!strncmp(file_description, "3Dmigoto", 8))
//			return true;
//	}
//
//	return false;
//}
//
//static void check_3dmigoto_version(const char* module_path, const char* ini_section)
//{
//	VS_FIXEDFILEINFO* query = NULL;
//	DWORD pointless_handle = 0;
//	unsigned int size;
//	char* buf;
//
//	size = GetFileVersionInfoSizeA(module_path, &pointless_handle);
//	if (!size)
//		wait_exit(EXIT_FAILURE, _strdup("3DMigoto version size check failed\n"));
//
//	buf = new char[size];
//
//	if (!GetFileVersionInfoA(module_path, pointless_handle, size, buf))
//		wait_exit(EXIT_FAILURE, _strdup("3DMigoto version info check failed\n"));
//
//	if (!check_file_description(buf, module_path)) {
//		printf("ERROR: The requested module \"%s\" is not 3DMigoto\n"
//			"Please ensure that [Loader] \"module\" is set correctly and the DLL is in place.", module_path);
//		wait_exit(EXIT_FAILURE);
//	}
//
//	if (!VerQueryValueA(buf, "\\", (void**)&query, &size))
//		wait_exit(EXIT_FAILURE, _strdup("3DMigoto version query check failed\n"));
//
//	printf("3DMigoto Version %d.%d.%d\n",
//		query->dwProductVersionMS >> 16,
//		query->dwProductVersionMS & 0xffff,
//		query->dwProductVersionLS >> 16);
//
//	if (query->dwProductVersionMS < 0x00010003 ||
//		query->dwProductVersionMS == 0x00010003 && query->dwProductVersionLS < 0x000f0000) {
//		wait_exit(EXIT_FAILURE, _strdup("This version of 3DMigoto is too old to be safely loaded - please use 1.3.15 or later\n"));
//	}
//
//	delete[] buf;
//}


int main()
{
	char* buf, target[MAX_PATH], setting[MAX_PATH], module_path[MAX_PATH];

	//仅用于临时兼容性测试
	char params[MAX_PATH];
	
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

	//判断是否有另一个3DmigotoLoader在运行
	CreateMutexA(0, FALSE, "Local\\3DMigotoLoader");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		wait_exit(EXIT_FAILURE, _strdup("错误: 另外一个3Dmigoto加载器进程正在运行，请不要同时打开多个3Dmigoto加载器\n"));

	printf("\n------------------------------- 3DMigoto-Armor专用加载器------------------------------\n");
	printf("\n---------------------------- 如无法生效，请使用管理员运行此程序-----------------------\n\n");

	ini_file = CreateFile(L"d3dx.ini", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (ini_file == INVALID_HANDLE_VALUE)
		exit_usage("无法打开d3dx.ini\n");

	filesize = GetFileSize(ini_file, NULL);
	buf = new char[filesize + 1];
	if (!buf)
		wait_exit(EXIT_FAILURE, _strdup("无法为d3dx.ini分配缓冲区\n"));

	if (!ReadFile(ini_file, buf, filesize, &readsize, 0) || filesize != readsize)
		wait_exit(EXIT_FAILURE, _strdup("无法读取d3dx.ini\n"));

	CloseHandle(ini_file);

	ini_section = find_ini_section_lite(buf, "loader");
	if (!ini_section)
		exit_usage("d3dx.ini 缺少所需的 [Loader] 部分\n");

	// Check that the target is configured. We don't do anything with this
	// setting from here other than to make sure it is set, because the
	// injection method we are using cannot single out a specific process.
	// Once 3DMigoto has been injected it into a process it will check this
	// value and bail if it is in the wrong one.
	if (!find_ini_setting_lite(ini_section, "target", target, MAX_PATH))
		exit_usage("d3dx.ini [Loader] 部分缺少了所需的 \"target\" 设置\n");


	if (!find_ini_setting_lite(ini_section, "module", module_path, MAX_PATH))
		exit_usage("d3dx.ini [Loader] 部分缺少了所需的 \"module\" 设置\n");

	// We've had support for this injection method in 3DMigoto since 1.3.5,
	// however until 1.3.15 it lacked the check in DllMain to bail out of
	// unwanted processes, so that is the first version we consider safe to
	// use for injection and by default we will not allow older DLLs.
	// Disabling this version check can allow the injector to work with
	// third party DLLs that support the same injection method, such as
	// Helix Mod.
	
	//这里会提示无法解析的外部符号 GetFileVersionInfoSizeA  VarQueryValueA GetFileVersionInfoA
	//if (find_ini_bool_lite(ini_section, "check_version", true))
	//	check_3dmigoto_version(module_path, ini_section);

	if (find_ini_bool_lite(ini_section, "require_admin", false))
		elevate_privileges();


	//we always need admin privileges.
	// Nico: 这玩意可能导致程序被识别为病毒，尽量不提权
	//elevate_privileges();

	module = LoadLibraryA(module_path);
	if (!module) {
		printf("无法加载3DMigoto \"%s\"\n", module_path);
		wait_exit(EXIT_FAILURE);
	}

	GetModuleFileName(module, module_full_path, MAX_PATH);
	printf("d3d11.dll 已准备就绪 %S\n", module_full_path);
	std::cout << "需注入的目标程序:" + std::string(target) << "\n" << "\n";
	if (!std::filesystem::exists(target)) {
		std::cout << "需注入的目标程序不存在于磁盘位置，请检查你的路径是否设置正确。\r\n1.请使用完整路径且不要出现中文路径。\r\n2.请从文件夹路径栏复制，右键文件属性中复制的路径会有看不见的特殊符号导致无法读取。" << "\n" << "\n";
	}

	if (find_ini_setting_lite(ini_section, "entry_point", setting, MAX_PATH))
		fn = GetProcAddress(module, setting);
	else
		fn = GetProcAddress(module, "CBTProc");
	if (!fn) {
		wait_exit(EXIT_FAILURE, _strdup("Module does not support injection method\n"
			"Make sure this is a recent 3DMigoto d3d11.dll\n"));
	}

	hook_proc = find_ini_int_lite(ini_section, "hook_proc", WH_CBT);

	hook_proc = WH_CBT;
	hook = SetWindowsHookEx(hook_proc, (HOOKPROC)fn, module, 0);
	if (!hook)
		wait_exit(EXIT_FAILURE, _strdup("Error installing hook\n"));

	rc = EXIT_SUCCESS;

	printf("3DMigoto加载器已准备就绪，现在请启动游戏.\n\n");

	launch = find_ini_setting_lite(ini_section, "launch", setting, MAX_PATH);
	if (launch) {
		printf("3DMigoto ready, launching \"%s\"...\n", setting);
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

		if (!MultiByteToWideChar(CP_UTF8, 0, setting, -1, setting_w, MAX_PATH))
			wait_exit(EXIT_FAILURE, _strdup("Invalid launch setting\n"));

		working_dir_p = deduce_working_directory(setting_w, working_dir);

		// Get launch_args from ini
		// 
		if (find_ini_setting_lite(ini_section, "launch_args", params, MAX_PATH)) {

			printf("使用启动参数: \"%s\"\n", params);
			// Convert from char -> wchar_t for ShellExecute
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, params, strlen(params), NULL, 0);
			wchar_t* paramsb = new wchar_t[size_needed + 1];
			MultiByteToWideChar(CP_UTF8, 0, params, strlen(params), paramsb, size_needed);

			ShellExecute(NULL, NULL, setting_w, paramsb, working_dir_p, SW_SHOWNORMAL);
			delete[] paramsb;
		}
		else {
			ShellExecute(NULL, NULL, setting_w, NULL, working_dir_p, SW_SHOWNORMAL);
		}
	}
	else {
		printf("3DMigoto ready - Now run the game.\n");
	}


	//uncomment this to always inject to specific processs without reading d3dx.ini
	//char target_str[] = "C:\\Program Files\\Star Rail\\Game\\StarRail.exe";

	//Here we always set delay to 5
	// means it will automatically quit when arrived 5 seconds don't care if it's success inject.
	wait_for_target(target, module_full_path,
		true,
		ExitAfterSeconds, false);

	UnhookWindowsHookEx(hook);
	delete[] buf;

	//usually we don't need to pause.
	//system("pause");

	return rc;
}




