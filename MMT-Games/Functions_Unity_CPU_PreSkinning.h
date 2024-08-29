#pragma once

#include "Functions_Basic.h"


class Functions_Unity_CPU_PreSkinning : public Functions_Basic {
public:
	void ExtractModel() override;
	void GenerateMod() override;
	std::wstring AutoGameType(std::wstring DrawIB) override;

};

