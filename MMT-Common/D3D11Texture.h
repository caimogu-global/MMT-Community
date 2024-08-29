#pragma once
#include <string>
#include <vector>
#include <unordered_map>


class TextureType {
public:
	std::unordered_map<std::string, std::string> PixelSlot_TextureType_Map;

	//过滤机制：把角色和物体类型分开
	bool GPUPreSkinning = true;

	TextureType();

	std::vector<int> PsSetShaderResourceTimeList;

	bool TimeInPsSetShaderResourceTimeList(int time);

	uint32_t GetMeaningfulDdsFileCount();

	std::string GetDiffuseMapSlot();

	std::unordered_map<std::string,std::vector<uint32_t>> AliasName_FileSizeList_Map;
};



