namespace NMBT_GUI
{
    partial class ConfigGame
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.checkBoxDeleteOutputFolder = new System.Windows.Forms.CheckBox();
            this.checkBoxBackUp = new System.Windows.Forms.CheckBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.checkBoxAutoCleanLog = new System.Windows.Forms.CheckBox();
            this.textBoxLogReserveNumber = new System.Windows.Forms.TextBox();
            this.checkBoxAutoCleanFrameAnalysisFolder = new System.Windows.Forms.CheckBox();
            this.textBoxFrameAnalysisFolderReserveNumber = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.checkBox_AutoTextureOnlyConvertDiffuseMap = new System.Windows.Forms.CheckBox();
            this.checkBoxTopMost = new System.Windows.Forms.CheckBox();
            this.checkBoxShareSourceModel = new System.Windows.Forms.CheckBox();
            this.groupBoxCreditInfo = new System.Windows.Forms.GroupBox();
            this.textBoxAuthorLink = new System.Windows.Forms.TextBox();
            this.textBoxAuthor = new System.Windows.Forms.TextBox();
            this.labelAuthorLink = new System.Windows.Forms.Label();
            this.labelAuthor = new System.Windows.Forms.Label();
            this.checkBoxOpenModOutputFolder = new System.Windows.Forms.CheckBox();
            this.groupBoxCreditInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // checkBoxDeleteOutputFolder
            // 
            this.checkBoxDeleteOutputFolder.AutoSize = true;
            this.checkBoxDeleteOutputFolder.Location = new System.Drawing.Point(12, 56);
            this.checkBoxDeleteOutputFolder.Name = "checkBoxDeleteOutputFolder";
            this.checkBoxDeleteOutputFolder.Size = new System.Drawing.Size(294, 16);
            this.checkBoxDeleteOutputFolder.TabIndex = 56;
            this.checkBoxDeleteOutputFolder.Text = "Auto delete OutputFolder before extract model";
            this.checkBoxDeleteOutputFolder.UseVisualStyleBackColor = true;
            // 
            // checkBoxBackUp
            // 
            this.checkBoxBackUp.AutoSize = true;
            this.checkBoxBackUp.Location = new System.Drawing.Point(12, 78);
            this.checkBoxBackUp.Name = "checkBoxBackUp";
            this.checkBoxBackUp.Size = new System.Drawing.Size(372, 16);
            this.checkBoxBackUp.TabIndex = 58;
            this.checkBoxBackUp.Text = "Back up OutputFolder to Backups folder before generate mod";
            this.checkBoxBackUp.UseVisualStyleBackColor = true;
            // 
            // checkBoxAutoCleanLog
            // 
            this.checkBoxAutoCleanLog.AutoSize = true;
            this.checkBoxAutoCleanLog.Location = new System.Drawing.Point(12, 12);
            this.checkBoxAutoCleanLog.Name = "checkBoxAutoCleanLog";
            this.checkBoxAutoCleanLog.Size = new System.Drawing.Size(414, 16);
            this.checkBoxAutoCleanLog.TabIndex = 78;
            this.checkBoxAutoCleanLog.Text = "Clean log file under Logs folder before exit MMT, reserve number:";
            this.checkBoxAutoCleanLog.UseVisualStyleBackColor = true;
            this.checkBoxAutoCleanLog.CheckedChanged += new System.EventHandler(this.checkBoxAutoCleanLog_CheckedChanged);
            // 
            // textBoxLogReserveNumber
            // 
            this.textBoxLogReserveNumber.Location = new System.Drawing.Point(432, 7);
            this.textBoxLogReserveNumber.Name = "textBoxLogReserveNumber";
            this.textBoxLogReserveNumber.Size = new System.Drawing.Size(43, 21);
            this.textBoxLogReserveNumber.TabIndex = 79;
            // 
            // checkBoxAutoCleanFrameAnalysisFolder
            // 
            this.checkBoxAutoCleanFrameAnalysisFolder.AutoSize = true;
            this.checkBoxAutoCleanFrameAnalysisFolder.Location = new System.Drawing.Point(12, 34);
            this.checkBoxAutoCleanFrameAnalysisFolder.Name = "checkBoxAutoCleanFrameAnalysisFolder";
            this.checkBoxAutoCleanFrameAnalysisFolder.Size = new System.Drawing.Size(498, 16);
            this.checkBoxAutoCleanFrameAnalysisFolder.TabIndex = 80;
            this.checkBoxAutoCleanFrameAnalysisFolder.Text = "Clean FrameAnalysis Folder under Loader Folder before exit MMT, reserve number:";
            this.checkBoxAutoCleanFrameAnalysisFolder.UseVisualStyleBackColor = true;
            this.checkBoxAutoCleanFrameAnalysisFolder.CheckedChanged += new System.EventHandler(this.checkBoxAutoCleanFrameAnalysisFolder_CheckedChanged);
            // 
            // textBoxFrameAnalysisFolderReserveNumber
            // 
            this.textBoxFrameAnalysisFolderReserveNumber.Location = new System.Drawing.Point(516, 29);
            this.textBoxFrameAnalysisFolderReserveNumber.Name = "textBoxFrameAnalysisFolderReserveNumber";
            this.textBoxFrameAnalysisFolderReserveNumber.Size = new System.Drawing.Size(43, 21);
            this.textBoxFrameAnalysisFolderReserveNumber.TabIndex = 81;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // checkBox_AutoTextureOnlyConvertDiffuseMap
            // 
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.AutoSize = true;
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.Location = new System.Drawing.Point(12, 101);
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.Name = "checkBox_AutoTextureOnlyConvertDiffuseMap";
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.Size = new System.Drawing.Size(264, 16);
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.TabIndex = 82;
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.Text = "Auto Texture Only Convert DiffuseMap.dds";
            this.checkBox_AutoTextureOnlyConvertDiffuseMap.UseVisualStyleBackColor = true;
            // 
            // checkBoxTopMost
            // 
            this.checkBoxTopMost.AutoSize = true;
            this.checkBoxTopMost.Location = new System.Drawing.Point(12, 125);
            this.checkBoxTopMost.Name = "checkBoxTopMost";
            this.checkBoxTopMost.Size = new System.Drawing.Size(228, 16);
            this.checkBoxTopMost.TabIndex = 83;
            this.checkBoxTopMost.Text = "Always show MMT winow on top most.";
            this.checkBoxTopMost.UseVisualStyleBackColor = true;
            // 
            // checkBoxShareSourceModel
            // 
            this.checkBoxShareSourceModel.AutoSize = true;
            this.checkBoxShareSourceModel.Location = new System.Drawing.Point(12, 147);
            this.checkBoxShareSourceModel.Name = "checkBoxShareSourceModel";
            this.checkBoxShareSourceModel.Size = new System.Drawing.Size(378, 16);
            this.checkBoxShareSourceModel.TabIndex = 84;
            this.checkBoxShareSourceModel.Text = "Share exported Source Model of Mod when generate mod files.";
            this.checkBoxShareSourceModel.UseVisualStyleBackColor = true;
            // 
            // groupBoxCreditInfo
            // 
            this.groupBoxCreditInfo.Controls.Add(this.textBoxAuthorLink);
            this.groupBoxCreditInfo.Controls.Add(this.textBoxAuthor);
            this.groupBoxCreditInfo.Controls.Add(this.labelAuthorLink);
            this.groupBoxCreditInfo.Controls.Add(this.labelAuthor);
            this.groupBoxCreditInfo.Location = new System.Drawing.Point(12, 202);
            this.groupBoxCreditInfo.Name = "groupBoxCreditInfo";
            this.groupBoxCreditInfo.Size = new System.Drawing.Size(547, 81);
            this.groupBoxCreditInfo.TabIndex = 85;
            this.groupBoxCreditInfo.TabStop = false;
            this.groupBoxCreditInfo.Text = "Credit Info";
            // 
            // textBoxAuthorLink
            // 
            this.textBoxAuthorLink.Location = new System.Drawing.Point(64, 47);
            this.textBoxAuthorLink.Name = "textBoxAuthorLink";
            this.textBoxAuthorLink.Size = new System.Drawing.Size(468, 21);
            this.textBoxAuthorLink.TabIndex = 3;
            // 
            // textBoxAuthor
            // 
            this.textBoxAuthor.Location = new System.Drawing.Point(64, 20);
            this.textBoxAuthor.Name = "textBoxAuthor";
            this.textBoxAuthor.Size = new System.Drawing.Size(164, 21);
            this.textBoxAuthor.TabIndex = 2;
            // 
            // labelAuthorLink
            // 
            this.labelAuthorLink.AutoSize = true;
            this.labelAuthorLink.Location = new System.Drawing.Point(6, 50);
            this.labelAuthorLink.Name = "labelAuthorLink";
            this.labelAuthorLink.Size = new System.Drawing.Size(29, 12);
            this.labelAuthorLink.TabIndex = 1;
            this.labelAuthorLink.Text = "Link";
            // 
            // labelAuthor
            // 
            this.labelAuthor.AutoSize = true;
            this.labelAuthor.Location = new System.Drawing.Point(6, 23);
            this.labelAuthor.Name = "labelAuthor";
            this.labelAuthor.Size = new System.Drawing.Size(41, 12);
            this.labelAuthor.TabIndex = 0;
            this.labelAuthor.Text = "Author";
            // 
            // checkBoxOpenModOutputFolder
            // 
            this.checkBoxOpenModOutputFolder.AutoSize = true;
            this.checkBoxOpenModOutputFolder.Location = new System.Drawing.Point(12, 169);
            this.checkBoxOpenModOutputFolder.Name = "checkBoxOpenModOutputFolder";
            this.checkBoxOpenModOutputFolder.Size = new System.Drawing.Size(264, 16);
            this.checkBoxOpenModOutputFolder.TabIndex = 86;
            this.checkBoxOpenModOutputFolder.Text = "Open ModOutputFolder After Generate Mod.";
            this.checkBoxOpenModOutputFolder.UseVisualStyleBackColor = true;
            // 
            // ConfigGame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(571, 295);
            this.Controls.Add(this.checkBoxOpenModOutputFolder);
            this.Controls.Add(this.groupBoxCreditInfo);
            this.Controls.Add(this.checkBoxShareSourceModel);
            this.Controls.Add(this.checkBoxTopMost);
            this.Controls.Add(this.checkBox_AutoTextureOnlyConvertDiffuseMap);
            this.Controls.Add(this.textBoxFrameAnalysisFolderReserveNumber);
            this.Controls.Add(this.checkBoxAutoCleanFrameAnalysisFolder);
            this.Controls.Add(this.checkBoxAutoCleanLog);
            this.Controls.Add(this.textBoxLogReserveNumber);
            this.Controls.Add(this.checkBoxBackUp);
            this.Controls.Add(this.checkBoxDeleteOutputFolder);
            this.Name = "ConfigGame";
            this.Opacity = 0.97D;
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Preferences";
            this.TopMost = true;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ConfigGame_FormClosed);
            this.Load += new System.EventHandler(this.ConfigBasic_Load);
            this.groupBoxCreditInfo.ResumeLayout(false);
            this.groupBoxCreditInfo.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxDeleteOutputFolder;
        private System.Windows.Forms.CheckBox checkBoxBackUp;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.CheckBox checkBoxAutoCleanLog;
        private System.Windows.Forms.TextBox textBoxLogReserveNumber;
        private System.Windows.Forms.CheckBox checkBoxAutoCleanFrameAnalysisFolder;
        private System.Windows.Forms.TextBox textBoxFrameAnalysisFolderReserveNumber;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.CheckBox checkBox_AutoTextureOnlyConvertDiffuseMap;
        private System.Windows.Forms.CheckBox checkBoxTopMost;
        private System.Windows.Forms.CheckBox checkBoxShareSourceModel;
        private System.Windows.Forms.GroupBox groupBoxCreditInfo;
        private System.Windows.Forms.Label labelAuthorLink;
        private System.Windows.Forms.Label labelAuthor;
        private System.Windows.Forms.TextBox textBoxAuthorLink;
        private System.Windows.Forms.TextBox textBoxAuthor;
        private System.Windows.Forms.CheckBox checkBoxOpenModOutputFolder;
    }
}