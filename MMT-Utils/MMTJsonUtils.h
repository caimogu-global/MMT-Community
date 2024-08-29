#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include "json.hpp"

namespace MMTJson {
	//用于读取Json文件到对象
	nlohmann::json ReadJsonFromFile(const std::wstring&);

	//保存到json文件,输出的json文件路径，json对象
	void SaveToJsonFile(std::wstring, nlohmann::json);

}

