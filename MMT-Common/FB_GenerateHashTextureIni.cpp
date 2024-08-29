#include "Functions_Basic.h"

#include "GlobalConfigs.h"
#include "IniBuilder.h"


void Functions_Basic::GenerateHashTextureIni_Deprecated(std::wstring ModOutputFolder) {

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
        std::wstring HashOutputFolder = ModOutputFolder + L"\\" + L"Textures\\";
        std::filesystem::create_directories(HashOutputFolder);

        std::wstring HashIniFilePath = HashOutputFolder + L"Texture.ini";
        std::wofstream HashIniFile(HashIniFilePath);

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
                    HashIniFile << L"[Resource_Texture_" + TextureHash + L"]" << L"\n";
                    HashIniFile << L"filename = " + NewFileName << L"\n" << L"\n";

                    HashIniFile << L"[TextureOverride_" + TextureHash + L"]" << L"\n";
                    HashIniFile << L"hash = " + TextureHash << L"\n";
                    HashIniFile << L"this = Resource_Texture_" + TextureHash << L"\n" << L"\n";

                    //把原本的文件复制到对应的Hash文件
                    std::filesystem::copy_file(FilePath, HashOutputFolder + NewFileName, std::filesystem::copy_options::skip_existing);
                }
            }
            LOG.NewLine();
        }
        HashIniFile.close();

        LOG.Info(L"生成Hash贴图替换成功");
    }
    LOG.NewLine();
}


void Functions_Basic::GenerateHashTextureIni_2(std::wstring ModOutputFolder) {

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


    //TODO 在这里生成一堆M_IniSection，在用到的时候按顺序输出即可，这里要给M_IniSection加个类型，这个类型需要是常量或者枚举类型
    std::vector<IniSection> TextureIniSectionList;
   
    if (Hash_TextureFileNameMap.size() != 0) {
        std::wstring HashOutputFolder = ModOutputFolder + L"\\" + L"Textures\\";
        std::filesystem::create_directories(HashOutputFolder);
      
        std::wstring HashIniFilePath = HashOutputFolder + L"Texture.ini";
        std::wofstream HashIniFile(HashIniFilePath);

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

                    IniSection ResourceSection;
                    ResourceSection.SectionType = IniSectionType::ResourceTexture;
                    ResourceSection.SectionLineList.push_back(L"[Resource_Texture_" + TextureHash + L"]");
                    ResourceSection.SectionLineList.push_back(L"filename = " + NewFileName + L"");
                    TextureIniSectionList.push_back(ResourceSection);

                    IniSection TextureOverrideSection;
                    TextureOverrideSection.SectionType = IniSectionType::TextureOverrideTexture;
                    TextureOverrideSection.SectionLineList.push_back(L"[TextureOverride_" + TextureHash + L"]");
                    TextureOverrideSection.SectionLineList.push_back(L"hash = " + TextureHash);
                    TextureOverrideSection.SectionLineList.push_back(L"this = Resource_Texture_" + TextureHash);
                    TextureIniSectionList.push_back(TextureOverrideSection);

                    //把原本的文件复制到对应的Hash文件
                    std::filesystem::copy_file(FilePath, HashOutputFolder + NewFileName, std::filesystem::copy_options::skip_existing);
                }
            }
            LOG.NewLine();
        }
        HashIniFile.close();

        LOG.Info(L"生成Hash贴图替换成功");
    }
    LOG.NewLine();
}