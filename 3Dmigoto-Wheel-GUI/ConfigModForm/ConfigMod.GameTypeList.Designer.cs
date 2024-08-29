using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NMBT_GUI
{
    partial class ConfigMod
    {
        public void InitializeGameTypeList()
        {
            string GameTypesDirectoryStr = "Configs\\ExtractTypes\\" + this.CurrentGame + "\\";
            //初始化游戏类型列表
            List<string> currentGameTypeList = new List<string>();
            currentGameTypeList.Add("Auto");

            if (Directory.Exists(GameTypesDirectoryStr))
            {
                //读取GameListSetting.json文件里的游戏类型列表
                string[] game_types = Directory.GetFiles(GameTypesDirectoryStr);
                foreach (string game_type in game_types)
                {
                    string game_type_filename = Path.GetFileName(game_type);
                    string game_type_name = game_type_filename.Substring(0, game_type_filename.Length - 5);
                    currentGameTypeList.Add(game_type_name);
                }
            }

            game_type_dict.Add(this.CurrentGame, currentGameTypeList);
        }



    }
}
