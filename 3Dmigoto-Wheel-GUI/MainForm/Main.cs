using Microsoft.VisualBasic.FileIO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;


namespace NMBT_GUI
{
    public partial class Main : Form
    {
        //程序名称的基础前缀
        private string MMTName = "MMT-Community V1.1.0.4";
        //当前选择的游戏名称
        private string CurrentGameName = "";
        //当前的语言类型
        private string CurrentLanguage = "";

        //由C++开发的核心算法进程
        private string MMT_EXE_FileName = "MMT.exe";

        //由后面的调用设置游戏类型的函数掌控
        private string window_name = "";

        //当前程序运行所在位置的路径,注意这里已经包含了结尾的\\
        private string basePath = AppDomain.CurrentDomain.BaseDirectory.ToString();
        
        private string Path_MainConfig = "Configs\\Main.json";
        private string Path_RunResultJson = "Configs\\RunResult.json";
        private string Path_RunInputJson = "Configs\\RunInput.json";
        private string Path_Languages = "Configs\\Languages.json";
        private string Path_Game_SettingJson = "Configs\\Setting.json";

        //运行后程序动态生成
        private string Path_Game_ConfigJson = "";
        private string Path_Game_VSCheck_Json = "";
        private string Path_OutputFolder = "";
        private string Path_LoaderFolder = "";

        //首选项设置
        private bool AutoCleanFrameAnalysisFolder = false;
        private int FrameAnalysisFolderReserveNumber = 0;
        private bool AutoCleanLogFile = false;
        private int LogFileReserveNumber = 0;
        private bool DeleteOutputFolder;
        private bool BackUpFiles;
        private bool AutoTextureOnlyConvertDiffuseMap = true;
        private bool MMTWindowTopMost = false;
        private bool OpenModOutputFolderAfterGenerateMod = false;

        //布局切换
        private bool ClassicLayout = true;

        public Main()
        {
            InitializeComponent();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            AutoDetectLanguage();

            //检查当前程序是否为位于中文路径下
            if (ContainsChinese(basePath))
            {
                ShowMessageBox("MMT can't be put in a path that contains Chinese, please put MMT in pure english path!", "MMT所在路径不能含有中文，请重新将MMT放置到纯英文路径！");
                Application.Exit();
            }

            //检查MMT是否存在
            if (!File.Exists(basePath + "Plugins\\" + MMT_EXE_FileName))
            {
                ShowMessageBox("Can't find " + basePath + MMT_EXE_FileName + ",please put it under this program's Plugins folder.", "未找到" + basePath + MMT_EXE_FileName + ",请将其放在本程序Plugins目录下，即将退出程序。");
                Application.Exit();
            }


            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            

            window_name = MMTName;
            this.Text = window_name;

            //填充游戏名称列表
            string[] game_folders = Directory.GetDirectories("Games\\");
            //转换提取出目录名称
            List<string> game_folder_names = new List<string>();
            foreach (string game_folder_path in game_folders)
            {
                string folder_name = Path.GetFileName(game_folder_path);
                game_folder_names.Add(folder_name);
            }

            Menu_GameName_toolStripComboBox.Items.Clear();
            Menu_GameName_toolStripComboBox.Items.AddRange(game_folder_names.ToArray());


            ReadPreferenceSetting();


            //读取游戏类型和DrawIB列表
            readConfig(Path_MainConfig);

            //更新一下ShaderCheck列表
            updateShaderCheckList();

            

        }


        private void DataGridViewIBList_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.ColumnIndex == 1 && e.RowIndex >= 0)
            {
                //获取drawIB的值，先校验DrawIB和游戏类型是否已设置
                object draw_ib_object = DataGridView_BasicIBList.Rows[e.RowIndex].Cells[0].Value;
                if (draw_ib_object == null)
                {
                    ShowMessageBox("Please fill the DrawIB", "请填写用于绘制的IB的哈希值");
                    //MessageBox.Show("Please fill the DrawIB");
                    return;
                }
                string draw_ib = draw_ib_object.ToString();
                if (string.IsNullOrEmpty(draw_ib))
                {
                    ShowMessageBox("Please fill the DrawIB", "请填写用于绘制的IB的哈希值");
                    //MessageBox.Show("Please fill the DrawIB");
                    return;
                }
                if (CurrentGameName == "")
                {
                    ShowMessageBox("Please choose your work game.", "请选择你当前工作的游戏");
                    //MessageBox.Show("Please choose your work game.");
                    return;
                }

                //设置好DrawIB和游戏类型才能执行后面的选项
                //先调出配置界面
                ConfigMod anotherForm = new ConfigMod(draw_ib, CurrentGameName);
                anotherForm.Show();

                //然后根据是否配置完成来显示对应信息
                if (File.Exists(Path_Game_ConfigJson))
                {
                    string jsonData = File.ReadAllText(Path_Game_ConfigJson);
                    JArray jsonArray = JArray.Parse(jsonData);
                    bool find_draw_ib = false;
                    foreach (JObject obj in jsonArray)
                    {
                        string drawIB = (string)obj["DrawIB"];
                        if (draw_ib == drawIB)
                        {
                            find_draw_ib = true;
                            break;
                        }

                    }

                    if (find_draw_ib)
                    {
                        //设置按钮显示为已配置
                        if (CurrentLanguage == "zh-cn")
                        {
                            DataGridView_BasicIBList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = "已配置";
                        }
                        else
                        {
                            DataGridView_BasicIBList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = "Configured";
                        }
                    }
                    else
                    {
                        if (CurrentLanguage == "zh-cn")
                        {
                            DataGridView_BasicIBList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = "未配置";
                        }
                        else
                        {
                            DataGridView_BasicIBList.Rows[e.RowIndex].Cells[e.ColumnIndex].Value = "Unconfigured";
                        }
                    }

                }

            }
        }

        private void DataGridViewIBList_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (e.ColumnIndex == 0 && e.RowIndex >= 0)
            {
                //如果当前行这个button显示为空，则设置显示为未配置
                if (DataGridView_BasicIBList.Rows[e.RowIndex].Cells[1].Value == null)
                {
                    DataGridViewCell buttonCell = DataGridView_BasicIBList.Rows[e.RowIndex].Cells[1];
                    if (CurrentLanguage == "zh-cn")
                    {
                        buttonCell.Value = "未配置";
                    }
                    else
                    {
                        buttonCell.Value = "Unconfigured";
                    }
                }

                //如果当前行的DrawIB显示为空，则设置显示也为空
                if (DataGridView_BasicIBList.Rows[e.RowIndex].Cells[0].Value == null)
                {

                    DataGridViewCell buttonCell = DataGridView_BasicIBList.Rows[e.RowIndex].Cells[1];
                    buttonCell.Value = "";
                }
                else
                {
                    //TODO 在Config.json中查找是否有这个DrawIB，如果没有则显示为未配置，和上面差不多
                    string DrawIBValue = DataGridView_BasicIBList.Rows[e.RowIndex].Cells[0].Value.ToString();
                    if (File.Exists(this.Path_Game_ConfigJson))
                    {
                        string jsonData = File.ReadAllText(this.Path_Game_ConfigJson);
                        JArray jsonArray = JArray.Parse(jsonData);

                        bool findDrawIB = false;
                        foreach (JObject obj in jsonArray)
                        {
                            string drawIB = (string)obj["DrawIB"];
                            if (drawIB == DrawIBValue)
                            {
                                findDrawIB = true;
                                break;
                            }
                        }

                        if (!findDrawIB)
                        {
                            DataGridViewCell buttonCell = DataGridView_BasicIBList.Rows[e.RowIndex].Cells[1];
                            if (CurrentLanguage == "zh-cn")
                            {
                                buttonCell.Value = "默认配置";
                            }
                            else
                            {
                                buttonCell.Value = "DefaultConfig";
                            }

                            ConfigMod anotherForm = new ConfigMod(DrawIBValue, CurrentGameName);
                            anotherForm.Show();
                            anotherForm.Close();
                        }
                    }
                    else
                    {
                        ConfigMod anotherForm = new ConfigMod(DrawIBValue, CurrentGameName);
                        anotherForm.Show();
                        anotherForm.Close();
                    }
                }

            }
        }

 



        //清空DrawIB界面
        private void 清空ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DataGridView_BasicIBList.Rows.Clear();
            saveConfig();

        }
        //保存DrawIB界面
        private void 保存ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool saveResult = saveConfig();
            if (saveResult)
            {
                ShowMessageBox("Save Success","保存成功");
                //MessageBox.Show("Save Success");
            }
            else
            {
                ShowMessageBox("Save failed, please check your config.", "保存失败，请检查你的配置");
                //MessageBox.Show("Save failed, please check your config.");
            }
        }


        void convertAllDdsToTgaInOutputFolder()
        {
            //在这里把所有output目录下的dds转为png格式
            string[] subdirectories = Directory.GetDirectories(this.Path_OutputFolder);
            foreach (string outputDirectory in subdirectories)
            {
                string[] filePathArray = Directory.GetFiles(outputDirectory);
                foreach (string ddsFilePath in filePathArray)
                {
                    if (this.AutoTextureOnlyConvertDiffuseMap)
                    {
                        if (!ddsFilePath.EndsWith("DiffuseMap.dds"))
                        {
                            continue;
                        }
                    }
                    else
                    {
                        if (!ddsFilePath.EndsWith(".dds"))
                        {
                            continue;
                        }
                    }
                   
                    Process process = new Process();
                    process.StartInfo.FileName = basePath + "Plugins\\texconv.exe";
                    if (!File.Exists(process.StartInfo.FileName))
                    {
                        ShowMessageBox("Current run path didn't exsits: " + process.StartInfo.FileName, "当前要执行的路径不存在: " + process.StartInfo.FileName);
                    }

                    process.StartInfo.Arguments = " " + ddsFilePath + " -ft tga -o " + outputDirectory;
                    process.StartInfo.UseShellExecute = false;  // 不使用操作系统的shell启动程序
                    process.StartInfo.RedirectStandardOutput = false;  // 重定向标准输出
                    process.StartInfo.RedirectStandardError = false;   // 重定向标准错误输出
                    process.StartInfo.CreateNoWindow = true;  // 不创建新窗口
                    process.Start();
                    process.WaitForExit();
                }
            }
        }

        //提取模型
        private void mergeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            preDoBeforeMerge();
            bool command_run_result = runCommand("merge");
            if (command_run_result)
            {
                //打开Output文件夹
                Process.Start(this.Path_OutputFolder);
            }
        }



        //生成模型
        private void splitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            runCommand("split");
            backupFiles();
            if (this.OpenModOutputFolderAfterGenerateMod)
            {
                openModGenerateFolder();
            }
        }

        private void DrawIB_清除当前配置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DataGridView_BasicIBList.Rows.Clear();
            saveConfig();
        }

        private void DrawIB_保存配置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool saveResult = saveConfig();
            if (saveResult)
            {
                ShowMessageBox("Save Success", "保存成功");

            }
            else
            {
                ShowMessageBox("Save Failed, please check your config", "保存失败，请检查配置");

                //MessageBox.Show("保存失败，请检查配置");
            }
        }



        private void File_打开Output文件夹ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(this.Path_OutputFolder))
            {
                if (Directory.Exists(Path_OutputFolder))
                {
                    Process.Start(this.Path_OutputFolder);
                }
                else
                {
                    ShowMessageBox("This folder doesn't exists,please check if your OutputFolder is correct.", "此目录不存在，请检查您的Output文件夹是否设置正确");
                    //MessageBox.Show("此目录不存在，请检查您的Output文件夹是否设置正确");
                }
            }
        }

        void openModGenerateFolder()
        {
            if (!string.IsNullOrEmpty(this.Path_OutputFolder))
            {
                DateTime currentDate = DateTime.Now;
                string formattedDate = currentDate.ToString("yyyy_MM_dd");
                string targetFolder = this.Path_OutputFolder + "/" + formattedDate + "/";
                if (Directory.Exists(targetFolder))
                {
                    Process.Start(targetFolder);
                }
                else
                {
                    ShowMessageBox("You have not generate any mod yet", "您还未生成二创模型");
                    //MessageBox.Show("您还未生成二创模型");
                }
            }
        }

        private void File_打开二创模型输出文件夹ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openModGenerateFolder();
        }

        private void File_打开工具所在目录ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(this.basePath);
        }

        private void 打开Logs目录ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(this.basePath + "Logs\\");
        }

        private void 打开Mods目录ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string modsFolder = this.Path_LoaderFolder + "Mods/";
            if (!string.IsNullOrEmpty(modsFolder))
            {
                if (Directory.Exists(modsFolder))
                {
                    Process.Start(modsFolder);
                }
                else
                {
                    ShowMessageBox("This path didn't exists, please check if your Mods folder is correct", "此目录不存在，请检查您的Mods文件夹是否设置正确");
                    //MessageBox.Show("此目录不存在，请检查您的Mods文件夹是否设置正确");
                }
            }
        }

        string GetLatestFrameAnalysisFolder()
        {
            try
            {
                string[] directories = Directory.GetDirectories(Path_LoaderFolder);
                List<string> frameAnalysisFileList = new List<string>();
                foreach (string directory in directories)
                {
                    string directoryName = Path.GetFileName(directory);

                    if (directoryName.StartsWith("FrameAnalysis-"))
                    {
                        frameAnalysisFileList.Add(directoryName);
                    }
                }

                //
                if (frameAnalysisFileList.Count > 0)
                {
                    frameAnalysisFileList.Sort();

                    string latestFrameAnalysisFolder = Path_LoaderFolder.Replace("/", "\\") + frameAnalysisFileList.Last();
                    return latestFrameAnalysisFolder;
                }

            }
            catch (IOException ex)
            {
                MessageBox.Show("An IO exception has occurred: " + ex.Message);
            }
            catch (UnauthorizedAccessException ex)
            {
                MessageBox.Show("You do not have permission to access one or more folders: " + ex.Message);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An unexpected exception has occurred: " + ex.Message);
            }

            return "";
        }

        private void 打开最新的FrameAnalysis文件夹ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string latestFrameAnalysisFolder = GetLatestFrameAnalysisFolder();
            if (!string.IsNullOrEmpty(latestFrameAnalysisFolder))
            {
                Process.Start(latestFrameAnalysisFolder);
            }
            else
            {
                ShowMessageBox("Target directory didn't have any FrameAnalysisFolder.", "目标目录没有任何帧分析文件夹");
            }
        }

        private void 打开3Dmigoto文件夹ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Directory.Exists(Path_LoaderFolder))
            {
                Process.Start( Path_LoaderFolder.Replace("/", "\\"));
            }
            else
            {
                ShowMessageBox("This directory doesn't exists.","此目录不存在，请检查3Dmigoto文件夹是否配置正确");
                //MessageBox.Show("This directory doesn't exists.");
            }
        }


        private void 打开ShaderFixs文件夹ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string modsFolder = this.Path_LoaderFolder + "ShaderFixes/";
            if (!string.IsNullOrEmpty(modsFolder))
            {
                if (Directory.Exists(modsFolder))
                {
                    Process.Start(modsFolder.Replace("/","\\"));
                }
                else
                {
                    ShowMessageBox("This folder didn't exsits,please check if your ShaderFixes folder is correct.", "此目录不存在，请检查您的ShaderFixs文件夹是否设置正确");
                    //MessageBox.Show("此目录不存在，请检查您的ShaderFixs文件夹是否设置正确");
                }
            }
        }



        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (AutoCleanFrameAnalysisFolder)
            {
                cleanFrameAnalysisFiles();

            }
            if (AutoCleanLogFile)
            {
                cleanLogFiles();

            }
        }



        private void openLatestLOGFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openLatestLogFile();
        }

        private void 简体中文ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            switchToChinese();
            setCurrentGame(CurrentGameName);
        }

        private void englishToolStripMenuItem_Click(object sender, EventArgs e)
        {
            switchToEnglish();
            setCurrentGame(CurrentGameName);

        }



        private void Menu_File_OpenBackupsFolder_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(this.basePath + "Backups\\");
        }

        private void Menu_File_OpenConfigsFolder_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start(this.basePath + "Configs\\");
        }

        private void Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Directory.Exists(Path_LoaderFolder))
            {
                Process.Start(Path_LoaderFolder.Replace("/", "\\") + "d3dx.ini") ;
            }
            else
            {
                ShowMessageBox("Your 3Dmigoto directory doesn't exists.", "您的3Dmigoto目录不存在，请检查3Dmigoto文件夹是否配置正确");
            }
        }

        private void Menu_GameName_toolStripComboBox_TextChanged(object sender, EventArgs e)
        {
            setCurrentGame(Menu_GameName_toolStripComboBox.Text.ToString());
        }

        

        void preDoBeforeMerge()
        {
            if (this.DeleteOutputFolder)
            {
                if (Directory.Exists(this.Path_OutputFolder))
                {
                    Directory.Delete(this.Path_OutputFolder, true);
                    Directory.CreateDirectory(this.Path_OutputFolder);
                }

            }

            //Before we run extract, we need to check if all the draw ib is configured.
            //foreach (DataGridViewRow row in DataGridView_BasicIBList.Rows)
            //{
            //    if (!row.IsNewRow && row.Cells[1].Value != null)
            //    {
            //        string configStatus = row.Cells[1].Value.ToString();
            //        if (configStatus == "Unconfigured" || configStatus == "未配置")
            //        {
            //            ShowMessageBox("Please config your DrawIB's config before run.", "在运行之前请配置一下您的绘制IB的配置");
            //            return;
            //        }
            //    }
            //}
            //不检查了，不配置的话那边直接使用默认的。


            if (DataGridView_BasicIBList.Rows.Count == 1)
            {
                ShowMessageBox("Please fill your DrawIB and config it before run.", "在运行之前请填写您的绘制IB的哈希值并进行配置");
                return;
            }


            initializeFolders();
        }

       

        private void extractReverseModelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            preDoBeforeMerge();
            bool command_run_result = runCommand("ReverseExtract","MMT-Reverse.exe");
            if (command_run_result)
            {
                convertAllDdsToTgaInOutputFolder();
                Process.Start(this.Path_OutputFolder);
            }
        }


        void RunReverseCommand(string commandStr)
        {
            if (string.IsNullOrEmpty(CurrentGameName))
            {
                ShowMessageBox("Please select your current game before reverse.", "在逆向Mod之前请选择当前要进行格式转换的二创模型的所属游戏");
                return;
            }

            openFileDialog1.Filter = "INI Files (*.ini)|*.ini";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = openFileDialog1.FileName;

                if (ContainsChinese(filePath))
                {
                    ShowMessageBox("Target mod ini file path can't contains Chinese.", "目标Mod的ini文件路径中不能出现中文");
                    return;
                }

                string json = File.ReadAllText(this.Path_RunInputJson); // 读取文件内容
                JObject runInputJson = JObject.Parse(json);
                runInputJson["GameName"] = CurrentGameName;
                runInputJson["ReverseFilePath"] = filePath;
                File.WriteAllText(this.Path_RunInputJson, runInputJson.ToString());

                runCommand(commandStr, "MMT-Reverse.exe");
            }
        }

        private void reverseOutfitCompilerCompressedMergedModToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RunReverseCommand("ReverseOutfitCompilerLv4");
        }

        private void reverseSingleModToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RunReverseCommand("ReverseSingleLv4");
        }

        private void reverseMergedModToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RunReverseCommand("ReverseMergedLv4");
        }

        private void Menu_Run_run3DmigotoLoaderexeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string LoaderExePath = this.Path_LoaderFolder + "3Dmigoto Loader.exe";
            if (File.Exists(LoaderExePath))
            {
                ProcessStartInfo startInfo = new ProcessStartInfo(LoaderExePath);
                startInfo.Verb = "runas";
                startInfo.UseShellExecute = false;
                startInfo.WorkingDirectory = Path.GetDirectoryName(LoaderExePath);
                Process.Start(startInfo);
            }
            else
            {
                ShowMessageBox("Can't find 3Dmigoto Loader.exe in your game's [3Dmigoto] folder.","在您当前游戏的[3Dmigoto]目录中未找到3Dmigoto Loader.exe");
            }
        }

        private void Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string LoaderExePath = this.Path_LoaderFolder + "3Dmigoto Loader-ByPassACE.exe";
            if (File.Exists(LoaderExePath))
            {
                ProcessStartInfo startInfo = new ProcessStartInfo(LoaderExePath);
                startInfo.Verb = "runas";
                startInfo.WorkingDirectory = Path.GetDirectoryName(LoaderExePath);
                startInfo.UseShellExecute = false;
                Process.Start(startInfo);
            }
            else
            {
                ShowMessageBox("Can't find 3Dmigoto Loader-ByPassACE.exe in your game's [3Dmigoto] folder.", "在您当前游戏的[3Dmigoto]目录中未找到3Dmigoto Loader-ByPassACE.exe");
            }
        }

       
        private void openTypesFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string TypeDirStr = "Configs\\ExtractTypes\\" + this.CurrentGameName + "\\";
            if (Directory.Exists(TypeDirStr))
            {
                Process.Start(TypeDirStr);
            }
        }

        private void Menu_Reverse_reverseLv4MergedNameSpaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(CurrentGameName))
            {
                ShowMessageBox("Please select your current game before reverse.", "在逆向Mod之前请选择当前要进行格式转换的二创模型的所属游戏");
                return;
            }
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = folderBrowserDialog1.SelectedPath;
                if (ContainsChinese(filePath))
                {
                    ShowMessageBox("Target mod ini file path can't contains Chinese.", "目标Mod的ini文件路径中不能出现中文");
                    //MessageBox.Show("Target mod ini file path can't contains Chinese.", "目标Mod的ini文件路径中不能出现中文");
                    return;
                }

                string json = File.ReadAllText(this.Path_RunInputJson); // 读取文件内容
                JObject runInputJson = JObject.Parse(json);
                runInputJson["GameName"] = CurrentGameName;
                runInputJson["ReverseFilePath"] = filePath;
                File.WriteAllText(this.Path_RunInputJson, runInputJson.ToString());

                runCommand("ReverseMergedNameSpaceLv4");
            }
        }

        private void Menu_Reverse_reverseLv43DmigotoSimulatorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(CurrentGameName))
            {
                ShowMessageBox("Please select your current game before reverse.", "在逆向Mod之前请选择当前要进行格式转换的二创模型的所属游戏");
                return;
            }
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = folderBrowserDialog1.SelectedPath;
                if (ContainsChinese(filePath))
                {
                    ShowMessageBox("Target mod ini file path can't contains Chinese.", "目标Mod的ini文件路径中不能出现中文");
                    //MessageBox.Show("Target mod ini file path can't contains Chinese.", "目标Mod的ini文件路径中不能出现中文");
                    return;
                }

                string json = File.ReadAllText(this.Path_RunInputJson); // 读取文件内容
                JObject runInputJson = JObject.Parse(json);
                runInputJson["GameName"] = CurrentGameName;
                runInputJson["ReverseFilePath"] = filePath;
                File.WriteAllText(this.Path_RunInputJson, runInputJson.ToString());

                runCommand("Reverse3DmigotoSimulatorLv4", "MMT-Reverse.exe");
            }
        }

        private void joinShaderFreedomDiscordToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start("https://discord.gg/eCXdNyVnsM");
        }

        private void mMT使用文档ToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            Process.Start("https://www.yuque.com/airde/lx53p6?# 《MMT知识库》");
        }



        private void ShaderCheck_updateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool readResult = updateShaderCheckList();
            if (readResult)
            {
                ShowMessageBox("Update Success.", "更新成功");
                //MessageBox.Show("Update Success.");
            }
        }

        private void ShaderCheck_importToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "JSON Files (*.json)|*.json";
            openFileDialog1.FilterIndex = 1;
            openFileDialog1.RestoreDirectory = true;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = openFileDialog1.FileName;
                bool result = updateShaderCheckList(filePath);
                if (result)
                {
                    ShowMessageBox("Import custom vertex shader check config success!", "导入自定义顶点着色器检查配置成功！");
                    //MessageBox.Show("导入自定义顶点着色器检查配置成功！");
                }
                else
                {
                    ShowMessageBox("Import failed, not a valid format", "导入失败，不是正确的格式！");
                    //MessageBox.Show("导入失败，不是正确的格式！");
                }

            }
        }

        private void ShaderCheck_saveToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            bool result = saveShaderCheckList();
            if (result)
            {
                ShowMessageBox("Save Success", "保存成功");
                //MessageBox.Show("Save Success");
            }
        }

        private void ShaderCheck_saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog1.Filter = "JSON Files (*.json)|*.json";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.RestoreDirectory = true;

            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string filePath = saveFileDialog1.FileName;
                bool result = saveShaderCheckList(filePath);
                if (result)
                {
                    ShowMessageBox("Save Success", "保存成功");

                    //MessageBox.Show("Save Success.");
                }
                else
                {
                    ShowMessageBox("Save Failed", "保存失败！");

                    //MessageBox.Show("Save Failed!");
                }

            }
        }

        private void ShaderCheck_generateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool result = generateShaderCheckFile();
            if (result)
            {
                ShowMessageBox("Generate shader check file success.", "生成顶点着色器检查文件成功");
                //MessageBox.Show("Generate shader check file success.");
            }

            Process.Start(this.Path_LoaderFolder + "Mods/");
        }

        private void Menu_PreferencesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConfigGame configBasic = new ConfigGame();
            configBasic.ShowDialog();
            setCurrentGame(this.CurrentGameName);
            ReadPreferenceSetting();
        }


        private void Menu_VertexShaderCheck_initializeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //TODO 删除所有的ShaderCheck的记录，用于重新统计
            dataGridViewShaderCheckList.Rows.Clear();

        }


        private void Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Dictionary<string, List<string>> buffHash_vsShaderHashValues_Dict = GetBuffHash_VSShaderHashValues_Dict();

            List<string> DrawIBList = GetDrawIBListFromTwoInput();

            string outputContent = "";
            //遍历IB列表逐个输出其中内容

            foreach (KeyValuePair<string, List<string>> kvp in buffHash_vsShaderHashValues_Dict)
            {
                string indexBufferHash = kvp.Key;
                List<string> values = kvp.Value;

                if (DrawIBList.Contains(indexBufferHash))
                {
                    outputContent = outputContent + "IndexBufferHash: " + indexBufferHash + "\n";
                    outputContent = outputContent + "对应需Check的VertexShaderHash列表:\n";
                    foreach (string shaderHash in values)
                    {
                        outputContent = outputContent + "VS: " + shaderHash + "\n";
                    }
                }

            }

            string outputPath = this.basePath + "Logs\\IndexBuffer_VertexShader_HashMapping.txt";
            File.WriteAllText(outputPath, outputContent);

            Process.Start(outputPath);
        }

        private void Menu_Analysis_VertexShaderCheck_ToolStripMenuItem_Click(object sender, EventArgs e)
        {

            

            Dictionary<string, List<string>> buffHash_vsShaderHashValues_Dict = GetBuffHash_VSShaderHashValues_Dict();
            List<string> DrawIBList = GetDrawIBListFromTwoInput();
            //TODO 写入json配置，并触发Update
            //(1) 检查游戏类型是否设置
            if (CurrentGameName == "")
            {
                ShowMessageBox("Please select your current game before you save.", "请选择您的游戏类型");
            }
            JObject jsonObject = new JObject();

            if (File.Exists(this.Path_Game_VSCheck_Json))
            {
                string jsonData = File.ReadAllText(this.Path_Game_VSCheck_Json);
                jsonObject = JObject.Parse(jsonData);
            }

            JArray gameJarray = new JArray();
            // 获取当前游戏的JObject

            HashSet<string> VSHashList = new HashSet<string>();
            //统计一下新的
            foreach (KeyValuePair<string, List<string>> kvp in buffHash_vsShaderHashValues_Dict)
            {
                string indexBufferHash = kvp.Key;
                List<string> values = kvp.Value;

                //这里IB必须存在于两个DrawIB的列表里
                if (DrawIBList.Contains(indexBufferHash))
                {
                    foreach (string VSHash in values)
                    {
                        VSHashList.Add(VSHash);
                    }
                }
                else
                {
                    //MessageBox.Show(DrawIBList[0]);
                }

            }
            //再统计一下旧的,只有在包含的时候统计
            if (jsonObject.ContainsKey(CurrentGameName))
            {
                JArray gameJarrayRead = (JArray)jsonObject[CurrentGameName];
                foreach (JObject property in gameJarrayRead)
                {
                    string hash = (string)property["ShaderHash"];
                    VSHashList.Add(hash);
                }
            }
            

            //写出
            foreach (string VSHash in VSHashList)
            {
                JObject jobj = new JObject();
                jobj["ShaderHash"] = VSHash;
                jobj["CheckSlots"] = textBoxVertexShaderCheckList.Text;
                jobj["forbiden"] = "false";
                gameJarray.Add(jobj);
            }
                

            //保存覆盖
            jsonObject[CurrentGameName] = gameJarray;
            // 将JObject转换为JSON字符串
            string json_string = jsonObject.ToString(Formatting.Indented);
            // 将JSON字符串写入文件
            File.WriteAllText(this.Path_Game_VSCheck_Json, json_string);


            bool readResult = updateShaderCheckList();
            if (readResult)
            {
                ShowMessageBox("Update Success.", "更新成功");
            }

        }

        private void Menu_Analysis_SkipIB_ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //这里不需要区分match_first_index,这是因为我们实际测试中不再需要用到match_first_index的过滤了。
            //直接分割然后输出即可
            List<string> strings = new List<string>(); 
            if (textBoxSkipIBList.Text.Contains(","))
            {
                strings = textBoxSkipIBList.Text.Split(',').ToList();
            }
            else
            {
                strings.Add(textBoxSkipIBList.Text);
            }

            string outputContent = "";
            foreach (string DrawIB in strings)
            {
                outputContent = outputContent + "[TextureOverride_IB_" + DrawIB + "]\r\n";
                outputContent = outputContent + "hash = " + DrawIB + "\r\n";
                outputContent = outputContent + "handling = skip\r\n";
                outputContent = outputContent + "\r\n";
            }

            string outputPath = this.Path_OutputFolder + "IBSkip.ini";
            File.WriteAllText(outputPath, outputContent);

            Process.Start(this.Path_OutputFolder);
            //ShowMessageBox("Run Success.", "生成成功");

        }

        private void 切换布局ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            if (ClassicLayout)
            {
                this.Width = 1238;
                this.Height = 420;
                
                ClassicLayout = false;
            }
            else
            {
                this.Width = 603;
                this.Height = 301;
                ClassicLayout = true;
            }
        }

        private void Menu_Run_run3DmigotoLoaderpyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string LoaderExePath = this.Path_LoaderFolder + "3Dmigoto Loader.py";
            if (File.Exists(LoaderExePath))
            {
                Process.Start(LoaderExePath);
            }
            else
            {
                ShowMessageBox("Can't find 3Dmigoto Loader.py in your game's [3Dmigoto] folder.", "在您当前游戏的[3Dmigoto]目录中未找到3Dmigoto Loader.py");
            }
        }

        private void Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                string[] directories = Directory.GetDirectories(Path_LoaderFolder);
                List<string> frameAnalysisFileList = new List<string>();
                foreach (string directory in directories)
                {
                    string directoryName = Path.GetFileName(directory);

                    if (directoryName.StartsWith("FrameAnalysis-"))
                    {
                        frameAnalysisFileList.Add(directoryName);
                    }
                }

                //
                if (frameAnalysisFileList.Count > 0)
                {
                    frameAnalysisFileList.Sort();

                    string latestFrameAnalysisFolder = Path_LoaderFolder.Replace("/", "\\") + frameAnalysisFileList.Last();

                    Process.Start(latestFrameAnalysisFolder + "\\log.txt");
                    //MessageBox.Show(latestFrameAnalysisFolderName);
                }
                else
                {
                    ShowMessageBox("Target directory didn't have any FrameAnalysisFolder.", "目标目录没有任何帧分析文件夹");
                    //MessageBox.Show("Target directory didn't have any FrameAnalysisFolder.");
                }


            }
            catch (IOException ex)
            {
                MessageBox.Show("An IO exception has occurred: " + ex.Message);
            }
            catch (UnauthorizedAccessException ex)
            {
                MessageBox.Show("You do not have permission to access one or more folders: " + ex.Message);
            }
            catch (Exception ex)
            {
                MessageBox.Show("An unexpected exception has occurred: " + ex.Message);
            }
        }

        private void checkForUpdateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Process.Start("https://github.com/StarBobis/MigotoModTool/releases");
        }

        private void Menu_File_openDedupedFolderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string latestFrameAnalysisFolder = GetLatestFrameAnalysisFolder();
            if (!string.IsNullOrEmpty(latestFrameAnalysisFolder))
            {
                Process.Start(latestFrameAnalysisFolder + "\\deduped\\");
            }
            else
            {
                ShowMessageBox("Target directory didn't have any FrameAnalysisFolder.", "目标目录没有任何帧分析文件夹");
            }
        }
    }
}
