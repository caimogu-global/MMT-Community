#include <io.h>
#include "GlobalConfigs.h"

//CPU-PreSkinning
#include "Functions_Unity_CPU_PreSkinning.h"

//UE4 CS
#include "Functions_WW.h"
#include "Functions_ToF.h"

//UE4 VS
#include "Functions_WW1.1.h"
#include "Functions_KBY.h"
#include "Functions_SnB.h"

//Unity VS
#include "Functions_HI3.h"
#include "Functions_GI.h"
#include "Functions_HSR.h"
#include "Functions_ZZZ.h"

//Unity CS


#include "MMTTimeUtils.h"


//初始化easylogpp
INITIALIZE_EASYLOGGINGPP
//初始化全局配置
GlobalConfigs G;
//初始化日志
MMTLogger LOG;


std::int32_t wmain(std::int32_t argc, wchar_t* argv[])
{
    //设置使用中文locale
    setlocale(LC_ALL, "Chinese-simplified");

    std::wstring fullPath = argv[0];
    std::wstring applicationLocation = MMTString::GetFolderPathFromFilePath(fullPath);
    
    //因为我们需要放到Plugins目录中
    applicationLocation = MMTString::GetParentFolderPathFromFolderPath(applicationLocation);

    //首先初始化日志配置，非常重要
    std::string logFileName = "Logs\\" + MMTTime::GetCurrentTimeString() + ".log";
    el::Configurations logConfigurations;
    logConfigurations.setToDefault();
    logConfigurations.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "false");
    logConfigurations.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
    logConfigurations.set(el::Level::Global, el::ConfigurationType::Filename, logFileName);
    el::Loggers::reconfigureAllLoggers(logConfigurations);

    LOG.Info(L"Running : " + fullPath);
    LOG.NewLine();

    //初始化日志类
    LOG = MMTLogger(applicationLocation);
    //初始化全局配置
    G = GlobalConfigs(applicationLocation);

    //初始化各游戏处理程序
    LOG.Info(L"欢迎使用MMT-Community，本程序由NicoMico开发，在Github开源免费：https://github.com/StarBobis/MigotoModTool");
    LOG.Info(L"当前执行：" + G.RunCommand);
    LOG.NewLine();

    //根据游戏类型决定具体使用哪个子类进行处理
    Functions_Basic* basic_functions = new Functions_Basic();

    if (G.GameName == L"GI") {
        basic_functions = new Functions_GI();
    }
    else if (G.GameName == L"HI3") {
        basic_functions = new Functions_HI3();
    }
    else if (G.GameName == L"HSR") {
        basic_functions = new Functions_HSR();
    }
    else if (G.GameName == L"ZZZ") {
        basic_functions = new Functions_ZZZ();
    }
    else if (G.GameName == L"WW1.1") {
        basic_functions = new Functions_WW11();
    }
    else if (G.GameName == L"SnB") {
        basic_functions = new Functions_SnB();
    }
    else if (G.GameName == L"KBY") {
        basic_functions = new Functions_KBY();
    }
    else if (G.GameName == L"ToF") {
        basic_functions = new Functions_ToF();
    }
    else if (G.GameName == L"WW") {
        basic_functions = new Functions_WW();
    }
    //CPU-PreSkinning的游戏
    else if (G.GameName == L"GF2") {
        basic_functions = new Functions_Unity_CPU_PreSkinning();
    }

    //初始化贴图识别算法列表
    basic_functions->InitializeTextureTypeList();

    //初始化该游戏数据类型
    std::wstring GameTypeConfigPath = G.ApplicationRunningLocation + L"\\Configs\\ExtractTypes\\" + G.GameName + L"\\";
    if (G.GameName == L"GF2") {
        //Unity-CPU-PreSkinning
        GameTypeConfigPath = G.ApplicationRunningLocation + L"\\Configs\\ExtractTypes\\Unity-CPU-PreSkinning\\";
    }
    basic_functions->InitializeGameTypeListFromConfig(GameTypeConfigPath);

    //额外添加逆向提取的数据类型路径
    std::wstring ReverseGameTypeExtraConfigPath = G.ApplicationRunningLocation + L"\\Configs\\ReverseTypes\\";


#ifdef _DEBUG 
    //注意！禁止使用DEBUG模式进行测试和编译！
    LOG.NewLine();
#else
    //正常提取模型
    if (G.RunCommand == L"merge") {
        //如果运行为Merge，则确保存在至少一个FrameAnalysis文件夹
        if (G.FrameAnalyseFolder == L"") {
            LOG.Error("Can't find any FrameAnalysis folder in your 3Dmigoto folder,please try press F8 to dump a new one with Hunting open.");
        }
        if (G.GameName == L"WWMI") {
            LOG.Error(L"WWMI仅用于逆向提取和脚本式一键逆向，如需使用WW功能请切换到WW1.1");
        }
        else {
            basic_functions->ExtractModel();
        }
    }
    //生成Mod
    else if (G.RunCommand == L"split") {

        if (G.GameName == L"WWMI") {
            LOG.Error(L"WWMI仅用于逆向提取和脚本式一键逆向，如需使用WW功能请切换到WW1.1");
        }
        else {
            basic_functions->GenerateMod();
        }
    }
    else {
        LOG.Error(L"未知的命令，请检查您的MMT版本");
    }

    //If we success executed,we will run to here.
    LOG.Success();

    //clean object created by "new" 
    delete basic_functions;
#endif
    return 0;
}
