#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "TextureSlotReplace.h"
#include "D3d11GameType.h"

class DrawIBConfig {
public:
	//2.通用项目
	std::wstring DrawIB;
	std::wstring GameType;

	//4.Split配置
	std::unordered_map <std::string, std::string> ColorMap;
	std::wstring TangentAlgorithm;
	std::wstring ColorAlgorithm;

	bool ForbidAutoTexture = false;
	bool UseHashTexture = false;

	bool NormalReverseX = false;
	bool NormalReverseY = false;
	bool NormalReverseZ = false;

	bool TangentReverseX = false;
	bool TangentReverseY = false;
	bool TangentReverseZ = false;
	bool TangentReverseW = false;

	//这玩意Merge的时候保存到tmp.json，然后Split的时候从hash.json中提取
	std::unordered_map <std::string, std::string> CategoryHashMap;
	std::vector<std::string> MatchFirstIndexList;
	std::vector<std::string> PartNameList;
	std::vector<std::string> TmpElementList;
	std::string VertexLimitVB;
	std::string WorkGameType;

	std::unordered_map<std::string, std::vector<std::string>> PartName_TextureSlotReplace_Map;

	void SaveTmpJsonConfigs(std::wstring outputPath);


	//下面这些是生成Mod时的属性
	D3D11GameType d3d11GameType;
	std::wstring ModOutputFolder;
	std::wstring BufferReadFolder;
	std::wstring BufferOutputFolder;
	std::wstring TextureOutputFolder;
	std::wstring CreditOutputFolder;
	std::wstring ModelFolder;
	uint32_t DrawNumber;
	std::unordered_map<std::wstring, std::vector<std::byte>> FinalVBCategoryDataMap;

	bool Initialize(std::wstring OutputFolder, D3D11GameType InputD3d11GameType);
};