#pragma once

#include "Functions_Basic.h"


//TODO 由于鸣潮更新取消了CS计算，所以现在这些代码只有参考作用了。
class Functions_WW: public Functions_Basic{
private:
	void Extract_GPU_PreSkinning_WW(std::wstring DrawIB, std::wstring GameType);
	void Extract_Object_WW(std::wstring DrawIB, std::wstring GameType);

public:
	void ExtractModel() override;
	void GenerateMod() override;
	void InitializeTextureTypeList() override;
};
