#include "Functions_HI3.h"


#include "GlobalConfigs.h"
#include "D3d11GameType.h"
#include "VertexBufferBufFile.h"
#include "MMTConstants.h"

#include "BufferUtils.h"
#include "IniBuilder.h"


void Functions_HI3::GenerateMod() {
    LOG.Info("Executing: GenerateMod_HI3");

    IniBuilder HI3IniBuilder;

    for (const auto& pair : G.DrawIB_ExtractConfig_Map) {
        std::wstring drawIB = pair.first;
        DrawIBConfig drawIBConfig = pair.second;
        drawIBConfig.GameType = MMTString::ToWideString(drawIBConfig.WorkGameType);
        D3D11GameType d3d11GameType = GameTypeName_D3d11GameType_Map[drawIBConfig.WorkGameType];
      
        if (!drawIBConfig.Initialize(G.OutputFolder, d3d11GameType)) {
            continue;
        }
        LOG.Info("D3d11GameType: " + d3d11GameType.GameType);


        //读取绘制的顶点数量
        drawIBConfig.DrawNumber = BufferUtils::GetSum_DrawNumber_FromVBFiles(drawIBConfig);

        //读取VB文件中的数据
        drawIBConfig.FinalVBCategoryDataMap = BufferUtils::Read_FinalVBCategoryDataMap(drawIBConfig);

        //数值处理
        if (drawIBConfig.TangentAlgorithm == L"average_normal") {
            BufferUtils::Unity_TANGENT_AverageNormal(drawIBConfig.FinalVBCategoryDataMap, d3d11GameType);
        }
        if (drawIBConfig.ColorAlgorithm == L"average_normal") {
            BufferUtils::Unity_COLOR_AverageNormal(drawIBConfig.FinalVBCategoryDataMap, drawIBConfig, d3d11GameType);
        }
        BufferUtils::Unity_Reverse_NORMAL_TANGENT(drawIBConfig.FinalVBCategoryDataMap, drawIBConfig, d3d11GameType);
        BufferUtils::Unity_Reset_COLOR(drawIBConfig.FinalVBCategoryDataMap, drawIBConfig, d3d11GameType);

        BufferUtils::Read_Convert_Output_IBBufferFiles(drawIBConfig);
        BufferUtils::OutputCategoryBufferFiles(drawIBConfig);
        if (G.ShareSourceModel) {
            BufferUtils::ShareSourceModel(drawIBConfig);
        }

        //（3）调用生成Wheel格式的ini文件
        //------------------------------------------------------------------------------------------------------------------
        //拼接要输出的贴图的资源名供后续使用 ]
        if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
            this->PartName_TextureSlotReplace_Map.clear();
            for (const auto& pair : drawIBConfig.PartName_TextureSlotReplace_Map) {
                TextureSlotReplace slotReplace(pair.second);
                this->PartName_TextureSlotReplace_Map[pair.first] = slotReplace;
            }
        }
        

        LOG.Info(L"Start to output ini file.");


        //1.TextureOverride VB部分，只有使用GPU-PreSkinning时是直接替换hash对应槽位
        if (d3d11GameType.GPUPreSkinning) {
            LOG.Info(L"Start to output TextureOverrideVB section");
            IniSection TextureOverrideVBSection(IniSectionType::TextureOverrideVB);
            for (std::string categoryName : d3d11GameType.OrderedCategoryNameList) {
                std::string categoryHash = drawIBConfig.CategoryHashMap[categoryName];
                std::string categorySlot = d3d11GameType.CategorySlotMap[categoryName];
                LOG.Info(L"categoryName: " + MMTString::ToWideString(categoryName) + L" categorySlot: " + MMTString::ToWideString(categorySlot));
                TextureOverrideVBSection.Append(L"[TextureOverride" + drawIBConfig.DrawIB + MMTString::ToWideString(categoryName) + L"]");
                TextureOverrideVBSection.Append(L"hash = " + MMTString::ToWideString(categoryHash));

                //遍历获取所有在当前分类hash下进行替换的分类，并添加对应的资源替换
                for (const auto& pair : d3d11GameType.CategoryDrawCategoryMap) {
                    std::string originalCategoryName = pair.first;
                    std::string drawCategoryName = pair.second;
                    if (categoryName == drawCategoryName) {
                        std::string categoryOriginalSlot = d3d11GameType.CategorySlotMap[originalCategoryName];
                        TextureOverrideVBSection.Append(MMTString::ToWideString(categoryOriginalSlot) + L" = " + L"Resource" + drawIBConfig.DrawIB + MMTString::ToWideString(originalCategoryName));
                    }
                }

                //draw一般都是在Blend槽位上进行的，所以我们这里要判断确定是Blend要替换的hash才能进行draw。
                if (categoryName == d3d11GameType.CategoryDrawCategoryMap["Blend"]) {
                    TextureOverrideVBSection.Append(L"handling = skip");
                    TextureOverrideVBSection.Append(L"draw = " + std::to_wstring(drawIBConfig.DrawNumber) + L", 0");
                }
                TextureOverrideVBSection.NewLine();
            }

            //2.VertexLimitRaise部分，这里只有使用到GPU-PreSkinning技术时才需要突破顶点数量限制
            std::string VertexLimitVB = drawIBConfig.VertexLimitVB;
            //步长，drawNumber，实现动态步长
            TextureOverrideVBSection.Append(L"[TextureOverride" + drawIBConfig.DrawIB + L"_" + std::to_wstring(d3d11GameType.CategoryStrideMap["Position"]) + L"_" + std::to_wstring(drawIBConfig.DrawNumber) + L"_VertexLimitRaise]");
            TextureOverrideVBSection.Append(L"hash = " + MMTString::ToWideString(VertexLimitVB));
            TextureOverrideVBSection.NewLine();

            HI3IniBuilder.AppendSection(TextureOverrideVBSection);
        }

        //1.IB SKIP部分
        IniSection IBSkipSection(IniSectionType::IBSkip);
        IBSkipSection.Append(L"[TextureOverride" + drawIBConfig.DrawIB + L"IB]");
        IBSkipSection.Append(L"hash = " + drawIBConfig.DrawIB);
        IBSkipSection.Append(L"handling = skip");
        IBSkipSection.NewLine();
        HI3IniBuilder.AppendSection(IBSkipSection);

        //2.IBOverride部分

        IniSection TextureOverrideIBSection(IniSectionType::TextureOverrideIB);
        for (int i = 0; i < drawIBConfig.PartNameList.size(); ++i) {
            std::string IBFirstIndex = drawIBConfig.MatchFirstIndexList[i];
            std::string partName = drawIBConfig.PartNameList[i];

            std::wstring IBResourceName = L"Resource" + drawIBConfig.DrawIB + G.GIMIPartNameAliasMap[MMTString::ToWideString(partName)];

            //声明贴图槽位资源用于备份，防止贴图槽位在Shader里乱窜
            if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
                for (std::string textureResourceReplace : drawIBConfig.PartName_TextureSlotReplace_Map[partName]) {
                    std::vector<std::wstring> splitsStrList = MMTString::SplitString(MMTString::ToWideString(textureResourceReplace), L"=");
                    std::wstring ReplaceSlot = boost::algorithm::trim_copy(splitsStrList[0]);
                    std::wstring ReplaceFileName = boost::algorithm::trim_copy(splitsStrList[1]);
                    std::wstring TextureResourceBackupName = L"Resource_Bak_" + ReplaceSlot + std::to_wstring(i);
                    TextureOverrideIBSection.Append(L"[" + TextureResourceBackupName + L"]");
                }
            }

            TextureOverrideIBSection.Append(L"[TextureOverride" + drawIBConfig.DrawIB + G.GIMIPartNameAliasMap[MMTString::ToWideString(partName)] + L"]");
            TextureOverrideIBSection.Append(L"hash = " + drawIBConfig.DrawIB);
            TextureOverrideIBSection.Append(L"match_first_index = " + MMTString::ToWideString(IBFirstIndex));

            //备份贴图槽位
            if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
                for (std::string textureResourceReplace : drawIBConfig.PartName_TextureSlotReplace_Map[partName]) {
                    std::vector<std::wstring> splitsStrList = MMTString::SplitString(MMTString::ToWideString(textureResourceReplace), L"=");
                    std::wstring ReplaceSlot = boost::algorithm::trim_copy(splitsStrList[0]);
                    std::wstring ReplaceFileName = boost::algorithm::trim_copy(splitsStrList[1]);
                    std::wstring TextureResourceBackupName = L"Resource_Bak_" + ReplaceSlot + std::to_wstring(i);
                    TextureOverrideIBSection.Append(TextureResourceBackupName + L" = ref " + ReplaceSlot);
                }
            }
            

            //替换IB槽位
            TextureOverrideIBSection.Append(L"ib = " + IBResourceName);

            //贴图资源替换
            std::vector<std::wstring> slotReplaceStrList = this->PartName_TextureSlotReplace_Map[partName].Out_SlotReplaceStrList;
            if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
                for (std::wstring slotReplaceStr : slotReplaceStrList) {
                    TextureOverrideIBSection.Append(slotReplaceStr);
                }
            }
           

            //如果不使用GPU-Skinning即为Object类型，此时需要在ib下面替换对应槽位
            if (!d3d11GameType.GPUPreSkinning) {
                for (std::string categoryName : d3d11GameType.OrderedCategoryNameList) {
                    std::string categoryHash = drawIBConfig.CategoryHashMap[categoryName];
                    std::string categorySlot = d3d11GameType.CategorySlotMap[categoryName];
                    LOG.Info(L"categoryName: " + MMTString::ToWideString(categoryName) + L" categorySlot: " + MMTString::ToWideString(categorySlot));

                    //遍历获取所有在当前分类hash下进行替换的分类，并添加对应的资源替换
                    for (const auto& pair : d3d11GameType.CategoryDrawCategoryMap) {
                        std::string originalCategoryName = pair.first;
                        std::string drawCategoryName = pair.second;
                        if (categoryName == drawCategoryName) {
                            std::string categoryOriginalSlot = d3d11GameType.CategorySlotMap[originalCategoryName];
                            TextureOverrideIBSection.Append(MMTString::ToWideString(categoryOriginalSlot) + L" = " + L"Resource" + drawIBConfig.DrawIB + MMTString::ToWideString(originalCategoryName));
                        }
                    }
                }
            }

            //Draw图形
            TextureOverrideIBSection.Append(L"drawindexed = auto");


            //恢复贴图槽位
            if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
                for (std::string textureResourceReplace : drawIBConfig.PartName_TextureSlotReplace_Map[partName]) {
                    std::vector<std::wstring> splitsStrList = MMTString::SplitString(MMTString::ToWideString(textureResourceReplace), L"=");
                    std::wstring ReplaceSlot = boost::algorithm::trim_copy(splitsStrList[0]);
                    std::wstring TextureResourceBackupName = L"Resource_Bak_" + ReplaceSlot + std::to_wstring(i);
                    TextureOverrideIBSection.Append(ReplaceSlot + L" = " + TextureResourceBackupName);
                }
            }
            

            TextureOverrideIBSection.NewLine();

        }
        HI3IniBuilder.AppendSection(TextureOverrideIBSection);

       
        //5.写出VBResource部分
        IniSection ResourceVBSection(IniSectionType::ResourceVB);
        for (std::string categoryName : d3d11GameType.OrderedCategoryNameList) {
            ResourceVBSection.Append(L"[Resource" + drawIBConfig.DrawIB + MMTString::ToWideString(categoryName) + L"]");
            ResourceVBSection.Append(L"type = Buffer");

            if (categoryName == "Blend" && d3d11GameType.PatchBLENDWEIGHTS) {
                int finalBlendStride = d3d11GameType.CategoryStrideMap[categoryName] - d3d11GameType.ElementNameD3D11ElementMap["BLENDWEIGHT"].ByteWidth;
                ResourceVBSection.Append(L"stride = " + std::to_wstring(finalBlendStride));

            }
            else {
                ResourceVBSection.Append(L"stride = " + std::to_wstring(d3d11GameType.CategoryStrideMap[categoryName]));
            }
            //vb文件的文件名
            ResourceVBSection.Append(L"filename = Buffer/" + drawIBConfig.DrawIB + MMTString::ToWideString(categoryName) + L".buf");
            ResourceVBSection.NewLine();
        }
        HI3IniBuilder.AppendSection(ResourceVBSection);


        //6.写出IBResource部分
        IniSection ResourceIBSection(IniSectionType::ResourceIB);
        for (int i = 0; i < drawIBConfig.PartNameList.size(); ++i) {
            std::string partName = drawIBConfig.PartNameList[i];
            ResourceIBSection.Append(L"[Resource" + drawIBConfig.DrawIB + G.GIMIPartNameAliasMap[MMTString::ToWideString(partName)] + L"]");
            ResourceIBSection.Append(L"type = Buffer");
            ResourceIBSection.Append(L"format = DXGI_FORMAT_R32_UINT");
            ResourceIBSection.Append(L"filename = Buffer/" + drawIBConfig.DrawIB + G.GIMIPartNameAliasMap[MMTString::ToWideString(partName)] + L".ib");
            ResourceIBSection.NewLine();
        }
        HI3IniBuilder.AppendSection(ResourceIBSection);

        //7.写出贴图resource部分
        if (!drawIBConfig.ForbidAutoTexture && !drawIBConfig.UseHashTexture) {
            IniSection ResourceTextureSection(IniSectionType::ResourceTexture);
            for (const auto& pair : this->PartName_TextureSlotReplace_Map) {
                TextureSlotReplace slotReplace = pair.second;
                for (const auto& slotPair : slotReplace.Out_ResourceName_FileName_Map) {
                    std::wstring ResourceName = slotPair.first;
                    std::wstring ResourceFileName = slotPair.second;
                    ResourceTextureSection.Append(L"[" + ResourceName + L"]");
                    ResourceTextureSection.Append(L"filename = Texture/" + ResourceFileName);
                    ResourceTextureSection.NewLine();
                }
            }
            HI3IniBuilder.AppendSection(ResourceTextureSection);
        }
        
        LOG.NewLine();

        //移动槽位的DDS文件到Mod文件夹中
        BufferUtils::CopySlotDDSTextureFromOutputFolder(drawIBConfig);

        LOG.Info(L"Generate mod completed!");
        LOG.NewLine();

    }


    if (!std::filesystem::exists(G.TimeOutputFolder)) {
        LOG.Error(MMT_Tips::TIP_GenerateMod_NoAnyExportModelDetected);
    }
    else {
        LOG.Info(L"开始生成基于Hash值的this = 类型贴图替换,如未勾选则不会生成");
        std::unordered_map<std::wstring, std::wstring> Hash_TextureFileNameMap;
        for (const auto& DrawIBPair : G.DrawIB_ExtractConfig_Map) {
            std::wstring drawIB = DrawIBPair.first;
            DrawIBConfig extractConfig = DrawIBPair.second;
            LOG.Info(L"DrawIB: " + drawIB);
            LOG.Info(L"UseHashTexture: " + std::to_wstring(extractConfig.UseHashTexture));
            LOG.Info(L"Allow TextureGenerate: " + std::to_wstring(!extractConfig.ForbidAutoTexture));
            LOG.NewLine();
            if (extractConfig.UseHashTexture && !extractConfig.ForbidAutoTexture) {
                for (const auto& texturePair : extractConfig.PartName_TextureSlotReplace_Map) {
                    std::string PartName = texturePair.first;
                    std::vector<std::string> TextureFileNameList = texturePair.second;
                    for (std::string TextureFileName : TextureFileNameList) {

                        std::vector<std::wstring> TextureFileNameSplitList = MMTString::SplitStringOnlyMatchFirst(MMTString::ToWideString(TextureFileName), L"=");
                        std::wstring RealTextureFileName = boost::algorithm::trim_copy(TextureFileNameSplitList[1]);
                        std::vector<std::wstring> StrSplitList = MMTString::SplitString(RealTextureFileName, L"-");
                        //std::wstring DrawIB = StrSplitList[0];
                        std::wstring TextureHash = StrSplitList[1];
                        Hash_TextureFileNameMap[TextureHash] = RealTextureFileName;
                    }
                }
            }
        }
        LOG.Info(L"Hash_TextureFileNameMap Size: " + std::to_wstring(Hash_TextureFileNameMap.size()));
        LOG.NewLine();


        if (Hash_TextureFileNameMap.size() != 0) {
            std::wstring TextureOutputFolder = G.TimeOutputFolder + L"Texture\\";
            std::filesystem::create_directories(TextureOutputFolder);

            for (const auto& DrawIBPair : G.DrawIB_ExtractConfig_Map) {
                std::wstring drawIB = DrawIBPair.first;
                DrawIBConfig extractConfig = DrawIBPair.second;
                std::wstring BufferReadFolder = G.OutputFolder + drawIB + L"/";
                LOG.Info(L"DrawIB: " + drawIB);

                for (const auto& TexturePair : Hash_TextureFileNameMap) {
                    std::wstring TextureHash = TexturePair.first;
                    std::wstring FileName = TexturePair.second;

                    std::wstring FilePath = BufferReadFolder + FileName;
                    LOG.Info(L"Current Processing: " + FilePath);

                    std::vector<std::wstring> StrSplitList = MMTString::SplitString(FileName, L"-");
                    std::wstring NewFileName = drawIB + L"_" + TextureHash + L"_" + StrSplitList[3];

                    if (std::filesystem::exists(FilePath)) {
                        //先拼接写出Resource

                        IniSection ResourceSection(IniSectionType::ResourceTexture);
                        ResourceSection.SectionLineList.push_back(L"[Resource_Texture_" + TextureHash + L"]");
                        ResourceSection.SectionLineList.push_back(L"filename = Texture/" + NewFileName + L"");
                        ResourceSection.NewLine();
                        HI3IniBuilder.AppendSection(ResourceSection);

                        IniSection TextureOverrideSection(IniSectionType::TextureOverrideTexture);
                        TextureOverrideSection.SectionLineList.push_back(L"[TextureOverride_" + TextureHash + L"]");
                        TextureOverrideSection.SectionLineList.push_back(L"hash = " + TextureHash);
                        TextureOverrideSection.SectionLineList.push_back(L"this = Resource_Texture_" + TextureHash);
                        TextureOverrideSection.NewLine();
                        HI3IniBuilder.AppendSection(TextureOverrideSection);

                        //把原本的文件复制到对应的Hash文件
                        std::filesystem::copy_file(FilePath, TextureOutputFolder + NewFileName, std::filesystem::copy_options::skip_existing);
                    }
                }
                LOG.NewLine();
            }

            LOG.Info(L"生成Hash贴图替换成功");
        }
        LOG.NewLine();
    }

    HI3IniBuilder.AppendSection(BufferUtils::GetCreditInfoIniSection());
    HI3IniBuilder.SaveToFile(G.TimeOutputFolder + L"Config.ini");

    BufferUtils::AddCreditREADME();

    LOG.NewLine();


}