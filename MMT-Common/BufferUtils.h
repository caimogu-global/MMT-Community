#pragma once

#include <string>
#include <unordered_map>

#include "GlobalConfigs.h"
#include "D3d11GameType.h"

#include "IniBuilder.h"

namespace BufferUtils {
	IniSection GetCreditInfoIniSection();
	void AddCreditREADME();

	//分享导出的源模型
	void ShareSourceModel(DrawIBConfig IBConfig);

	//COLOR重计算，使用基于算数平均的AverageNormal重计算方法
	void Unity_COLOR_AverageNormal(std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType);
	//TANGENT使用基于Vector的AverageNormal归一化重计算方法
	void Unity_TANGENT_AverageNormal(std::unordered_map<std::wstring, std::vector<std::byte>> &finalVBCategoryDataMap, D3D11GameType d3d11GameType);
	//COLOR值直接赋予
	void Unity_Reset_COLOR(std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType);
	//翻转NORMAL和TANGENT的值
	void Unity_Reverse_NORMAL_TANGENT(std::unordered_map<std::wstring, std::vector<std::byte>>& finalVBCategoryDataMap, DrawIBConfig basicConfig, D3D11GameType d3d11GameType);

	//生成Mod用的---------------------------------------------
	uint32_t GetSum_DrawNumber_FromVBFiles( DrawIBConfig IBConfig);
	std::unordered_map<std::wstring, std::vector<std::byte>> Read_FinalVBCategoryDataMap( DrawIBConfig IBConfig);
	void CopySlotDDSTextureFromOutputFolder( DrawIBConfig IBConfig);
	void Read_Convert_Output_IBBufferFiles( DrawIBConfig IBConfig);
	void OutputCategoryBufferFiles( DrawIBConfig IBConfig);
}