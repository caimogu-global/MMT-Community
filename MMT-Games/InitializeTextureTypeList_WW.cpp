#include "Functions_WW.h"


void Functions_WW::InitializeTextureTypeList() {
	std::vector<TextureType> TmpList;

	TextureType Body_T2_0123;
	Body_T2_0123.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T2_0123.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Body_T2_0123.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Body_T2_0123.PixelSlot_TextureType_Map["ps-t3"] = "HighLightMap.dds";
	TmpList.push_back(Body_T2_0123);

	this->TextureTypeList = TmpList;
}