#include "Functions_WW1.1.h"


void Functions_WW11::InitializeTextureTypeList() {
	// 排序规则：
	// 1.总槽位数量相同的情况下，有效槽位越少应该越靠前越先识别
	std::vector<TextureType> TmpList;

	//这些是已经测试好的
	TextureType Body_T1_0123;
	Body_T1_0123.PixelSlot_TextureType_Map["ps-t0"] = "LightMap.dds";
	Body_T1_0123.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Body_T1_0123.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Body_T1_0123.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	TmpList.push_back(Body_T1_0123);

	TextureType Body_T2_0123_49101112;
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";

	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Body_T2_0123_49101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpList.push_back(Body_T2_0123_49101112);

	TextureType Eye_T1_0123_49101112;
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t0"] = "ShadowMap.dds";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t2"] = "LightMap1.dds";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t3"] = "LightMap2.dds";

	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Eye_T1_0123_49101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpList.push_back(Eye_T1_0123_49101112);


	TextureType Face_T1_012_349101112;
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t0"] = "ShadowMap.dds";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t2"] = "LightMap.dds";

	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Face_T1_012_349101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpList.push_back(Face_T1_012_349101112);


	TextureType Hair_T0_01_249101112;
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t0"] = "DiffuseMap.dds";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Hair_T0_01_249101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpList.push_back(Hair_T0_01_249101112);


	TextureType Hair_T1_0123456;
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t2"] = "LightMap.dds";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Hair_T1_0123456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Hair_T1_0123456);



	TextureType Body_T1_0123456;
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t2"] = "HighLightMap.dds";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t3"] = "ShadowMap.dds";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T1_0123456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Body_T1_0123456);

	TextureType Hair_T0_01_23456;
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t0"] = "DiffuseMap.dds";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Hair_T0_01_23456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Hair_T0_01_23456);

	TextureType Hair_T0_01_234567;
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t0"] = "DiffuseMap.dds";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	Hair_T0_01_234567.PixelSlot_TextureType_Map["ps-t7"] = "NicoMico";
	TmpList.push_back(Hair_T0_01_234567);

	TextureType Body_T4_0123456;
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t1"] = "NicoMico";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t3"] = "ShadowMap.dds";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t4"] = "DiffuseMap.dds";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t5"] = "HighLightMap.dds";
	Body_T4_0123456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Body_T4_0123456);

	TextureType Hair_T3_0123456;
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t1"] = "NicoMico";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t3"] = "DiffuseMap.dds";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t4"] = "LightMap.dds";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Hair_T3_0123456.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Hair_T3_0123456);

	

	TextureType Eye_T2_012_34569101112;
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t0"] = "ShadowMap.dds";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t1"] = "HighLightMap.dds";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t9"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t10"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t11"] = "NicoMico";
	Eye_T2_012_34569101112.PixelSlot_TextureType_Map["ps-t12"] = "NicoMico";
	TmpList.push_back(Eye_T2_012_34569101112);

	TextureType Body_T1_012_34;
	Body_T1_012_34.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T1_012_34.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Body_T1_012_34.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Body_T1_012_34.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T1_012_34.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	TmpList.push_back(Body_T1_012_34);

	TextureType Eye_T2_012_34;
	Eye_T2_012_34.PixelSlot_TextureType_Map["ps-t0"] = "ShadowMap.dds";
	Eye_T2_012_34.PixelSlot_TextureType_Map["ps-t1"] = "HighLightMap.dds";
	Eye_T2_012_34.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Eye_T2_012_34.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Eye_T2_012_34.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	TmpList.push_back(Eye_T2_012_34);

	TextureType Body_T2_012_34567;
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	Body_T2_012_34567.PixelSlot_TextureType_Map["ps-t7"] = "NicoMico";
	TmpList.push_back(Body_T2_012_34567);

	TextureType Body_T2_012_345;
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t2"] = "DiffuseMap.dds";
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T2_012_345.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	TmpList.push_back(Body_T2_012_345);

	TextureType Body_T1_01_2345;
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t0"] = "NormalMap.dds";
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t1"] = "DiffuseMap.dds";
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Body_T1_01_2345.PixelSlot_TextureType_Map["ps-t5"] = "NicoMico";
	TmpList.push_back(Body_T1_01_2345);

	TextureType Eye_T5_015_2346;
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t0"] = "ShadowMap.dds";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t1"] = "LightMap.dds";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t5"] = "DiffuseMap.dds";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t2"] = "NicoMico";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t3"] = "NicoMico";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t4"] = "NicoMico";
	Eye_T5_015_2346.PixelSlot_TextureType_Map["ps-t6"] = "NicoMico";
	TmpList.push_back(Eye_T5_015_2346);

	this->TextureTypeList = TmpList;
}