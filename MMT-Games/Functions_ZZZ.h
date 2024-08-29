#pragma once

#include "Functions_Basic.h"


class Functions_ZZZ : public Functions_Basic {
public:
	std::wstring AutoGameType(std::wstring DrawIB) override;
	void ExtractModel() override;
	void GenerateMod() override;
	void InitializeTextureTypeList() override;
	void AutoDetectTextureFiles( std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map,bool GPUPreSkinning) override;
};

