#include "Functions_Unity_CPU_PreSkinning.h"

#include "FrameAnalysisData.h"
#include "MMTLogUtils.h"
#include "GlobalConfigs.h"
#include "VertexBufferTxtFileDetect.h"



std::wstring Functions_Unity_CPU_PreSkinning::AutoGameType(std::wstring DrawIB) {
	FrameAnalysisData FAData(G.WorkFolder, DrawIB);
	std::vector<std::wstring> trianglelistIndexList = FAData.TrianglelistIndexList;
	std::vector<std::wstring> pointlistIndexList = FAData.PointlistIndexList;


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

		uint64_t MaxDrawNumber = 0;
		std::wstring TrianglelistExtractIndex = L"";
		//查找Trianglelist中包含DrawIB的VB0
		for (std::wstring TrianglelistIndex : trianglelistIndexList) {
			//直接查询VB0的Buffer文件
			std::vector<std::wstring> VB0FileList = FAData.FindFrameAnalysisFileNameListWithCondition(TrianglelistIndex + L"-vb0", L".txt");
			if (VB0FileList.size() == 0) {
				LOG.Info(L"Can't find -vb0 .txt file for index:" + TrianglelistIndex);
				continue;
			}

			//这里必须使用txt类型读取里面的顶点数，不然如果只有一个vb0类型则Object类型永远无法匹配，同时也能兼容绝区零里的技能界面提取。
			std::wstring VB0FileName = VB0FileList[0];
			VertexBufferDetect vbFileDetect(G.WorkFolder + VB0FileName);

			uint64_t VertexNumber = vbFileDetect.fileShowVertexCount;
			LOG.Info(L"TrianglelistIndex:" + TrianglelistIndex + L" VertexNumber(file show):" + std::to_wstring(VertexNumber));
			if (VertexNumber >= MaxDrawNumber) {
				MaxDrawNumber = VertexNumber;

				//TODO 这里要判断当前GameType的所有槽位都存在才行：
				bool AllCategorySlotExists = true;
				for (const auto& CategoryPair : d3d11GameType.CategorySlotMap) {
					std::vector<std::wstring> CategorySlotFileList = FAData.FindFrameAnalysisFileNameListWithCondition(TrianglelistIndex + L"-" + MMTString::ToWideString(CategoryPair.second), L".buf");
					if (CategorySlotFileList.size() == 0) {
						LOG.Warning(CategoryPair.second + " Slot Can't Match");
						AllCategorySlotExists = false;
						break;
					}
					else {
						LOG.Info(CategoryPair.second + " Slot Matched.");
					}
				}

				if (AllCategorySlotExists) {
					LOG.Info(L"All Slot Exists for Index:" + TrianglelistIndex);
					TrianglelistExtractIndex = TrianglelistIndex;
				}
				else {
					LOG.Warning(L"Can't find all category slot for trianglelist index: " + TrianglelistIndex + L", so skip this index.");
					LOG.NewLine();
				}

			}
		}
		LOG.Info(L"MaxDrawNumber:" + std::to_wstring(MaxDrawNumber));
		LOG.Info(L"ExtractIndex:" + TrianglelistExtractIndex);
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
					//size_t TxtSize = vbBufDetect.realElementList.size();
					//size_t GameTypeSize = d3d11GameType.getCategoryElementList(d3d11GameType.OrderedFullElementList, "Texcoord").size();
					//LOG.Info("TxtSize:" + std::to_string(TxtSize) + "   GameTypeSize:" + std::to_string(GameTypeSize));
					//if (TxtSize != GameTypeSize) {
					//	AllMatch = false;
					//	LOG.Info("Texcoord Category Can't Match! Txt Show ElementList size is not equalt to GameType's element list");
					//}

					uint32_t DetectCOLORSize = vbBufDetect.elementNameD3D11ElementMap["color"].ByteWidth;
					uint32_t GameTypeCOLORSize = d3d11GameType.ElementNameD3D11ElementMap["COLOR"].ByteWidth;
					LOG.Info("DetectCOLORSize:" + std::to_string(DetectCOLORSize) + "   GameTypeCOLORSize:" + std::to_string(GameTypeCOLORSize));
					if (DetectCOLORSize != GameTypeCOLORSize) {
						AllMatch = false;
						LOG.Info("COLOR Element Can't Match! Txt Show COLOR ByteWidth is not equalt to GameType's element ByteWidth");
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