#include "MMTLogUtils.h"
#include "MMTStringUtils.h"
#include "MMTJsonUtils.h"


MMTLogger::MMTLogger() {

}


MMTLogger::MMTLogger(const std::wstring ApplicationLocation) {
    this->RunResultJsonPath = ApplicationLocation + L"\\Configs\\RunResult.json";
}


void MMTLogger::Info(const std::wstring str) {
    LOG(INFO) << MMTString::ToByteString(str);
}


void MMTLogger::Info(const std::string str) {
    LOG(INFO) << str;
}


void MMTLogger::Warning(const std::wstring str) {
    LOG(INFO) << MMTString::ToByteString(L"[Warning]:" + str);
}


void MMTLogger::Warning(const std::string str) {
    LOG(INFO) << "[Warning]:" + str;
}


void MMTLogger::SaveResultJson(const std::wstring str) {
    nlohmann::json runResultJson;
    runResultJson["result"] = MMTString::ToByteString(str);
    MMTJson::SaveToJsonFile(this->RunResultJsonPath, runResultJson);
}


void MMTLogger::SaveResultJson(const std::string str) {
    nlohmann::json runResultJson;
    runResultJson["result"] = str;
    MMTJson::SaveToJsonFile(this->RunResultJsonPath, runResultJson);
}


void MMTLogger::Error(const std::wstring str) {
    //把结果写到json文件里面
    SaveResultJson(str);
    std::wcout << L"[Error]:" << str << "\n";
    LOG(INFO) << MMTString::ToByteString(L"[Error]:" + str);
    exit(1);
}


void MMTLogger::Error(const std::string str) {
    //把结果写到json文件里面
    SaveResultJson(str);
    LOG(INFO) << "[Error]:" + str;
    exit(1);
}


void MMTLogger::Success() {
    //把结果写到json文件里面
    SaveResultJson(L"success");
    LOG(INFO) << "Run complete! Success!";
}


void MMTLogger::NewLine() {
    Info("--------------------------------------------------------------------");
}

void MMTLogger::NewSeperator() {
    Info("--------------------------------------------------------------------");
    Info("--------------------------------------------------------------------");
    Info("--------------------------------------------------------------------");
    Info("--------------------------------------------------------------------");
    Info("--------------------------------------------------------------------");
    Info("--------------------------------------------------------------------");
}