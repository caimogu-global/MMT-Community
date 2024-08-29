using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NMBT_GUI
{
    partial class Main
    {


        //给ShaderCheck添加一行新的
        public void addLineToDataGridShaderCheckList(string hash, string checkSlots, string forbiden = "")
        {
            DataGridViewRow row = new DataGridViewRow();

            DataGridViewTextBoxCell textBoxCellHash = new DataGridViewTextBoxCell();
            textBoxCellHash.Value = hash;
            row.Cells.Add(textBoxCellHash);

            DataGridViewTextBoxCell textBoxCellCheckSlots = new DataGridViewTextBoxCell();
            textBoxCellCheckSlots.Value = checkSlots;
            row.Cells.Add(textBoxCellCheckSlots);

            DataGridViewCheckBoxCell checkBoxForbiden = new DataGridViewCheckBoxCell();
            checkBoxForbiden.Value = forbiden;
            row.Cells.Add(checkBoxForbiden);

            dataGridViewShaderCheckList.Rows.Add(row);
        }

        //更新ShaderCheck列表
        private bool updateShaderCheckList(string basic_check_setting_path = "")
        {
            if (basic_check_setting_path == "")
            {
                basic_check_setting_path = this.Path_Game_VSCheck_Json;
            }

            if (!File.Exists(basic_check_setting_path))
            {
                return false;
            }

            //先清空当前的列表
            dataGridViewShaderCheckList.Rows.Clear();

            //Read hash and check slot from BasicCheckSetting.json
            string json = File.ReadAllText(basic_check_setting_path);
            // 解析 JSON
            JObject jsonObject = JObject.Parse(json);

            //如果配置里没有当前游戏的，就直接返回
            if (!jsonObject.ContainsKey(CurrentGameName))
            {
                return false;
            }

            // 获取当前游戏的JObject，这里使用了一个键值对对象，但是需要换成三个属性的列表
            JArray gameJarray = (JArray)jsonObject[CurrentGameName];

            foreach (JObject property in gameJarray)
            {
                string hash = (string)property["ShaderHash"];
                string checkslots = (string)property["CheckSlots"];
                string forbiden = (string)property["forbiden"];
                addLineToDataGridShaderCheckList(hash, checkslots, forbiden);
            }
            return true;
        }


        private bool saveShaderCheckList(string savePath = "")
        {
            if (savePath == "")
            {
                savePath = this.Path_Game_VSCheck_Json;
            }
            //(1) 检查游戏类型是否设置
            if (CurrentGameName == "")
            {
                ShowMessageBox("Please select your current game before you save.", "在保存前请选择您的游戏类型");
                //MessageBox.Show("在保存前请选择您的游戏类型");
                return false;
            }

            JObject jsonObject = new JObject();
            if (File.Exists(savePath))
            {
                string jsonData = File.ReadAllText(savePath);
                jsonObject = JObject.Parse(jsonData);
            }

            JArray gameJarray = new JArray();
            // 获取当前游戏的JObject

            int validCount = 0;
            foreach (DataGridViewRow row in dataGridViewShaderCheckList.Rows)
            {
                if (!row.IsNewRow && row.Cells[0].Value != null && row.Cells[1].Value != null)
                {
                    JObject jobj = new JObject();

                    jobj["ShaderHash"] = row.Cells[0].Value.ToString();
                    jobj["CheckSlots"] = row.Cells[1].Value.ToString();
                    jobj["forbiden"] = row.Cells[2].Value.ToString();
                    gameJarray.Add(jobj);
                    validCount++;
                }
            }
            if (validCount == 0)
            {
                ShowMessageBox("Can't save empty config.", "无法保存空的配置");
                //MessageBox.Show("无法保存空的配置");
                return false;
            }

            //保存覆盖
            jsonObject[CurrentGameName] = gameJarray;
            // 将JObject转换为JSON字符串
            string json_string = jsonObject.ToString(Formatting.Indented);
            // 将JSON字符串写入文件
            File.WriteAllText(savePath, json_string);
            return true;

        }


        private bool generateShaderCheckFile(string basicCheckSettingPath = "")
        {

            if (basicCheckSettingPath == "")
            {
                basicCheckSettingPath = this.Path_Game_VSCheck_Json;
            }

            //生成之前总是先更新
            bool updateResult = updateShaderCheckList();
            if (!updateResult)
            {
                MessageBox.Show("更新顶点着色器检查列表失败");
                return false;
            }

            //然后开始更新，首先获取所有的行的值
            //Read hash and check slot from BasicCheckSetting.json
            string json = File.ReadAllText(basicCheckSettingPath);
            // 解析 JSON
            JObject jsonObject = JObject.Parse(json);


            //如果配置里没有当前游戏的，就直接返回
            if (!jsonObject.ContainsKey(CurrentGameName))
            {
                return false;
            }

            // 获取当前游戏的JObject
            JArray gameJarray = (JArray)jsonObject[CurrentGameName];



            //接下来可以打开一个ini文件或者组装一个string进行读写了，参考贴图Mod生成
            string shaderCheckString = "";
            foreach (JObject property in gameJarray)
            {
                string hash = (string)property["ShaderHash"];
                string checkslots = (string)property["CheckSlots"];
                string forbiden = (string)property["forbiden"];

                if (forbiden == "True")
                {
                    continue;
                }

                List<string> checkSlotList = checkslots.Split(',').ToList();

                shaderCheckString = shaderCheckString + "[ShaderOverride_" + hash + "]\r\n";
                shaderCheckString = shaderCheckString + "hash = " + hash + "\r\n";
                shaderCheckString = shaderCheckString + "if $costume_mods\r\n";

                foreach (string checkSlot in checkSlotList)
                {
                    shaderCheckString = shaderCheckString + "  checktextureoverride = " + checkSlot + "\r\n";
                }

                shaderCheckString = shaderCheckString + "endif\r\n\r\n";

            }

            File.WriteAllText("Games\\" + CurrentGameName + "\\3Dmigoto\\Mods\\" + "ShaderCheck.ini", shaderCheckString);
            return true;
        }

        

        private Dictionary<string, List<string>> GetBuffHash_VSShaderHashValues_Dict()
        {
            string frameAnalyseFolder = "";
            string[] directories = Directory.GetDirectories(this.Path_LoaderFolder.Replace("/", "\\")); ;
            List<string> frameAnalysisFileList = new List<string>();
            foreach (string directory in directories)
            {
                string directoryName = Path.GetFileName(directory);

                if (directoryName.StartsWith("FrameAnalysis-"))
                {
                    frameAnalysisFileList.Add(directoryName);
                }
            }

            //Get FA numbers to reserve
            frameAnalysisFileList.Sort();
            if (frameAnalysisFileList.Count > 0)
            {
                //排序后是从小到大的，时间上也是如此，我们这里是找最新的一个，所以选-1个
                frameAnalyseFolder = frameAnalysisFileList[frameAnalysisFileList.Count - 1];
            }
            else
            {
                ShowMessageBox("Can't find any FrameAnalysisFolder", "未找到FrameAnalysisFolder");
                //MessageBox.Show("未找到FrameAnalysisFolder");
            }

            if (frameAnalyseFolder == "")
            {
                ShowMessageBox("Current specified FrameAnalysisFolder didn't exists, please check your setting", "当前指定的FrameAnalysisFolder不存在，请重新设置");
            }

            string frameAnalysisFolderPath = this.Path_LoaderFolder + frameAnalyseFolder;

            Dictionary<string, List<string>> buffHash_vsShaderHashValues_Dict = new Dictionary<string, List<string>>();

            // 获取当前目录下的所有文件
            string[] files = Directory.GetFiles(frameAnalysisFolderPath);
            foreach (string fileName in files)
            {
                if (!fileName.EndsWith(".txt"))
                {
                    continue;
                }

                int vsIndex = fileName.IndexOf("-vs=");
                if (vsIndex != -1)
                {
                    string bufferHash = fileName.Substring(vsIndex - 8, 8);
                    string vsShaderHash = fileName.Substring(vsIndex + 4, 16);

                    List<string> tmpList = new List<string>();
                    if (buffHash_vsShaderHashValues_Dict.ContainsKey(bufferHash))
                    {
                        tmpList = buffHash_vsShaderHashValues_Dict[bufferHash];
                    }
                    tmpList.Add(vsShaderHash);
                    buffHash_vsShaderHashValues_Dict[bufferHash] = tmpList;
                }
                else
                {
                    continue;
                }

            }

            

            return buffHash_vsShaderHashValues_Dict;
        }


        private List<string> GetDrawIBListFromTwoInput()
        {
            //首先获取当前DrawIB列表里所有的DrawIB
            List<string> drawIBList = new List<string>();
            foreach (DataGridViewRow row in DataGridView_BasicIBList.Rows)
            {
                if (!row.IsNewRow && row.Cells[0].Value != null)
                {
                    string drawIB = row.Cells[0].Value.ToString();
                    drawIBList.Add(drawIB);
                }
            }

            //其次获取当前SkipIB列表里所有的SkipIB
            if (textBoxSkipIBList.Text.Contains(","))
            {
                string[] skipIBList = textBoxSkipIBList.Text.Split(',');
                drawIBList.AddRange(skipIBList);
            }
            else
            {
                string trimStr = textBoxSkipIBList.Text.Trim();
                if (!String.IsNullOrEmpty(trimStr))
                {
                    drawIBList.Add(textBoxSkipIBList.Text);
                }
            }

            return drawIBList;
        }

    }
}
