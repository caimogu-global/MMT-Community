#include "Functions_SnB.h"


void Functions_SnB::InitializeTextureTypeList() {
	std::vector<TextureType> TmpList;

	TextureType Body_T2_2456;
	Body_T2_2456.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Body_T2_2456.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T2_2456.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T2_2456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Body_T2_2456);

	TextureType Face_T6_0567;
	Face_T6_0567.PixelSlot_TextureType_Map["ps-t0"] = "NicoMico";
	Face_T6_0567.PixelSlot_TextureType_Map["ps-t5"] = "NormalMap.dds";
	Face_T6_0567.PixelSlot_TextureType_Map["ps-t6"] = "DiffuseMap.dds";
	Face_T6_0567.PixelSlot_TextureType_Map["ps-t7"] = "LightMap.dds";
	TmpList.push_back(Face_T6_0567);

	TextureType Universal_T6_567;
	Universal_T6_567.PixelSlot_TextureType_Map["ps-t5"] = "NormalMap.dds";
	Universal_T6_567.PixelSlot_TextureType_Map["ps-t6"] = "DiffuseMap.dds";
	Universal_T6_567.PixelSlot_TextureType_Map["ps-t7"] = "LightMap.dds";
	TmpList.push_back(Universal_T6_567);

	this->TextureTypeList = TmpList;
}