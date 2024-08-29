#pragma once

#include <vector>
#include <unordered_map>

#include "D3d11GameType.h"
#include "D3D11Texture.h"
#include "TextureSlotReplace.h"
#include "MMTJsonUtils.h"
#include "DrawIBConfig.h"




//基类，方便管理和代码复用，存放案例方法子类可以有独特实现。
class Functions_Basic {
//仅允许子类访问的方法放到protected里，在这里存放公用方法。
protected:

	//数据类型相关属性
	std::vector<D3D11GameType> CurrentD3d11GameTypeList;
	std::vector<std::string> Ordered_GPU_Object_D3D11GameTypeList;
	std::unordered_map<std::string, D3D11GameType> GameTypeName_D3d11GameType_Map;

	//贴图相关属性
	std::vector<TextureType> TextureTypeList;
	std::unordered_map<std::string, TextureSlotReplace> PartName_TextureSlotReplace_Map; //这个每个DrawIB用之前记得都清理一下，它只是起一个变量传递的作用。
	virtual void AutoDetectTextureFiles(std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map, bool GPUPreSkinning) {};
	virtual void MoveAllUsedTexturesToOutputFolder(std::wstring DrawIB, std::wstring OutputFolder);
	virtual void GenerateHashTextureIni_Deprecated(std::wstring ModOutputFolder);
	virtual void GenerateHashTextureIni_2(std::wstring ModOutputFolder);

	//自动数据类型识别算法
	virtual std::wstring AutoGameType(std::wstring DrawIB);
	virtual std::wstring AutoGameType_Reverse(std::wstring TrianglelistIBIndex, uint32_t TrianglelistVertexCount);

	void InitializeGameTypeList(std::unordered_map<std::string, nlohmann::json> GameTypeName_GameTypeJson_Map);

//虚函数多态导出调用窗口，每个游戏都有自己的实现
public:
	//运行前的前置配置初始化 
	virtual void InitializeGameTypeListFromConfig(std::wstring GameTypeConfigPath);
	virtual void InitializeGameTypeListFromPreset() {};

	//初始化贴图自动识别算法
	virtual void InitializeTextureTypeList() {};

	//最基础的提取模型和生成Mod 必选
	virtual void ExtractModel() { };
	virtual void GenerateMod() { };

	//逆向提取和一键逆向
	//virtual void ExtractModel_Reverse();
	//virtual void ReverseSingle();
	//virtual void ReverseMerged_Toggle();
	//virtual void ReverseMerged_OutfitCompiler();
	//virtual void ReverseMerged_NameSpace();
	//virtual void Reverse_3DmigotoSimulater();
};

