#include "Functions_Basic.h"

#include "MMTJsonUtils.h"
#include "GlobalConfigs.h"


void Functions_Basic::InitializeGameTypeListFromConfig(std::wstring GameTypeConfigPath) {
	//拼接扫描的目标目录

	std::vector<std::wstring> GameTypeConfigJsonFileNameList = MMTFile::FindFileNameListWithCondition(GameTypeConfigPath, L"", L".json");

	std::unordered_map<std::string, nlohmann::json> GameTypeName_GameTypeJson_Map;

	for (std::wstring GameTypeJsonFileName : GameTypeConfigJsonFileNameList) {
		std::wstring GameTypeJsonPath = GameTypeConfigPath + GameTypeJsonFileName;
		nlohmann::json GameTypeJson = MMTJson::ReadJsonFromFile(GameTypeJsonPath);
		std::string GameTypeName = MMTString::ToByteString(MMTString::GetFileNameWithOutSuffix(GameTypeJsonFileName));
		GameTypeName_GameTypeJson_Map[GameTypeName] = GameTypeJson;
	}

	this->InitializeGameTypeList(GameTypeName_GameTypeJson_Map);
}