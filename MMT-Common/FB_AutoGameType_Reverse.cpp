#include "Functions_Basic.h"

#include "FrameAnalysisData.h"
#include "MMTLogUtils.h"
#include "GlobalConfigs.h"
#include "VertexBufferTxtFileDetect.h"
#include "IndexBufferBufFile.h"

std::wstring Functions_Basic::AutoGameType_Reverse(std::wstring OriginalDrawIB,uint32_t TrianglelistVertexCount) {
	FrameAnalysisData FAData = G.GetFrameAnalysisData();

	std::vector<std::wstring> pointlistIndexList = FAData.ReadPointlistIndexList();

	std::vector<std::string> MatchedGameTypeList;
	for (std::string GameTypeName : Ordered_GPU_Object_D3D11GameTypeList) {
		D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[GameTypeName];
		LOG.Info("Try to match with GameType: " + GameTypeName);

		//自动类型识别，首先确定IB的大小
		if (pointlistIndexList.size() == 0 && d3d11GameType.GPUPreSkinning == true) {
			LOG.Info("GameType: " + d3d11GameType.GameType + " use GPU-PreSkinning,but can't find any pointlist file in FrameAnalysisFolder , so skip this.");
			LOG.NewLine();
			continue;
		}

		std::unordered_map<std::string, uint32_t> CategoryStrideMap = d3d11GameType.getCategoryStrideMap(d3d11GameType.OrderedFullElementList);
		uint8_t PositionStride = CategoryStrideMap["Position"];

		//强制赋值
		uint64_t MaxDrawNumber = TrianglelistVertexCount;
		LOG.Info(L"MaxDrawNumber:" + std::to_wstring(MaxDrawNumber));
		LOG.NewLine();

		std::wstring PointlistExtractIndex = L"";
		for (std::wstring PointlistIndex : pointlistIndexList) {
			//直接查询VB0的Buffer文件
			std::vector<std::wstring> VB0FileList = FAData.FindFrameAnalysisFileNameListWithCondition(PointlistIndex + L"-vb0", L".buf");
			if (VB0FileList.size() == 0) {
				continue;
			}
			std::wstring VB0FileName = VB0FileList[0];
			uint64_t VB0FileSize = MMTFile::GetFileSize(G.WorkFolder + VB0FileName);
			uint64_t VertexNumber = VB0FileSize / PositionStride;
			LOG.Info(L"PointlistIndex:" + PointlistIndex + L" VertexNumber:" + std::to_wstring(VertexNumber));
			if (VertexNumber == MaxDrawNumber) {
				MaxDrawNumber = VertexNumber;
				PointlistExtractIndex = PointlistIndex;
				break;
			}
		}
		if (PointlistExtractIndex == L"") {
			LOG.Info("Can't find any matched PointlistIndex!");
			if (d3d11GameType.GPUPreSkinning) {
				LOG.Info("Use GPU PreSkinning but can't find any match PointlistIndex, so skip this gametype: " + d3d11GameType.GameType);
				continue;
			}
		}
		else {
			LOG.Info(L"PointlistIndex Matched: " + PointlistExtractIndex);
		}
		LOG.NewLine();
		
		//接下来就是分Category来找对应的文件判断Stride是否相等
		LOG.Info("Start to verify if every category match:");
		bool AllMatch = true;
		std::unordered_map<std::string, bool> Category_Matched_Map;
		for (const auto& categoryPair : d3d11GameType.CategorySlotMap) {
			std::string CategoryName = categoryPair.first;
			std::string CategorySlot = categoryPair.second;
			std::string CategoryExtractTechnique = d3d11GameType.CategoryTopologyMap[CategoryName];
			std::wstring ExtractIndex = PointlistExtractIndex;

			// 如果是Trianglelist，由于存在部分Index的vb1槽位缺失问题
			// 所以这里要找到含有对应CategorySlot的buf文件才能作为提取对象
			if (CategoryExtractTechnique == "trianglelist") {
				std::unordered_map<std::wstring, IndexBufferBufFile> UniqueVertexCount_IBBufFile_Map = FAData.Get_MatchFirstIndex_IBBufFile_Map_FromLog(OriginalDrawIB);
				for (const auto& UniqueCountPair: UniqueVertexCount_IBBufFile_Map) {
					std::wstring TmpTrianglelistIndex = UniqueCountPair.second.Index;
					std::vector<std::wstring> CategorySlotFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(ExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".buf");
					if (CategorySlotFileNameList.size() == 0) {
						continue;
					}
					ExtractIndex = TmpTrianglelistIndex;
					break;
				}
			}
			LOG.Info("CategoryName:" + CategoryName + " CategorySlot:" + CategorySlot + " CategoryExtractTechnique:" + CategoryExtractTechnique);
			LOG.Info(L"ExtractIndex:" + ExtractIndex);

			std::vector<std::wstring> CategorySlotFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(ExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".buf");
			if (CategorySlotFileNameList.size() == 0) {
				LOG.Error("Can't find slot file for category,please check and repair TrianglelistExtractIndex algorithm");
			}
			std::wstring CategorySlotFileName = CategorySlotFileNameList[0];
			uint8_t CategoryStride = CategoryStrideMap[CategoryName];


			//如果补充默认的BLENDWEIGHTS的话，这里的Stride必须减去一个16
			if (d3d11GameType.PatchBLENDWEIGHTS && CategoryStride == 20 && CategoryName == "Blend") {
				CategoryStride = 4;
			}

			uint64_t CategorySlotFileSize = MMTFile::GetFileSize(G.WorkFolder + CategorySlotFileName);
			uint64_t FileStride = CategorySlotFileSize / MaxDrawNumber;
			LOG.Info("CategoryStride:" + std::to_string(CategoryStride) + " FileStride:" + std::to_string(FileStride));

			if (CategoryStride != FileStride) {
				//TODO TexcoordPatch
				if (d3d11GameType.TexcoordPatchNull != 0 && CategoryName == "Texcoord" && (FileStride - CategoryStride == d3d11GameType.TexcoordPatchNull)) {
					LOG.Warning(L"遇到了特殊情况:Texcoord槽位使用空白的0x00在每个数据的结尾填补指定位数以阻止模型提取，允许匹配。");
					if (!Category_Matched_Map.contains(CategoryName)) {
						Category_Matched_Map[CategoryName] = true;
						LOG.Info("Category Match! ");
					}
				}
				else {
					AllMatch = false;
					LOG.Info("Category Can't Match! ");
				}
			}
			else {
				if (!Category_Matched_Map.contains(CategoryName)) {
					Category_Matched_Map[CategoryName] = true;
					LOG.Info("Category Match! ");
				}
			}
			LOG.NewLine();
		}

		if (AllMatch) {
			//验证是否所有的Category都是true的
			bool allCategoryMatch = true;
			for (const auto& pair: d3d11GameType.CategoryDrawCategoryMap) {
				std::string CategoryName = pair.first;
				if (!Category_Matched_Map.contains(CategoryName)) {
					allCategoryMatch = false;
				}
			}

			if (allCategoryMatch) {
				LOG.Info("GameType:" + d3d11GameType.GameType + "  Matched!");
				MatchedGameTypeList.push_back(d3d11GameType.GameType);

				//这里使用偷懒写法，既然已经确定了先匹配GPU-PreSkinning类型，那么只要匹配到就可以结束匹配了。
				break;
			}
		}
		LOG.NewLine();

	}
	LOG.NewLine();

	std::string MatchedGameType = "";

	if (MatchedGameTypeList.size() == 1) {
		MatchedGameType = MatchedGameTypeList[0];
	}
	LOG.Info("MatchGameType: " + MatchedGameType);
	return MMTString::ToWideString(MatchedGameType);
}