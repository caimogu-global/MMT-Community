#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <istream>
#include <fstream>

namespace MMTFile{
	//移动文件到回收站而不是直接删除，防止数据丢失
	//实际测试发现有BUG，有的时候无法移动文件到回收站，所以不推荐使用
	bool MoveFileToRecycleBin_Deprecated(const std::wstring& filePath);

	std::vector<std::wstring> FindFileNameListWithCondition(std::wstring SearchFolderPath, std::wstring searchStr, std::wstring endStr);
	std::vector<std::wstring> FindTextureFileList(std::wstring SearchFolderPath, std::wstring FilterStr);

	std::vector<std::wstring> ReadIniFileLineList(std::wstring filePath);

	std::vector<std::wstring> GetFilePathListRecursive(std::wstring directory);

	//读取文件的所有行，一般用于ini读取或分析
	std::vector<std::wstring> ReadAllLinesW(std::wstring filePath);

	//删除指定目录下指定后缀名文件，用于清理上一次生成的Mod文件。
	void DeleteFilesWithSuffix(std::wstring directory, std::wstring suffix);

	std::unordered_map<uint64_t, std::vector<std::byte>> ReadBufMapFromFile(std::wstring readPathW, uint64_t vertexNumber);

	//读取文件大小
	uint64_t GetFileSize(std::wstring FileName);

	//读取特殊的结尾被填充特别多0的文件的真实的不为0部分的文件大小
	uint64_t GetRealFileSize_NullTerminated(std::wstring FileName);

	std::wstring FindMaxPrefixedDirectory(const std::wstring& directoryPath, const std::wstring& prefix);

	//在文件中查找指定3Dmigoto的ini类型属性，比如查找format,需提供绝对路径，比直接分析整个文件的其它方法更快
	std::wstring FindMigotoIniAttributeInFile(const std::wstring& filePath, const std::wstring& attributeName);

	bool IsValidFilename(std::string filename);

	void ReadDirectoryRecursively(const std::string& directory, std::vector<std::string>& filePaths);
}
