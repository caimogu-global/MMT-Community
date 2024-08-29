#include "Functions_WW1.1.h"

#include "MMTStringUtils.h"
#include "GlobalConfigs.h"
#include "FrameAnalysisData.h"
#include "FrameAnalysisLog.h"
#include "IndexBufferTxtFile.h"
#include <filesystem>
#include <set>
#include "MMTLogUtils.h"
#include "MMTFileUtils.h"


void Functions_WW11::AutoDetectTextureFiles(std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map, bool GPUPreSkinning) {
    LOG.Info(L"开始鸣潮自动贴图识别：");
    FrameAnalysisLog FALog = G.GetFrameAnalysisLog();
    FrameAnalysisData FAData = G.GetFrameAnalysisData();

    //鸣潮必须读取所有的并进行贴图读取，不能偷懒
    std::vector<std::wstring> trianglelistIndexList = FAData.ReadTrianglelistIndexList(OutputIB);

    //鸣潮中真实的调用贴图会直接调用PSSetShaderResources来调用对应的槽位，如果一次DrawCall中没有这个，说明不能用于贴图识别。
    std::vector<std::wstring> TrianglelistIndexList_Filter_PSSetShaderResources;
    for (std::wstring Index : trianglelistIndexList) {
        int Call_PSSetShaderResources_Time = FALog.GetIndexDrawCallPsSetShaderResourcesTime(Index);
        if (Call_PSSetShaderResources_Time != 0) {
            LOG.Info(L"Index With Call PSSetShaderResources: " + Index);
            TrianglelistIndexList_Filter_PSSetShaderResources.push_back(Index);
        }
    }
    LOG.NewLine();

    //这个列表用来防止重复匹配
    std::vector<std::wstring> MatchedFirstIndexList;

    //记录上一次匹配到的有效文件数量，如果这次比上一次有效文件数多才允许覆盖
    int lastMatchNumber = 0;
    
    //对于每个Index都需要进行处理。
    for (std::wstring index : TrianglelistIndexList_Filter_PSSetShaderResources) {
        LOG.Info(L"当前识别Index: " + index);

        //获取MatchFirstIndex
        std::wstring MatchFirstIndex = FAData.GetIBMatchFirstIndexByIndex(G.WorkFolder, index);
        if (MatchFirstIndex == L"") {
            LOG.Info(L"当前Index未找到IB文件，不属于有效Index，自动跳过。");
            LOG.NewLine();
            continue;
        }

        //判断当前MatchFirstIndex是否已经处理过。
        bool findExistsFirstIndex = false;
        for (std::wstring match_firstIndex : MatchedFirstIndexList) {
            if (MatchFirstIndex == match_firstIndex) {
                findExistsFirstIndex = true;
            }
        }
        int PartNameCount = std::stoi(MatchFirstIndex_PartName_Map[MatchFirstIndex]);
        LOG.Info(L"当前Match_First_index: " + MatchFirstIndex);
        LOG.Info(L"当前PartName: " + std::to_wstring(PartNameCount));

        std::vector<std::wstring> pixelShaderTextureAllFileNameList = MMTFile::FindTextureFileList(G.WorkFolder, index + L"-ps-t");
        
        std::set<std::wstring> PixelSlotSet;
        for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
            std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
            PixelSlotSet.insert(PixelSlot);
            LOG.Info(L"Detect Slot: " + PixelSlot);
        }

        //遍历所有的TextureType
        bool findMatched = false;
        for (TextureType textureType : this->TextureTypeList) {
            //遍历每个贴图名称并把ps-t*放入一个Set，这里用Set是因为存在同名的dds和jpg造成同一个槽位多次匹配的情况
            std::unordered_map<std::string, std::string> PixelSlot_TextureType_Map = textureType.PixelSlot_TextureType_Map;

            //如果槽位数量不相等直接退出即可
            if (PixelSlotSet.size() != PixelSlot_TextureType_Map.size()) {
                LOG.Info("All slot matched but total slot number not equal, skip this.");
                LOG.NewLine();
                continue;
            }
            else {
                //如果相等的话则输出当前的槽位信息，方便DEBUG
                std::wstring slotPairStr;
                for (const auto& slotPair : PixelSlot_TextureType_Map) {
                    slotPairStr = slotPairStr + MMTString::ToWideString(slotPair.first) + L" ";
                }
                LOG.Info(L"Current Slots:" + slotPairStr);
            }

            //并且还需要判断当前的Slot是否在TextureType的预设中出现过，只要有一个没出现都不合格。
            bool allMatch = true;
            for (std::wstring PixelSlot : PixelSlotSet) {
                std::wstring AliasFileName = MMTString::ToWideString(PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)]);
                if (!PixelSlot_TextureType_Map.contains(MMTString::ToByteString(PixelSlot)) ) {
                    LOG.Info(L"Can't match for slot: " + PixelSlot + L"  " + AliasFileName);
                    allMatch = false;
                    break;
                }
                else {
                    LOG.Info(PixelSlot + L" matched! " + AliasFileName);
                }
            }

            if (!allMatch) {
                LOG.Info("Can't match all slot, skip this.");
                LOG.NewLine();
                continue;

            }
            else {
                LOG.Info("All Matched!");
            }


            //鸣潮中DrawCall调用PsSetShaderResources的次数必须大于等于我们实际上有意义的dds结尾的槽位数量
            uint32_t Call_PSSetShaderResources_Time = FALog.GetIndexDrawCallPsSetShaderResourcesTime(index);
            if (Call_PSSetShaderResources_Time < textureType.GetMeaningfulDdsFileCount()) {
                LOG.Info(L"当前DrawCall调用PSSetShaderResources的数量小于此贴图类型的有效贴图数量，跳过此项。");
                LOG.NewLine();
                continue;
            }

            //最后判断Diffuse槽位的贴图的格式在Deduped里是不是该贴图类型指定的格式
            bool findDiffuseMap = false;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                if (PixelShaderTextureFileName.find(MMTString::ToWideString(textureType.GetDiffuseMapSlot())) != std::wstring::npos) {
                    std::wstring textureFileName = FAData.FindDedupedTextureName(G.WorkFolder, PixelShaderTextureFileName);
                    //这里每个游戏都有准确的格式，如果格式不对则无法匹配
                    //这里使用find而不是ends_with更加宽松，毕竟特别需要SRGB的话可以强行指定
                    //如果不是特别需要或者模糊一点的话，使用默认的BC7_UNORM也能凑合，所以find更通用兼容性更好。
                    if (textureFileName.find(L"BC7_UNORM_SRGB") != std::wstring::npos) {
                        int DiffuseSize = MMTFile::GetFileSize(G.WorkFolder + PixelShaderTextureFileName);
                        //鸣潮需要额外判断DiffuseMap的大小：
                        if (DiffuseSize >= 4194452) {
                            findDiffuseMap = true;
                            LOG.Info(L"Diffuse Size: 4194452");
                            break;
                        }
                        else {
                            if (DiffuseSize == 262292 && PartNameCount >= 6) {
                                //262 292
                                findDiffuseMap = true;
                                LOG.Info(L"Diffuse Size: 262292  PartName >= 6");
                                break;
                                //这个会导致贴图篡位，从ShaderUsage里读取width和height为512，512也无法准确识别
                                //只能暂时判断当PartName >= 6时才能允许这样匹配，因为鸣潮里的这个是固定的
                            }
                            else {
                                LOG.Warning(L"既不大于等于4,194,452也不是缩小版本贴图");
                            }

                        }

                    }
                }
                
            }
            if (!findDiffuseMap) {
                LOG.Warning(L"由于DiffuseMap槽位未找到BC7_UNORM_SRGB格式dds贴图文件，此贴图类型不正确，自动跳过。");
                LOG.NewLine();
                continue;
            }


            //能执行到这里说明总槽位和Diffuse的格式都匹配成功了
            findMatched = true;
            LOG.NewLine();


            LOG.Info(L"总槽位数量：" + std::to_wstring(pixelShaderTextureAllFileNameList.size()) + L" 之前生成的贴图替换槽位数量：" + std::to_wstring(lastMatchNumber));
            std::wstring PartName = MatchFirstIndex_PartName_Map[MatchFirstIndex];
            if (findExistsFirstIndex) {
                //GI存在特殊情况，先Draw一次只调用DiffuseMap和LightMap。
                //后面的Draw才调用带有NormalMap的，所以识别时先识别槽位少的会导致错误识别。

                //判断识别到的贴图数量如果大于之前已有的数量则允许进行替换，否则直接continue
                if (pixelShaderTextureAllFileNameList.size() < lastMatchNumber) {
                    LOG.Info(L"当前match_first_index已识别到贴图类型，且当前识别的贴图类型槽位数量不如之前识别到的多，所以跳过此索引");
                    continue;
                }
                else {
                    LOG.Info(L"允许覆盖操作");
                    PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)] = {};
                }
            }
            lastMatchNumber = (int)pixelShaderTextureAllFileNameList.size();


            //接下来就是使用当前的TextureType里的信息为每个槽位的贴图移动并更改名称了。
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
                std::wstring TextureAliasName = MMTString::ToWideString(textureType.PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)]);
                if (TextureAliasName == L"NicoMico") {
                    continue;
                }

                std::wstring Hash = MMTString::GetFileHashFromFileName(PixelShaderTextureFileName);
                //拼接要输出的文件名，如果源文件是.jpg则后缀需要改为.jpg
                std::wstring outputTextureName = OutputIB + L"-" + Hash + L"-" + PartName + L"-" + TextureAliasName;
                if (PixelShaderTextureFileName.ends_with(L".jpg")) {
                    outputTextureName = outputTextureName.substr(0, outputTextureName.length() - 4) + L".jpg";
                }

                //获取当前已产生的替换列表
                TextureSlotReplace slotReplace = PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)];
                bool findExists = false;

                for (const auto& slotFileNamePair : slotReplace.In_SlotName_ResourceFileName_Map) {
                    if (outputTextureName == slotFileNamePair.second) {
                        findExists = true;
                    }
                }

                //没出现过才往里加
                if (!findExists) {
                    slotReplace.In_SlotName_ResourceFileName_Map[PixelSlot] = outputTextureName;
                    slotReplace.In_SlotName_ResourceName_Map[PixelSlot] = L"Resource_" + outputTextureName;
                    slotReplace.SetSerializedList();
                    PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)] = slotReplace;
                }

                LOG.Info(L"Try Copy Original File: " + G.WorkFolder + PixelShaderTextureFileName);
                if (!std::filesystem::exists(G.WorkFolder + PixelShaderTextureFileName)) {
                    LOG.Warning(L"Can't find original file: " + G.WorkFolder + PixelShaderTextureFileName);
                }
                std::filesystem::copy_file(G.WorkFolder + PixelShaderTextureFileName, G.OutputFolder + OutputIB + L"\\" + outputTextureName, std::filesystem::copy_options::overwrite_existing);
                LOG.Info(L"Copy To: " + outputTextureName);
            }

            //每个Index只能有一个TextureType满足并识别成功，所以必须break
            LOG.Info(L"识别成功");
            MatchedFirstIndexList.push_back(MatchFirstIndex);
            LOG.NewLine();
            break;
        }
        LOG.NewLine();
        LOG.NewLine();

        //如果含有1个MatchFirstIndex的话则直接break，如果有多个的话则允许多次匹配
        if (MatchFirstIndex_PartName_Map.size() <= 1 && findMatched) {
            break;
        }

    }

    LOG.NewSeperator();
}

