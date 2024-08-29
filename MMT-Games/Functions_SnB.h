#pragma once


#include "Functions_Basic.h"


class Functions_SnB : public Functions_Basic {
private:
	void ExtractFromBuffer_VS_UE4(std::wstring DrawIB, std::wstring GameType);

public:
	void ExtractModel() override;
	void GenerateMod() override;
	void InitializeTextureTypeList() override;
};

