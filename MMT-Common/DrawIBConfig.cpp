#include "DrawIBConfig.h"

#include "MMTLogUtils.h"
#include "MMTJsonUtils.h"
#include "MMTFileUtils.h"
#include "MMTStringUtils.h"


bool DrawIBConfig::Initialize(std::wstring OutputFolder, D3D11GameType InputD3d11GameType) {
    //获取当前日期 YYYY_MM_DD
    std::wstring timeStr = MMTString::GetFormattedDateTimeForFilename().substr(0, 10);
    this->ModOutputFolder = OutputFolder + timeStr;

    // 指定输出目录
    this->BufferReadFolder = OutputFolder + this->DrawIB + L"\\";
    this->BufferOutputFolder = this->ModOutputFolder + L"\\Buffer\\";
    this->TextureOutputFolder = this->ModOutputFolder + L"\\Texture\\";
    this->ModelFolder = this->ModOutputFolder + L"\\Model\\";
    this->CreditOutputFolder = this->ModOutputFolder + L"\\Credit\\";

    this->d3d11GameType = InputD3d11GameType;

    //判断是否存在1.vb什么的来判断当前DrawIB是否导出了模型文件
    bool findVBModel = false;
    for (std::string partName : this->PartNameList) {
        std::wstring VBFileName = MMTString::ToWideString(partName) + L".vb";
        if (std::filesystem::exists(BufferReadFolder + VBFileName)) {
            findVBModel = true;
        }
    }

    if (findVBModel) {
        //先删除上一次生成的所有Buffer文件
        //std::filesystem::remove_all(BufferOutputFolder);

        //再重新创建Buffer文件夹
        std::filesystem::create_directories(BufferOutputFolder);
        std::filesystem::create_directories(TextureOutputFolder);
        std::filesystem::create_directories(ModelFolder);
        //std::filesystem::create_directories(CreditOutputFolder);
        return true;
    }
    else {
        LOG.Warning(L"Detect didn't export vb file for DrawIB: " + this->DrawIB + L" , so skip this DrawIB's mod generate.");
        return false;
    }


}


//接收outputFolder，并保存到outputfolder下面的DrawIB为名称的文件夹里
void DrawIBConfig::SaveTmpJsonConfigs(std::wstring outputPath) {
    LOG.Info(L"Start to save tmp config");

    //write to json file.
    nlohmann::json tmpJsonData;
    tmpJsonData["VertexLimitVB"] = VertexLimitVB;
    tmpJsonData["TmpElementList"] = TmpElementList;
    tmpJsonData["CategoryHash"] = CategoryHashMap;
    tmpJsonData["MatchFirstIndex"] = MatchFirstIndexList;
    tmpJsonData["PartNameList"] = PartNameList;
    tmpJsonData["WorkGameType"] = WorkGameType;

    tmpJsonData["PartNameTextureResourceReplaceList"] = this->PartName_TextureSlotReplace_Map;

    std::wstring configCompletePath = outputPath + L"tmp.json";
    LOG.Info(L"configCompletePath: " + configCompletePath);
    // save json config into file.
    // 这里如果文件不存在，则会自动创建新的文件来写入，所以不用担心。
    if (std::filesystem::exists(configCompletePath)) {
        std::filesystem::remove(configCompletePath);
    }
    std::ofstream file(configCompletePath);
    std::string content = tmpJsonData.dump(4); // 使用缩进格式保存到文件，缩进为4个空格
    file << content << "\n";
    file.close();
    LOG.Info(L"Json config has saved into file.");

    LOG.NewLine();
}
