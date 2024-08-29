#pragma once


#include "Functions_Basic.h"

//TODO 要添加幻塔支持的话，必须得先学会WWMI中所有技巧，而且是老版本的第一个WWMI版本才行。
class Functions_ToF : public Functions_Basic {
private:
	void Extract_GPU_PreSkinning_WW(std::wstring DrawIB, std::wstring GameType);
	void Extract_Object_WW(std::wstring DrawIB, std::wstring GameType);
public:
	void ExtractModel() override;
	void GenerateMod() override;
	void InitializeTextureTypeList() override;
};

