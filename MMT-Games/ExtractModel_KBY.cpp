#include "Functions_KBY.h"


#include "D3d11GameType.h"
#include "IndexBufferTxtFile.h"
#include "VertexBufferBufFile.h"
#include "VertexBufferTxtFile.h"
#include "GlobalConfigs.h"
#include "VertexBufferTxtFileDetect.h"
#include "FmtFile.h"
#include "ConstantBufferBufFile.h"
#include <set>
#include "FrameAnalysisData.h"
#include "IndexBufferBufFile.h"



void Functions_KBY::ExtractFromBuffer_VS_UE4(std::wstring DrawIB, std::wstring GameType) {

    FrameAnalysisData FAData = G.GetFrameAnalysisData();

    DrawIBConfig extractConfig = G.DrawIB_ExtractConfig_Map[DrawIB];

    D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[MMTString::ToByteString(GameType)];
    LOG.Info("GameType: " + d3d11GameType.GameType);

    std::wstring OutputDrawIBFolder = G.OutputFolder + DrawIB + L"\\";
    std::filesystem::create_directories(OutputDrawIBFolder);
    LOG.Info(L"Create output folder for DrawIB: " + DrawIB + L"  Path:" + OutputDrawIBFolder);

    std::vector<std::wstring> FrameAnalyseFileNameList = FAData.FrameAnalysisFileNameList;
    LOG.Info("Read FrameAnalysis file name list success.");

    // 从vb0中读取到顶点数量然后在下面的过程中，UE4的VB0一般固定为POSITION长度固定为2
    // 限制顶点数量匹配到我们的DrawNumber来设置对应的索引
    std::map<int, std::wstring> matchFirstIndexIBFileNameMap;
    std::wstring VSExtractIndex;
    std::wstring PositionExtractFileName = L"";
    int MatchNumber = 0;
    int POSITION_ByteWidth = d3d11GameType.ElementNameD3D11ElementMap["POSITION"].ByteWidth;
    for (std::wstring filename : FrameAnalyseFileNameList) {
        if (!filename.ends_with(L".txt")) {
            continue;
        }
        if (filename.find(L"-ib=" + DrawIB) == std::wstring::npos) {
            continue;
        }
        IndexBufferTxtFile ibFileData(G.WorkFolder + filename, false);
        VSExtractIndex = ibFileData.Index;
        PositionExtractFileName = FAData.FindFrameAnalysisFileNameListWithCondition(VSExtractIndex + L"-vb0=", L".buf")[0];
        int POSITION_FileSize = MMTFile::GetFileSize(G.WorkFolder + PositionExtractFileName);
        MatchNumber = POSITION_FileSize / POSITION_ByteWidth;
        LOG.Info("Match DrawNumber: " + std::to_string(MatchNumber));

        LOG.Info(filename);
        if (ibFileData.FirstIndex == L"") {
            LOG.Info("Can't find FirstIndex attribute in this file, so skip this.");
            continue;
        }
        int matchFirstIndex = std::stoi(ibFileData.FirstIndex);
        matchFirstIndexIBFileNameMap[matchFirstIndex] = filename;
    }
    for (const auto& pair : matchFirstIndexIBFileNameMap) {
        LOG.Info("Match First Index: " + std::to_string(pair.first));
    }
    LOG.NewLine();

    std::unordered_map<std::string, uint32_t> CategoryStrideMap = d3d11GameType.getCategoryStrideMap(d3d11GameType.OrderedFullElementList);
    // 先使用OrderedElementList来做排序，防止顺序不正确。
    std::unordered_map<std::string, std::unordered_map<uint64_t, std::vector<std::byte>> > Category_CategoryBufMap_Map;
    for (std::string CategoryName : d3d11GameType.OrderedCategoryNameList) {
        std::string CategorySlot = d3d11GameType.CategorySlotMap[CategoryName];
        LOG.Info("CategoryName: " + CategoryName + "  CategorySlot: " + CategorySlot);
        std::vector<std::wstring> CategoryExtractFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(VSExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".buf");
        if (CategoryExtractFileNameList.size() >= 1) {
            std::wstring CategoryExtractFileName = CategoryExtractFileNameList[0];
            LOG.Info(L"CategoryExtractFileName: " + CategoryExtractFileName);
            std::unordered_map<uint64_t, std::vector<std::byte>> CategoryBufMap = MMTFile::ReadBufMapFromFile(G.WorkFolder + CategoryExtractFileName, MatchNumber);
            //判断当前Category读取到的文件的长度是否和CategoryStrideMap中的一致
            if (CategoryStrideMap[CategoryName] != CategoryBufMap[0].size()) {
                LOG.Info("RealStride:" + std::to_string(CategoryBufMap[0].size()));
                LOG.Info("PresetStride:" + std::to_string(CategoryStrideMap[CategoryName]));
                LOG.Error("Current Processing Texcoord stride: " + std::to_string(CategoryBufMap[0].size()) + " can't match with GameType's Texcoord stride: " + std::to_string(CategoryStrideMap[CategoryName]) + " \nPlease try use other GameType to extract.");
            }
            Category_CategoryBufMap_Map[CategoryName] = CategoryBufMap;
            //如果是Blend槽位，则需要准备一个移除Blend中的位于BLENDINDICES和BLENDWEIGHTS之后的00空白的新槽位
            //移除Blend中的位于BLENDINDICES和BLENDWEIGHTS之后的00空白
            if (d3d11GameType.UE4PatchNullInBlend) {
                std::unordered_map<uint64_t, std::vector<std::byte>> BlendBufMap_Clean;
                for (const auto& pair : CategoryBufMap) {
                    std::vector<std::byte> BlendTmpBuf = pair.second;
                    std::vector<std::byte> BlendTmpBufNew;

                    for (int i = 0; i < BlendTmpBuf.size(); i++) {
                        if (i < 4) {
                            BlendTmpBufNew.push_back(BlendTmpBuf[i]);
                        }
                        else if (i > 7 && i < 12) {
                            BlendTmpBufNew.push_back(BlendTmpBuf[i]);
                        }
                    }
                    BlendBufMap_Clean[pair.first] = BlendTmpBufNew;
                }
                Category_CategoryBufMap_Map[CategoryName] = BlendBufMap_Clean;
            }

            //设置CategoryHash
            std::wstring CategoryHash = CategoryExtractFileName.substr(11, 8);
            extractConfig.CategoryHashMap[CategoryName] = MMTString::ToByteString(CategoryHash);
            LOG.Info(MMTString::ToWideString(CategoryName) + L" Category Hash: " + CategoryHash);

            LOG.NewLine();
        }
        else {
            LOG.Warning(L"Can't find .buf file for index: " + VSExtractIndex + L" slot:" + MMTString::ToWideString(CategorySlot));
        }
    }

    std::vector<std::byte> finalVB0Buf;
    for (int i = 0; i < MatchNumber; i++) {
        for (std::string CategoryName : d3d11GameType.OrderedCategoryNameList) {
            finalVB0Buf.insert(finalVB0Buf.end(), Category_CategoryBufMap_Map[CategoryName][i].begin(), Category_CategoryBufMap_Map[CategoryName][i].end());
        }
    }


    FmtFile fmtFileData;
    fmtFileData.ElementNameList = d3d11GameType.OrderedFullElementList;
    fmtFileData.d3d11GameType = d3d11GameType;
    fmtFileData.Format = L"DXGI_FORMAT_R32_UINT";
    fmtFileData.Stride = d3d11GameType.getElementListStride(fmtFileData.ElementNameList);
    LOG.NewLine();


    LOG.Info("Start to go through every IB file:");
    //遍历每一个IB，输出
    int outputCount = 1;
    std::vector<std::string> MatchFirstIndexList;
    std::vector<std::string> PartNameList;
    std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map;
    for (const auto& pair : matchFirstIndexIBFileNameMap) {
        std::wstring IBFileName = pair.second;
        std::wstring Index = IBFileName.substr(0, 6);

        std::wstring IBReadBufferFileName = IBFileName.substr(0, IBFileName.length() - 4) + L".buf";
        std::wstring IBReadBufferFilePath = G.WorkFolder + IBReadBufferFileName;

        std::wstring IBReadFilePath = G.WorkFolder + IBFileName;
        IndexBufferTxtFile ibFileData(IBReadFilePath, true);

        MatchFirstIndexList.push_back(MMTString::ToByteString(ibFileData.FirstIndex));
        PartNameList.push_back(std::to_string(outputCount));
        MatchFirstIndex_PartName_Map[ibFileData.FirstIndex] = std::to_wstring(outputCount);

        LOG.Info(IBFileName);
        LOG.Info(L"MatcheFirstIndex: " + ibFileData.FirstIndex + L"  PartName:" + std::to_wstring(outputCount));
        LOG.Info("MinNumber: " + std::to_string(ibFileData.MinNumber) + "\t\tMaxNumber:" + std::to_string(ibFileData.MaxNumber));


        //分别输出fmt,ib,vb
        std::wstring OutputIBBufFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".ib";
        std::wstring OutputVBBufFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".vb";
        std::wstring OutputFmtFilePath = OutputDrawIBFolder + DrawIB + L"-" + std::to_wstring(outputCount) + L".fmt";

        //输出FMT文件
        fmtFileData.OutputFmtFile(OutputFmtFilePath);
        //输出IB文件
        IndexBufferBufFile ibBufFile(IBReadBufferFilePath, ibFileData.Format);
        ibBufFile.SelfDivide(std::stoi(ibFileData.FirstIndex), std::stoi(ibFileData.IndexCount));
        ibBufFile.SaveToFile_UINT32(OutputIBBufFilePath, -1 * ibBufFile.MinNumber);

        //输出VB文件
        VertexBufferBufFile vbBufFile;
        vbBufFile.FinalVB0Buf = finalVB0Buf;
        vbBufFile.SelfDivide(ibBufFile.MinNumber, ibBufFile.MaxNumber, fmtFileData.Stride);
        vbBufFile.SaveToFile(OutputVBBufFilePath);

        outputCount++;
    }


    //输出Tmp.json
    extractConfig.MatchFirstIndexList = MatchFirstIndexList;
    extractConfig.PartNameList = PartNameList;
    extractConfig.TmpElementList = d3d11GameType.OrderedFullElementList;
    extractConfig.WorkGameType = MMTString::ToByteString(GameType);
    
    AutoDetectTextureFiles ( DrawIB, MatchFirstIndex_PartName_Map, d3d11GameType.GPUPreSkinning);
    for (const auto& pair : this->PartName_TextureSlotReplace_Map) {
        extractConfig.PartName_TextureSlotReplace_Map[pair.first] = pair.second.Save_SlotName_Equal_ResourceFileName_List;
    }
    extractConfig.SaveTmpJsonConfigs(OutputDrawIBFolder);

    MoveAllUsedTexturesToOutputFolder(DrawIB, OutputDrawIBFolder);
    LOG.NewLine();
}


void Functions_KBY::ExtractModel() {

    for (const auto& pair : G.DrawIB_ExtractConfig_Map) {
        std::wstring DrawIB = pair.first;

        FrameAnalysisData FAData = G.GetFrameAnalysisData();
        //LOG.Info(L"DrawIB after before confirm:" + DrawIB);
        //DrawIB = FAData.GetRealIB_IfNoModIB_FromLog(DrawIB);
        //LOG.Info(L"DrawIB after auto confirm:" + DrawIB);

        DrawIBConfig extractConfig = pair.second;
        LOG.NewLine();
        LOG.Info("Extract Buffer from UE4 VertexShader:");
        LOG.NewLine();


        //鸣潮中的虚假IB检测 8d45cfee
        if (DrawIB == L"8d45cfee" && (G.GameName == L"WW" || G.GameName == L"WW1.1" || G.GameName == L"WWMI")) {
            LOG.Error("Can't use 8d45cfee to extract in WuWa, it's a fake IndexBuffer hash shared by all character, please select a unique IndexBuffer Hash value instead.");
        }

        std::wstring MatchedGameType = extractConfig.GameType;
        if (extractConfig.GameType == L"Auto") {
            //进行自动类型识别并重新赋值GameType,这里用排除法通过对比实际stride与预计stride是否相同，来排除每个类型。
            //最终得到所有可能的类型列表，如果类型列表的数量不止一个，则报错并提示所有可能的类型
            //如果只有一个类型，则使用那个类型来进行提取

            std::vector<std::string> MatchedGameTypeList;
            LOG.Info("Start to auto detect game type:");
            for (const auto& pair : GameTypeName_D3d11GameType_Map) {
                std::string GameType = pair.first;
                D3D11GameType d3d11GameType = pair.second;
                LOG.Info("Try Match GameType: " + d3d11GameType.GameType);

                std::vector<std::wstring> FrameAnalyseFileNameList = FAData.FrameAnalysisFileNameList;
                LOG.Info("Read FrameAnalysis file name list success.");

                // 从vb0中读取到顶点数量然后在下面的过程中，UE4的VB0一般固定为POSITION长度固定为2
                // 限制顶点数量匹配到我们的DrawNumber来设置对应的索引
                std::map<int, std::wstring> matchFirstIndexIBFileNameMap;
                std::wstring VSExtractIndex;
                std::wstring PositionExtractFileName = L"";
                int MatchNumber = 0;
                int POSITION_ByteWidth = d3d11GameType.ElementNameD3D11ElementMap["POSITION"].ByteWidth;
                for (std::wstring filename : FrameAnalyseFileNameList) {
                    if (!filename.ends_with(L".txt")) {
                        continue;
                    }
                    if (filename.find(L"-ib=" + DrawIB) == std::wstring::npos) {
                        continue;
                    }
                    IndexBufferTxtFile ibFileData(G.WorkFolder + filename, false);
                    VSExtractIndex = ibFileData.Index;
                    PositionExtractFileName = FAData.FindFrameAnalysisFileNameListWithCondition(VSExtractIndex + L"-vb0=", L".buf")[0];
                    int POSITION_FileSize = MMTFile::GetFileSize(G.WorkFolder + PositionExtractFileName);
                    MatchNumber = POSITION_FileSize / POSITION_ByteWidth;
                    LOG.Info("Match DrawNumber: " + std::to_string(MatchNumber));

                    LOG.Info(filename);
                    if (ibFileData.FirstIndex == L"") {
                        LOG.Info("Can't find FirstIndex attribute in this file, so skip this.");
                        continue;
                    }
                    int matchFirstIndex = std::stoi(ibFileData.FirstIndex);
                    matchFirstIndexIBFileNameMap[matchFirstIndex] = filename;
                }
                for (const auto& pair : matchFirstIndexIBFileNameMap) {
                    LOG.Info("Match First Index: " + std::to_string(pair.first));
                }
                LOG.NewLine();
                LOG.Info(L"VSExtractIndex: " + VSExtractIndex);
                LOG.NewLine();
                //这里批量检测每个槽位的步长是否都能对上
                bool Match = true;
                std::unordered_map<std::string, uint32_t> CategoryStrideMap = d3d11GameType.getCategoryStrideMap(d3d11GameType.OrderedFullElementList);
                for (const auto& pair : d3d11GameType.CategorySlotMap) {
                    std::string CategoryName = pair.first;
                    std::string CategorySlot = pair.second;
                    LOG.Info("CategoryName: " + CategoryName + " CategorySlot: " + CategorySlot);

                    std::vector<std::wstring> CategoryExtractFileNameList = FAData.FindFrameAnalysisFileNameListWithCondition(VSExtractIndex + L"-" + MMTString::ToWideString(CategorySlot), L".buf");
                    //如果标注的Category找不到，那肯定不是这个类型
                    if (CategoryExtractFileNameList.size() == 0) {
                        LOG.Info("Can't match because CategoryExtractFileName get be get: ");
                        Match = false;
                        continue;
                    }
                    std::wstring CategoryExtractFileName = CategoryExtractFileNameList[0];
                    LOG.Info(L"CategoryExtractFileName" + CategoryExtractFileName);
                    int CategoryRealStride = MMTFile::GetFileSize(G.WorkFolder + CategoryExtractFileName) / MatchNumber;
                    LOG.Info(L"CategoryPresetStride: " + std::to_wstring(CategoryStrideMap[CategoryName]));
                    LOG.Info(L"CategoryRealStride: " + std::to_wstring(CategoryRealStride));

                    if (CategoryStrideMap[CategoryName] != CategoryRealStride) {
                        LOG.Info("Can't match category stride: " + CategoryName);
                        if (CategoryName == "Blend") {
                            //当Blend步长不同时，先看看是否为需要补充0的特殊情况
                            if (d3d11GameType.UE4PatchNullInBlend && CategoryRealStride == 16 && CategoryStrideMap["Blend"] == 8) {
                                LOG.Info("Meet special case: UE4PatchNullInBlend, allow match");
                            }
                            else {
                                Match = false;

                                continue;
                            }
                        }
                        else {
                            Match = false;

                            continue;
                        }
                    }

                    //在stride相等的情况下，必须不使用UE4的Blend填充，否则也不能匹配上
                    if (CategoryRealStride == CategoryStrideMap["Blend"] && CategoryName == "Blend") {
                        if (d3d11GameType.UE4PatchNullInBlend) {
                            Match = false;

                            continue;
                        }
                    }

                }

                //如果能执行到这里说明可以添加到匹配到的类型列表中
                if (Match) {
                    MatchedGameTypeList.push_back(GameType);
                }
            }

            if (MatchedGameTypeList.size() == 0) {
                LOG.Error("Can't find any GameType using auto game type detect, try to manually set GameType if you think this exists in our GameType,or contact NicoMico to add this new GameType.");
            }
            else if (MatchedGameTypeList.size() == 1) {
                MatchedGameType = MMTString::ToWideString(MatchedGameTypeList[0]);
                LOG.Info(L"Auto GameType detect success! Current GameType: " + MatchedGameType);
                LOG.NewLine();
            }
            else {
                std::string GameTypeListString = "";
                for (std::string matchedGameType : MatchedGameTypeList) {
                    GameTypeListString = GameTypeListString + matchedGameType + " ";
                }
                LOG.Info("More than one GameType detected: " + GameTypeListString);
                //如果匹配到了不止一个数据类型，那么可能是比如说一个有COLOR一个没有COLOR但是都匹配上了
                //这种情况首先按GPU-PreSkinning过滤如果有GPU-PreSkinning的，优先使用GPU-PreSkinning的
                std::vector<std::string> GPUPreSkinningMatchedGameTypeStrList;
                for (std::string matchedGameType : MatchedGameTypeList) {
                    D3D11GameType matchedTestD3D11GameType = GameTypeName_D3d11GameType_Map[matchedGameType];
                    if (matchedTestD3D11GameType.GPUPreSkinning) {
                        GPUPreSkinningMatchedGameTypeStrList.push_back(matchedGameType);
                    }
                }

                if (GPUPreSkinningMatchedGameTypeStrList.size() == 1) {
                    MatchedGameType = MMTString::ToWideString(GPUPreSkinningMatchedGameTypeStrList[0]);
                }
                else if (GPUPreSkinningMatchedGameTypeStrList.size() > 1) {
                    //如果两个都是GPU-PreSkinning的话，就只能按Category数量取胜了，哪个数量多优先用哪个
                    uint64_t CategoryNumber = 0;
                    for (std::string matchedGameType : MatchedGameTypeList) {
                        D3D11GameType matchedTestD3D11GameType = GameTypeName_D3d11GameType_Map[matchedGameType];
                        if (matchedTestD3D11GameType.CategoryDrawCategoryMap.size() > CategoryNumber) {
                            CategoryNumber = matchedTestD3D11GameType.CategoryDrawCategoryMap.size();
                            MatchedGameType = MMTString::ToWideString(matchedGameType);
                        }
                        else if (matchedTestD3D11GameType.CategoryDrawCategoryMap.size() == CategoryNumber) {
                            LOG.Error("Two GameType's category size can't be equal when both detected,please ask NicoMico to fix this.");
                        }
                        else {
                            continue;
                        }
                    }
                }
                else {
                    LOG.Error("Two object game type can't be detect in same time,please ask NicoMico to fix this.");
                }

            }


        }

        if (MatchedGameType == L"") {
            LOG.Error("Can't Auto find GameType!");
        }
        LOG.NewLine();
        LOG.Info(L"Start to extract using game type: " + MatchedGameType);
        ExtractFromBuffer_VS_UE4(DrawIB, MatchedGameType);

    }
}