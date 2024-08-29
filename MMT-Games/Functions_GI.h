#pragma once

#include "Functions_Basic.h"


class Functions_GI : public Functions_Basic{

public:
	void ExtractModel() override;

	void GenerateMod() override;

	void InitializeTextureTypeList() override;

	void AutoDetectTextureFiles(std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map, bool GPUPreSkinning);

	std::wstring AutoGameType(std::wstring DrawIB);

};

