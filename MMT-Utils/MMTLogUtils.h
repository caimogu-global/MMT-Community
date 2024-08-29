#pragma once
#include <iostream>
#include "easylogging++.h"

class MMTLogger {
public:
	std::wstring RunResultJsonPath;

	//日志记录功能
	void Info(const std::wstring);
	void Info(const std::string);

	void Warning(const std::wstring);
	void Warning(const std::string);

	void Error(const std::wstring);
	void Error(const std::string);

	void NewSeperator();

	void NewLine();
	void Success();

	void SaveResultJson(const std::wstring);
	void SaveResultJson(const std::string);

	MMTLogger();
	MMTLogger(const std::wstring ApplicationLocation);
};

//全局日志类
extern MMTLogger LOG;
