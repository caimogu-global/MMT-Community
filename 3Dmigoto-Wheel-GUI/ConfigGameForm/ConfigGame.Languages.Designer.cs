using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NMBT_GUI
{
    partial class ConfigGame
    {
        void ShowMessageBox(string EnglishTip, string ChineseTip)
        {
            if (currentLanguage == "en")
            {
                MessageBox.Show(EnglishTip);
            }
            else if (currentLanguage == "zh-cn")
            {
                MessageBox.Show(ChineseTip);
            }
        }
    
        void switchToChinese()
        {
            checkBoxAutoCleanLog.Text = "在退出MMT之前自动清理生成的日志文件，保留数量：";
            checkBoxAutoCleanFrameAnalysisFolder.Text = "在退出MMT之前自动清理生成的帧分析文件夹，保留数量：";
            checkBoxDeleteOutputFolder.Text = "在提取模型之前自动删除Output文件夹";
            checkBoxBackUp.Text = "在生成二创模型之前自动备份之前的Output文件夹";
            checkBox_AutoTextureOnlyConvertDiffuseMap.Text = "自动上贴图仅转换DiffuseMap.dds为.tga格式";
            checkBoxTopMost.Text = "保持MMT窗口总是在屏幕最高层级显示，确保一直处于可视状态";
            checkBoxShareSourceModel.Text = "在生成Mod文件时分享导出的源模型";
            checkBoxOpenModOutputFolder.Text = "在生成二创模型后自动打开Mod生成文件夹";

            //Credit Info
            groupBoxCreditInfo.Text = "版权信息";
            labelAuthor.Text = "作者名称";
            labelAuthorLink.Text = "赞助链接";
        }
    
    }
}
