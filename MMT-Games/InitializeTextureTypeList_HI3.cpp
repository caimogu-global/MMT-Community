#include "Functions_HI3.h"


void Functions_HI3::InitializeTextureTypeList() {
	std::vector<TextureType> TmpTextureTypeList;

	//崩坏三2.0角色通用 (仅无法线贴图角色)
	TextureType Body_T0_012345678910;
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t0"] = "DiffuseMap.dds";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t1"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t2"] = "LightMap.dds";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t7"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t8"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Body_T0_012345678910.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	TmpTextureTypeList.push_back(Body_T0_012345678910);

	//崩坏三2.0角色 有法线贴图的角色
	TextureType Body_T0_0123456789101112;
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t0"] = "DiffuseMap.dds";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t1"] = "NormalMap.dds";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t2"] = "LightMap.dds";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t7"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t8"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Body_T0_0123456789101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpTextureTypeList.push_back(Body_T0_0123456789101112);

	//魔法少女西琳 奇迹魔法少女
	TextureType Body_T1_01234589101112;
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t0"] = "LightMap.dds";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t8"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Body_T1_01234589101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpTextureTypeList.push_back(Body_T1_01234589101112);

	this->TextureTypeList = TmpTextureTypeList;
}