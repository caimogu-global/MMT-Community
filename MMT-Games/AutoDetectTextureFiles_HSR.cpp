#include "Functions_HSR.h"

#include "MMTStringUtils.h"
#include "GlobalConfigs.h"
#include "FrameAnalysisData.h"
#include "FrameAnalysisLog.h"
#include "IndexBufferTxtFile.h"
#include <filesystem>
#include <set>
#include "MMTLogUtils.h"
#include "MMTFileUtils.h"


void Functions_HSR::AutoDetectTextureFiles(std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map, bool GPUPreSkinning) {
    LOG.Info(L"开始崩铁自动贴图识别算法：");
    FrameAnalysisLog FALog = G.GetFrameAnalysisLog();
    FrameAnalysisData FAData = G.GetFrameAnalysisData();

    //这个列表用来防止重复匹配
    std::vector<std::wstring> MatchedFirstIndexList;

    std::vector<std::wstring> trianglelistIndexList = FAData.ReadRealTrianglelistIndexListFromLog(OutputIB);

    //由于崩铁前几个渲染会渲染错误的，所以要逆向匹配
    // TODO 但是寒鸦的丝袜质感贴图只在前几个渲染，这样就太难了，感觉最稳的方法还是基于Shader的Hash值进行
    // 能否知道每个相同类型Shader的真实的Hash值？因为现在很多Shader的Hash值不同，但是用到的槽位是相同的。
    // TODO 等后面实在是都识别不动的时候再换回Shader识别吧。
     // 翻转 vector
    std::reverse(trianglelistIndexList.begin(), trianglelistIndexList.end());


    int lastMatchNumber = 0;
    for (std::wstring index : trianglelistIndexList) {
        LOG.Info(L"当前识别Index: " + index);

        //2.判断当前Index是否含有IB文件。
        std::wstring MatchFirstIndex = FAData.GetIBMatchFirstIndexByIndex(G.WorkFolder, index);
        if (MatchFirstIndex == L"") {
            LOG.Info(L"当前Index未找到IB文件，不属于有效Index，自动跳过。");
            LOG.NewLine();
            continue;
        }

        //3.判断当前MatchFirstIndex是否已经处理过。
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
                if (!PixelSlot_TextureType_Map.contains(MMTString::ToByteString(PixelSlot))) {
                    LOG.Info(L"Can't match for slot: " + PixelSlot + L"  " + MMTString::ToWideString(PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)]));
                    allMatch = false;
                    break;
                }
                else {
                    LOG.Info(PixelSlot + L" matched!");
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


            //根据槽位对应的文件大小，过滤
            bool AllSizeMatch = true;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
                std::string AliasName = textureType.PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)];

                if (textureType.AliasName_FileSizeList_Map.contains(AliasName)) {
                    std::vector<uint32_t> StandardSizeList = textureType.AliasName_FileSizeList_Map[AliasName];
                    uint64_t FileSize = MMTFile::GetFileSize(G.WorkFolder + PixelShaderTextureFileName);
                    bool findValidSize = false;

                    for (uint32_t StandardSize: StandardSizeList) {
                        if (StandardSize == FileSize) {
                            findValidSize = true;
                            break;
                        }
                    }

                    if (!findValidSize) {
                        AllSizeMatch = false;
                        LOG.Info(L"当前识别：" + MMTString::ToWideString(AliasName) + L"  原文件名：" + PixelShaderTextureFileName);
                        LOG.Info(L"  当前文件大小：" + std::to_wstring(FileSize));
                        LOG.Info(MMTString::ToWideString(AliasName) + L" 贴图大小无法匹配。");
                    }
                    
                }
            }
            if (!AllSizeMatch) {
                LOG.Warning(L"贴图文件大小匹配无法满足要求，跳过此项");
                LOG.NewLine();
                continue;
            }


            //凡是有意义的贴图的Hash值不能有重复
            std::unordered_map<std::wstring, std::wstring> HashValue_PixelSlot_Map;
            bool RepeatHash = false;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
                std::wstring HashValue = MMTString::GetFileHashFromFileName(PixelShaderTextureFileName);
                std::string AliasName = textureType.PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)];
                if (AliasName != "NicoMico") {
                    if (HashValue_PixelSlot_Map.contains(HashValue)) {
                        LOG.Info(L"PixelSlot:" + PixelSlot + L" HashValue:" + HashValue);
                        LOG.Info(L"Repeat Hash: " + HashValue);
                        RepeatHash = true;
                        break;
                    }
                    else {
                        HashValue_PixelSlot_Map[HashValue] = PixelSlot;
                    }
                }
                
            }
            if (RepeatHash) {
                LOG.Warning(L"同一张有效贴图出现在多个槽位上，非Draw Index，跳过此项");
                LOG.NewLine();
                continue;
            }


            //TODO 这里凡是dds的并且为GPU-PreSkinning的，都要识别贴图
            //先从deduped找到当前槽位对应的图片并判断是否真的为DiffuseMap
            bool findDiffuseMap = false;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                if (PixelShaderTextureFileName.find(MMTString::ToWideString(textureType.GetDiffuseMapSlot())) != std::wstring::npos) {
                    std::wstring textureFileName = FAData.FindDedupedTextureName(G.WorkFolder, PixelShaderTextureFileName);
                    //这里每个游戏都有准确的格式，如果格式不对则无法匹配
                    //这里使用find而不是ends_with更加宽松，毕竟特别需要SRGB的话可以强行指定
                    //如果不是特别需要或者模糊一点的话，使用默认的BC7_UNORM也能凑合，所以find更通用兼容性更好。
                    if (textureFileName.find(L"BC7_UNORM") != std::wstring::npos) {
                        findDiffuseMap = true;
                        break;
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
                LOG.Info(L"崩铁仅在第一次Draw时使用全面的贴图，不允许重复识别，跳过此项");
                continue;
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
                    else {
                        if (PixelSlot == slotFileNamePair.first && slotFileNamePair.second.ends_with(L".dds") && outputTextureName.ends_with(L".jpg")) {
                            LOG.Warning(L"已经存在dds格式贴图，又遇到了对应贴图的jpg格式，不允许添加jpg格式！");
                            findExists = true;
                        }
                    }
                }

                //没出现过才往里加
                if (!findExists) {
                    slotReplace.In_SlotName_ResourceFileName_Map[PixelSlot] = outputTextureName;
                    slotReplace.In_SlotName_ResourceName_Map[PixelSlot] = L"Resource_" + outputTextureName;
                    slotReplace.SetSerializedList();
                    PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)] = slotReplace;

                    LOG.Info(L"Try Copy Original File: " + G.WorkFolder + PixelShaderTextureFileName);
                    if (!std::filesystem::exists(G.WorkFolder + PixelShaderTextureFileName)) {
                        LOG.Warning(L"Can't find original file: " + G.WorkFolder + PixelShaderTextureFileName);
                    }
                    std::filesystem::copy_file(G.WorkFolder + PixelShaderTextureFileName, G.OutputFolder + OutputIB + L"\\" + outputTextureName, std::filesystem::copy_options::overwrite_existing);
                    LOG.Info(L"Copy To: " + outputTextureName);
                }

                
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

}



