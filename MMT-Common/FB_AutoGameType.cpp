#include "Functions_Basic.h"

#include "FrameAnalysisData.h"
#include "MMTLogUtils.h"
#include "GlobalConfigs.h"
#include "VertexBufferTxtFileDetect.h"


std::wstring Functions_Basic::AutoGameType(std::wstring DrawIB) {
	FrameAnalysisData FAData(G.WorkFolder, DrawIB);

	std::map<std::wstring, uint32_t> PointlistIndexVertexCountMap = FAData.Read_PointlistIndex_VertexCount_Map(VertexCountType::BufFileTotal);
	std::map<std::wstring, uint32_t> TrianglelistIndexVertexCountMap = FAData.Read_TrianglelistIndex_VertexCount_Map(VertexCountType::TxtFileShow, DrawIB);

	std::vector<std::string> MatchedGameTypeList;
	for (std::string GameTypeName : Ordered_GPU_Object_D3D11GameTypeList) {
		D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[GameTypeName];
		LOG.Info("Try to match with GameType: " + GameTypeName);

		if (TrianglelistIndexVertexCountMap.size() == 0 && d3d11GameType.GPUPreSkinning == true) {
			LOG.Warning("GameType: " + d3d11GameType.GameType + " use GPU-PreSkinning,but can't find any pointlist file in FrameAnalysisFolder , so skip this.");
			LOG.NewLine();
			continue;
		}

		uint64_t MaxDrawNumber = 0;
		std::wstring TrianglelistExtractIndex = L"";
		for (const auto& TrianglelistIndexPair: TrianglelistIndexVertexCountMap) {
			if (TrianglelistIndexPair.second > MaxDrawNumber) {
				MaxDrawNumber = TrianglelistIndexPair.second;
				TrianglelistExtractIndex = TrianglelistIndexPair.first;
			}
		}

		std::wstring PointlistExtractIndex = L"";
		for (const auto& PointlistIndexPair: PointlistIndexVertexCountMap) {
			if (PointlistIndexPair.second == MaxDrawNumber) {
				PointlistExtractIndex = PointlistIndexPair.first;
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
		for (const auto& categoryPair : d3d11GameType.CategorySlotMap) {
			std::string CategoryName = categoryPair.first;
			std::string CategorySlot = categoryPair.second;
			std::string CategoryExtractTechnique = d3d11GameType.CategoryTopologyMap[CategoryName];
			std::wstring ExtractIndex = TrianglelistExtractIndex;
			if (CategoryExtractTechnique == "pointlist") {
				ExtractIndex = PointlistExtractIndex;
			}
			LOG.Info("CategoryName:" + CategoryName + " CategorySlot:" + CategorySlot + " CategoryExtractTechnique:" + CategoryExtractTechnique);
			LOG.Info(L"ExtractIndex:" + ExtractIndex);

			std::vector<std::wstring> CategorySlotFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(ExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".buf");
			if (CategorySlotFileNameList.size() == 0) {
				LOG.Error("Can't find slot file for category,please check and repair TrianglelistExtractIndex algorithm");
			}
			std::wstring CategorySlotFileName = CategorySlotFileNameList[0];
			uint8_t CategoryStride = d3d11GameType.CategoryStrideMap[CategoryName];


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
				}
				else {
					AllMatch = false;
					LOG.Info("Category Can't Match! ");
				}
			}
			else {
				if (CategoryName == "Texcoord") {
					//判断txt文件中现实的elementlist大小是否和数据类型中的相同

					std::vector<std::wstring> TmpCategorySlotFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(ExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".txt");
					if (TmpCategorySlotFileNameList.size() == 0) {
						LOG.Error("Can't find slot file for category,please check and repair TrianglelistExtractIndex algorithm");
					}
					std::wstring TmpCategorySlotFileName = TmpCategorySlotFileNameList[0];
					VertexBufferDetect vbBufDetect(G.WorkFolder + TmpCategorySlotFileName);
					size_t TxtSize = vbBufDetect.realElementList.size();
					size_t GameTypeSize = d3d11GameType.getCategoryElementList(d3d11GameType.OrderedFullElementList, "Texcoord").size();
					LOG.Info("TxtSize:" + std::to_string(TxtSize) + "   GameTypeSize:" + std::to_string(GameTypeSize));
					if (TxtSize != GameTypeSize) {
						AllMatch = false;
						LOG.Info("Texcoord Category Can't Match! Txt Show ElementList size is not equalt to GameType's element list");
					}

				}
				else {
					LOG.Info("Category Match! ");
				}
				
			}
			LOG.NewLine();
		}

		if (AllMatch) {
			LOG.Info("GameType:" + d3d11GameType.GameType + "  Matched!");
			MatchedGameTypeList.push_back(d3d11GameType.GameType);

			//这里使用偷懒写法，既然已经确定了先匹配GPU-PreSkinning类型，那么只要匹配到就可以结束匹配了。
			break;
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