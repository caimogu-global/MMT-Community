#include "Functions_GI.h"

#include "MMTLogUtils.h"
#include "MMTStringUtils.h"
#include "GlobalConfigs.h"
#include "FrameAnalysisData.h"
#include "FrameAnalysisLog.h"
#include "IndexBufferTxtFile.h"
#include <filesystem>
#include <set>
#include "MMTFileUtils.h"


void Functions_GI::AutoDetectTextureFiles(std::wstring OutputIB, std::unordered_map<std::wstring, std::wstring> MatchFirstIndex_PartName_Map,bool GPUPreSkinning) {
	LOG.Info(L"开始GI贴图自动类型识别:");
    FrameAnalysisLog FALog = G.GetFrameAnalysisLog();
    FrameAnalysisData FAData = G.GetFrameAnalysisData();

    //这个列表用来防止重复匹配
    std::vector<std::wstring> MatchedFirstIndexList;

    //这里为了兼容逆向提取，必须使用从Log里获取的真实完整Index列表
    std::vector<std::wstring> trianglelistIndexList = FAData.ReadRealTrianglelistIndexListFromLog(OutputIB);

    //因为原神刚开始Draw的内容肯定会有问题，低于000020的Index可能都是影子的Shader所以可能没有NormalMap
    //所以这里我们做特殊处理，先把000020以上的放前面匹配，以下的放后面匹配
    std::vector<std::wstring> ReorderedTrianglelistIndexList;
    for (std::wstring Index: trianglelistIndexList) {
        if (std::stoi(Index) >= 20) {
            ReorderedTrianglelistIndexList.push_back(Index);
        }
    }
    for (std::wstring Index : trianglelistIndexList) {
        if (std::stoi(Index) < 20) {
            ReorderedTrianglelistIndexList.push_back(Index);
        }
    }

    //来一个记录上一次匹配成功的文件数量的map
    std::unordered_map<std::wstring,int> PartName_LastMatchFileNumber_Map;


    int lastMatchNumber = 0;
    for (std::wstring index : ReorderedTrianglelistIndexList) {
        LOG.Info(L"当前识别Index: " + index);

        //2.判断当前Index是否含有IB文件。
        std::wstring MatchFirstIndex = FAData.GetIBMatchFirstIndexByIndex(G.WorkFolder, index);
        if (MatchFirstIndex == L"") {
            LOG.Info(L"当前Index未找到IB文件，不属于有效Index，自动跳过。");
            LOG.NewLine();
            continue;
        }

        LOG.Info(L"当前对应PartName: " + MatchFirstIndex_PartName_Map[MatchFirstIndex]);
        

        //3.判断当前MatchFirstIndex是否已经处理过。
        bool findExistsFirstIndex = false;
        for (std::wstring match_firstIndex : MatchedFirstIndexList) {
            if (MatchFirstIndex == match_firstIndex) {
                findExistsFirstIndex = true;
            }
        }

        //遍历每个贴图名称并把ps-t*放入一个Set，这里用Set是因为存在同名的dds和jpg造成同一个槽位多次匹配的情况
         //根据是否识别jpg类型来进行填充
        std::vector<std::wstring> pixelShaderTextureAllFileNameList = MMTFile::FindTextureFileList(G.WorkFolder, index + L"-ps-t");
        std::set<std::wstring> PixelSlotSet;
        for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
            std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
            
            if (std::stoi(PixelSlot.substr(4)) > 20 ) {
                LOG.Warning(L"当前Slot值大于最大有效Slot值20，跳过此项: " + PixelSlot);
            }
            else {
                PixelSlotSet.insert(PixelSlot);
                LOG.Info(L"Detect Slot: " + PixelSlot);
            }
        }

        //遍历所有的TextureType
        bool findMatched = false;
        for (TextureType textureType : this->TextureTypeList) {
            //根据当前传入类型是否为GPUPreSkinning来进行过滤
            if (textureType.GPUPreSkinning != GPUPreSkinning) {
                LOG.Info(L"当前识别的数据类型的GPUPreSkinning设定与当前贴图类型设定不符，跳过识别.");
                LOG.NewLine();
                continue;
            }

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

            //TODO 这里要升级一下，最好是有个什么DDS格式读取工具能够直接读取并判断各个通道中的内容。
            //以此来更准确的理解是不是Diffuse贴图？


            //最后判断Diffuse槽位的贴图的格式在Deduped里是不是该贴图类型指定的格式
            //进行槽位判断，首先寻找DiffuseMap.dds
            //寻找diffuse slot
            std::wstring diffuse_slot = L"";
            for (const auto& slotPair : textureType.PixelSlot_TextureType_Map) {
                std::string slot = slotPair.first;
                std::string textureFileName = slotPair.second;
                if (textureFileName == "DiffuseMap.dds") {
                    diffuse_slot = MMTString::ToWideString(slot);
                }
            }
            //先从deduped找到当前槽位对应的图片并判断是否真的为DiffuseMap
            //所以每个游戏都需要有一个预设
            bool findDiffuseMap = false;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                if (PixelShaderTextureFileName.find(diffuse_slot) != std::wstring::npos) {
                    std::wstring textureFileName = FAData.FindDedupedTextureName(G.WorkFolder, PixelShaderTextureFileName);
                    if (textureFileName == L"") {
                        LOG.Info(L"由于无法在Deduped文件中找到该贴图的deduped文件名，尝试从Dump log中读取");
                        textureFileName = FALog.FindRealDedupedResourceFileNameFromDumpCallLog(index, PixelShaderTextureFileName);
                        LOG.Info(L"从Dump Log中读取到的deduped文件名为：" + textureFileName);
                    }

                    //这里每个游戏都有准确的格式，如果格式不对则无法匹配
                    //这里使用find而不是ends_with更加宽松，毕竟特别需要SRGB的话可以强行指定
                    //如果不是特别需要或者模糊一点的话，使用默认的BC7_UNORM也能凑合，所以find更通用兼容性更好。
                    if (textureFileName.find(L"BC7_UNORM_SRGB") != std::wstring::npos) {
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
                    
                }
            }
            lastMatchNumber = (int)pixelShaderTextureAllFileNameList.size();

            
            //原神独有的贴图大小检测，实在没办法了，只能一点点过滤啦!
            //只在使用GPU-PreSkinning时进行
            int TextureStandardSize = 1048724;
            if (!GPUPreSkinning) {
                TextureStandardSize = 262292;
            }
            bool AllTextureSizeValid = true;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
                std::wstring TextureAliasName = MMTString::ToWideString(textureType.PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)]);

                //1048724大小判断，原神的Dump下来的贴图都得至少是这么大的才行。
                int filesize = MMTFile::GetFileSize(G.WorkFolder + PixelShaderTextureFileName);
                if (TextureAliasName == L"DiffuseMap.dds") {
                    if (filesize < TextureStandardSize) {
                        AllTextureSizeValid = false;
                    }
                }
                else if (TextureAliasName == L"NormalMap.dds") {
                    if (filesize < TextureStandardSize) {
                        AllTextureSizeValid = false;
                    }
                }
                else if (TextureAliasName == L"LightMap.dds") {
                    if (filesize < TextureStandardSize) {
                        AllTextureSizeValid = false;
                    }
                }
                else if (TextureAliasName == L"HighLightMap.dds") {
                    if (filesize < TextureStandardSize) {
                        AllTextureSizeValid = false;
                    }
                }

            }

            if (!AllTextureSizeValid) {
                LOG.Info(L"并非所有贴图的大小都满足识别要求，跳过此项");
                LOG.NewLine();
                continue;
            }

            //能执行到这里说明所有的前置条件都匹配成功了
            findMatched = true;
            LOG.NewLine();


            //因为这里存在重复匹配，第二次覆盖第一次的情况，所以这里要判断
            //只有第二次得到的替换文件数量大于第一次得到的才能覆盖

            int CurrentValidFileNumber = 0;
            for (std::wstring PixelShaderTextureFileName : pixelShaderTextureAllFileNameList) {
                std::wstring PixelSlot = MMTString::GetPixelSlotFromTextureFileName(PixelShaderTextureFileName);
                std::wstring TextureAliasName = MMTString::ToWideString(textureType.PixelSlot_TextureType_Map[MMTString::ToByteString(PixelSlot)]);
                if (TextureAliasName == L"NicoMico") {
                    continue;
                }
                else {
                    CurrentValidFileNumber++;
                }
            }

            if (PartName_LastMatchFileNumber_Map.contains(PartName)) {
                
                int LastValidFileNumber = PartName_LastMatchFileNumber_Map[PartName];
                LOG.Info(L"当前覆盖的文件数量 " + std::to_wstring(CurrentValidFileNumber)
                    + L"上一次识别成功产生的文件数量:"
                    + std::to_wstring(LastValidFileNumber)
                );

                if (CurrentValidFileNumber < LastValidFileNumber) {
                    LOG.Info(L"为避免重复情况对当前覆盖的文件数量 " + std::to_wstring(CurrentValidFileNumber)
                        + L"进行判断，发现小于上一次识别成功产生的文件数量:"
                        + std::to_wstring(LastValidFileNumber)
                        + L" 所以跳过此错误项"
                    );
                    LOG.NewLine();
                    continue;
                }
            }
            else {
                PartName_LastMatchFileNumber_Map[PartName] = CurrentValidFileNumber;
            }
            
            //在重新赋值之前需要把旧的清空防止影响发挥
            PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)] = {};
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
                TextureSlotReplace slotReplace = this->PartName_TextureSlotReplace_Map[MMTString::ToByteString(PartName)];
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
                    LOG.Info(L"添加：" + PixelSlot + L" = " + outputTextureName);
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