namespace NMBT_GUI
{
    partial class Main
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle6 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle7 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle8 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle5 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.menuStrip4 = new System.Windows.Forms.MenuStrip();
            this.Menu_File_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenModsFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_openDedupedFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenTypesFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_Setting_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_PreferencesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_SwitchLayoutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_languages_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.简体中文ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.englishToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_GameName_toolStripComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.Menu_RunToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBoxBasicMod = new System.Windows.Forms.GroupBox();
            this.menuStrip8 = new System.Windows.Forms.MenuStrip();
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DataGridView_BasicIBList = new System.Windows.Forms.DataGridView();
            this.ColumnDrawIB = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnConfig = new System.Windows.Forms.DataGridViewButtonColumn();
            this.ColumnStatus = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.button_GenerateMod = new System.Windows.Forms.Button();
            this.button_ExtractModel = new System.Windows.Forms.Button();
            this.groupBox_Analysis = new System.Windows.Forms.GroupBox();
            this.menuStrip3 = new System.Windows.Forms.MenuStrip();
            this.Menu_Analysis_SkipIB_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textBoxVertexShaderCheckList = new System.Windows.Forms.TextBox();
            this.label_Analysis_VSCheckList = new System.Windows.Forms.Label();
            this.label_Analysis_IBHashList = new System.Windows.Forms.Label();
            this.textBoxSkipIBList = new System.Windows.Forms.TextBox();
            this.groupBox_VertexShaderCheck = new System.Windows.Forms.GroupBox();
            this.dataGridViewShaderCheckList = new System.Windows.Forms.DataGridView();
            this.ColumnShaderHash = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnCheckList = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnForbiden = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.menuStrip6 = new System.Windows.Forms.MenuStrip();
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pageSetupDialog1 = new System.Windows.Forms.PageSetupDialog();
            this.menuStrip4.SuspendLayout();
            this.groupBoxBasicMod.SuspendLayout();
            this.menuStrip8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DataGridView_BasicIBList)).BeginInit();
            this.groupBox_Analysis.SuspendLayout();
            this.menuStrip3.SuspendLayout();
            this.groupBox_VertexShaderCheck.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewShaderCheckList)).BeginInit();
            this.menuStrip6.SuspendLayout();
            this.SuspendLayout();
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // menuStrip4
            // 
            this.menuStrip4.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.menuStrip4.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_File_ToolStripMenuItem,
            this.Menu_Setting_ToolStripMenuItem,
            this.Menu_languages_ToolStripMenuItem,
            this.Menu_GameName_toolStripComboBox,
            this.Menu_RunToolStripMenuItem});
            this.menuStrip4.Location = new System.Drawing.Point(0, 0);
            this.menuStrip4.Name = "menuStrip4";
            this.menuStrip4.Size = new System.Drawing.Size(587, 29);
            this.menuStrip4.TabIndex = 37;
            this.menuStrip4.Text = "menuStrip4";
            // 
            // Menu_File_ToolStripMenuItem
            // 
            this.Menu_File_ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem,
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem,
            this.Menu_File_OpenModsFolder_ToolStripMenuItem,
            this.toolStripSeparator7,
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem,
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem,
            this.Menu_File_openDedupedFolderToolStripMenuItem,
            this.toolStripSeparator6,
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem,
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem,
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem,
            this.Menu_File_OpenTypesFolderToolStripMenuItem,
            this.toolStripSeparator4,
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem,
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem,
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem,
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem,
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem});
            this.Menu_File_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.Menu_File_ToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.Menu_File_ToolStripMenuItem.Name = "Menu_File_ToolStripMenuItem";
            this.Menu_File_ToolStripMenuItem.Size = new System.Drawing.Size(42, 25);
            this.Menu_File_ToolStripMenuItem.Text = "File";
            // 
            // Menu_File_OpenOutputFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem.Name = "Menu_File_OpenOutputFolder_ToolStripMenuItem";
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem.Text = "Open OutputFolder";
            this.Menu_File_OpenOutputFolder_ToolStripMenuItem.Click += new System.EventHandler(this.File_打开Output文件夹ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenModGenerateFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem.Name = "Menu_File_OpenModGenerateFolder_ToolStripMenuItem";
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem.Text = "Open Mod Generate Folder";
            this.Menu_File_OpenModGenerateFolder_ToolStripMenuItem.Click += new System.EventHandler(this.File_打开二创模型输出文件夹ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenModsFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenModsFolder_ToolStripMenuItem.Name = "Menu_File_OpenModsFolder_ToolStripMenuItem";
            this.Menu_File_OpenModsFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenModsFolder_ToolStripMenuItem.Text = "Open Mods Folder";
            this.Menu_File_OpenModsFolder_ToolStripMenuItem.Click += new System.EventHandler(this.打开Mods目录ToolStripMenuItem_Click);
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(286, 6);
            // 
            // Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem.Name = "Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem";
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem.Text = "Open Latest FrameAnalysis Folder";
            this.Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem.Click += new System.EventHandler(this.打开最新的FrameAnalysis文件夹ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem
            // 
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem.Name = "Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem";
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem.Text = "Open Latest FrameAnalysis/log.txt";
            this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem.Click += new System.EventHandler(this.Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem_Click);
            // 
            // Menu_File_openDedupedFolderToolStripMenuItem
            // 
            this.Menu_File_openDedupedFolderToolStripMenuItem.Name = "Menu_File_openDedupedFolderToolStripMenuItem";
            this.Menu_File_openDedupedFolderToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_openDedupedFolderToolStripMenuItem.Text = "Open Deduped Folder";
            this.Menu_File_openDedupedFolderToolStripMenuItem.Click += new System.EventHandler(this.Menu_File_openDedupedFolderToolStripMenuItem_Click);
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(286, 6);
            // 
            // Menu_File_Open3DmigotoFolder_ToolStripMenuItem
            // 
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem.Name = "Menu_File_Open3DmigotoFolder_ToolStripMenuItem";
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem.Text = "Open 3Dmigoto Folder";
            this.Menu_File_Open3DmigotoFolder_ToolStripMenuItem.Click += new System.EventHandler(this.打开3Dmigoto文件夹ToolStripMenuItem_Click);
            // 
            // Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem
            // 
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem.Name = "Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem";
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem.Text = "Open 3Dmigoto\'s d3dx.ini";
            this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenShaderFixesFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem.Name = "Menu_File_OpenShaderFixesFolder_ToolStripMenuItem";
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem.Text = "Open ShaderFixes Folder";
            this.Menu_File_OpenShaderFixesFolder_ToolStripMenuItem.Click += new System.EventHandler(this.打开ShaderFixs文件夹ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenTypesFolderToolStripMenuItem
            // 
            this.Menu_File_OpenTypesFolderToolStripMenuItem.Name = "Menu_File_OpenTypesFolderToolStripMenuItem";
            this.Menu_File_OpenTypesFolderToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenTypesFolderToolStripMenuItem.Text = "Open ExtractTypes Folder";
            this.Menu_File_OpenTypesFolderToolStripMenuItem.Click += new System.EventHandler(this.openTypesFolderToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(286, 6);
            // 
            // Menu_File_OpenMMTLocationFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem.Name = "Menu_File_OpenMMTLocationFolder_ToolStripMenuItem";
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem.Text = "Open MMT\'s Location Folder";
            this.Menu_File_OpenMMTLocationFolder_ToolStripMenuItem.Click += new System.EventHandler(this.File_打开工具所在目录ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenLogsFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem.Name = "Menu_File_OpenLogsFolder_ToolStripMenuItem";
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem.Text = "Open Logs Folder";
            this.Menu_File_OpenLogsFolder_ToolStripMenuItem.Click += new System.EventHandler(this.打开Logs目录ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenLatestLogFile_ToolStripMenuItem
            // 
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem.Name = "Menu_File_OpenLatestLogFile_ToolStripMenuItem";
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem.Text = "Open Latest LOG File";
            this.Menu_File_OpenLatestLogFile_ToolStripMenuItem.Click += new System.EventHandler(this.openLatestLOGFileToolStripMenuItem_Click);
            // 
            // Menu_File_OpenBackupsFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem.Name = "Menu_File_OpenBackupsFolder_ToolStripMenuItem";
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem.Text = "Open Backups Folder";
            this.Menu_File_OpenBackupsFolder_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_File_OpenBackupsFolder_ToolStripMenuItem_Click);
            // 
            // Menu_File_OpenConfigsFolder_ToolStripMenuItem
            // 
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem.Name = "Menu_File_OpenConfigsFolder_ToolStripMenuItem";
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem.Size = new System.Drawing.Size(289, 22);
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem.Text = "Open Configs Folder";
            this.Menu_File_OpenConfigsFolder_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_File_OpenConfigsFolder_ToolStripMenuItem_Click);
            // 
            // Menu_Setting_ToolStripMenuItem
            // 
            this.Menu_Setting_ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_PreferencesToolStripMenuItem,
            this.Menu_SwitchLayoutToolStripMenuItem});
            this.Menu_Setting_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.Menu_Setting_ToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.Menu_Setting_ToolStripMenuItem.Name = "Menu_Setting_ToolStripMenuItem";
            this.Menu_Setting_ToolStripMenuItem.Size = new System.Drawing.Size(70, 25);
            this.Menu_Setting_ToolStripMenuItem.Text = "Settings";
            // 
            // Menu_PreferencesToolStripMenuItem
            // 
            this.Menu_PreferencesToolStripMenuItem.Name = "Menu_PreferencesToolStripMenuItem";
            this.Menu_PreferencesToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.Menu_PreferencesToolStripMenuItem.Text = "Preferences";
            this.Menu_PreferencesToolStripMenuItem.Click += new System.EventHandler(this.Menu_PreferencesToolStripMenuItem_Click);
            // 
            // Menu_SwitchLayoutToolStripMenuItem
            // 
            this.Menu_SwitchLayoutToolStripMenuItem.Name = "Menu_SwitchLayoutToolStripMenuItem";
            this.Menu_SwitchLayoutToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.Menu_SwitchLayoutToolStripMenuItem.Text = "Switch Layout";
            this.Menu_SwitchLayoutToolStripMenuItem.Click += new System.EventHandler(this.切换布局ToolStripMenuItem_Click);
            // 
            // Menu_languages_ToolStripMenuItem
            // 
            this.Menu_languages_ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.简体中文ToolStripMenuItem,
            this.englishToolStripMenuItem});
            this.Menu_languages_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_languages_ToolStripMenuItem.Name = "Menu_languages_ToolStripMenuItem";
            this.Menu_languages_ToolStripMenuItem.Size = new System.Drawing.Size(86, 25);
            this.Menu_languages_ToolStripMenuItem.Text = "Languages";
            // 
            // 简体中文ToolStripMenuItem
            // 
            this.简体中文ToolStripMenuItem.Name = "简体中文ToolStripMenuItem";
            this.简体中文ToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.简体中文ToolStripMenuItem.Text = "简体中文";
            this.简体中文ToolStripMenuItem.Click += new System.EventHandler(this.简体中文ToolStripMenuItem_Click);
            // 
            // englishToolStripMenuItem
            // 
            this.englishToolStripMenuItem.Name = "englishToolStripMenuItem";
            this.englishToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.englishToolStripMenuItem.Text = "English";
            this.englishToolStripMenuItem.Click += new System.EventHandler(this.englishToolStripMenuItem_Click);
            // 
            // Menu_GameName_toolStripComboBox
            // 
            this.Menu_GameName_toolStripComboBox.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.Menu_GameName_toolStripComboBox.Name = "Menu_GameName_toolStripComboBox";
            this.Menu_GameName_toolStripComboBox.Size = new System.Drawing.Size(100, 25);
            this.Menu_GameName_toolStripComboBox.TextChanged += new System.EventHandler(this.Menu_GameName_toolStripComboBox_TextChanged);
            // 
            // Menu_RunToolStripMenuItem
            // 
            this.Menu_RunToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem,
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem,
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem});
            this.Menu_RunToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_RunToolStripMenuItem.Name = "Menu_RunToolStripMenuItem";
            this.Menu_RunToolStripMenuItem.Size = new System.Drawing.Size(44, 25);
            this.Menu_RunToolStripMenuItem.Text = "Run";
            // 
            // Menu_Run_run3DmigotoLoaderexeToolStripMenuItem
            // 
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem.Name = "Menu_Run_run3DmigotoLoaderexeToolStripMenuItem";
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem.Size = new System.Drawing.Size(307, 22);
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem.Text = "Run 3Dmigoto Loader.exe";
            this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem.Click += new System.EventHandler(this.Menu_Run_run3DmigotoLoaderexeToolStripMenuItem_Click);
            // 
            // Menu_Run_run3DmigotoLoaderpyToolStripMenuItem
            // 
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem.Name = "Menu_Run_run3DmigotoLoaderpyToolStripMenuItem";
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem.Size = new System.Drawing.Size(307, 22);
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem.Text = "Run 3Dmigoto Loader.py";
            this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem.Click += new System.EventHandler(this.Menu_Run_run3DmigotoLoaderpyToolStripMenuItem_Click);
            // 
            // Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem
            // 
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem.Name = "Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem";
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem.Size = new System.Drawing.Size(307, 22);
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem.Text = "Run 3Dmigoto Loader-ByPassACE.exe";
            this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem.Click += new System.EventHandler(this.Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem_Click);
            // 
            // groupBoxBasicMod
            // 
            this.groupBoxBasicMod.BackColor = System.Drawing.Color.Transparent;
            this.groupBoxBasicMod.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.groupBoxBasicMod.Controls.Add(this.menuStrip8);
            this.groupBoxBasicMod.Controls.Add(this.DataGridView_BasicIBList);
            this.groupBoxBasicMod.Controls.Add(this.button_GenerateMod);
            this.groupBoxBasicMod.Controls.Add(this.button_ExtractModel);
            this.groupBoxBasicMod.ForeColor = System.Drawing.Color.Black;
            this.groupBoxBasicMod.Location = new System.Drawing.Point(3, 32);
            this.groupBoxBasicMod.Name = "groupBoxBasicMod";
            this.groupBoxBasicMod.Size = new System.Drawing.Size(581, 228);
            this.groupBoxBasicMod.TabIndex = 40;
            this.groupBoxBasicMod.TabStop = false;
            this.groupBoxBasicMod.Text = "Basic Mod";
            // 
            // menuStrip8
            // 
            this.menuStrip8.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.menuStrip8.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem,
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem});
            this.menuStrip8.Location = new System.Drawing.Point(3, 17);
            this.menuStrip8.Name = "menuStrip8";
            this.menuStrip8.Size = new System.Drawing.Size(575, 25);
            this.menuStrip8.TabIndex = 40;
            this.menuStrip8.Text = "menuStrip8";
            // 
            // Menu_BasicMod_InitializeConfig_ToolStripMenuItem
            // 
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.BackColor = System.Drawing.Color.Transparent;
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.Name = "Menu_BasicMod_InitializeConfig_ToolStripMenuItem";
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.Size = new System.Drawing.Size(118, 21);
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.Text = "Initialize Config";
            this.Menu_BasicMod_InitializeConfig_ToolStripMenuItem.Click += new System.EventHandler(this.DrawIB_清除当前配置ToolStripMenuItem_Click);
            // 
            // Menu_BasicMod_SaveConfig_ToolStripMenuItem
            // 
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.BackColor = System.Drawing.Color.Transparent;
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.ForeColor = System.Drawing.Color.Black;
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.Name = "Menu_BasicMod_SaveConfig_ToolStripMenuItem";
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.Size = new System.Drawing.Size(93, 21);
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.Text = "Save Config";
            this.Menu_BasicMod_SaveConfig_ToolStripMenuItem.Click += new System.EventHandler(this.DrawIB_保存配置ToolStripMenuItem_Click);
            // 
            // DataGridView_BasicIBList
            // 
            dataGridViewCellStyle1.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle1.ForeColor = System.Drawing.Color.Black;
            this.DataGridView_BasicIBList.AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this.DataGridView_BasicIBList.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.DataGridView_BasicIBList.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.DataGridView_BasicIBList.CellBorderStyle = System.Windows.Forms.DataGridViewCellBorderStyle.Raised;
            dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle2.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle2.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle2.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle2.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle2.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle2.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.DataGridView_BasicIBList.ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this.DataGridView_BasicIBList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.DataGridView_BasicIBList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColumnDrawIB,
            this.ColumnConfig,
            this.ColumnStatus});
            this.DataGridView_BasicIBList.Cursor = System.Windows.Forms.Cursors.Default;
            dataGridViewCellStyle6.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle6.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle6.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle6.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle6.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle6.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle6.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.DataGridView_BasicIBList.DefaultCellStyle = dataGridViewCellStyle6;
            this.DataGridView_BasicIBList.GridColor = System.Drawing.Color.White;
            this.DataGridView_BasicIBList.Location = new System.Drawing.Point(3, 47);
            this.DataGridView_BasicIBList.Name = "DataGridView_BasicIBList";
            dataGridViewCellStyle7.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle7.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle7.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            dataGridViewCellStyle7.ForeColor = System.Drawing.Color.Black;
            dataGridViewCellStyle7.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle7.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle7.WrapMode = System.Windows.Forms.DataGridViewTriState.True;
            this.DataGridView_BasicIBList.RowHeadersDefaultCellStyle = dataGridViewCellStyle7;
            dataGridViewCellStyle8.BackColor = System.Drawing.Color.White;
            dataGridViewCellStyle8.ForeColor = System.Drawing.Color.Black;
            this.DataGridView_BasicIBList.RowsDefaultCellStyle = dataGridViewCellStyle8;
            this.DataGridView_BasicIBList.Size = new System.Drawing.Size(374, 175);
            this.DataGridView_BasicIBList.TabIndex = 35;
            this.DataGridView_BasicIBList.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.DataGridViewIBList_CellContentClick);
            this.DataGridView_BasicIBList.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.DataGridViewIBList_CellValueChanged);
            // 
            // ColumnDrawIB
            // 
            dataGridViewCellStyle3.BackColor = System.Drawing.Color.Transparent;
            dataGridViewCellStyle3.ForeColor = System.Drawing.Color.Black;
            this.ColumnDrawIB.DefaultCellStyle = dataGridViewCellStyle3;
            this.ColumnDrawIB.HeaderText = "Draw IB";
            this.ColumnDrawIB.Name = "ColumnDrawIB";
            // 
            // ColumnConfig
            // 
            dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle4.BackColor = System.Drawing.Color.Transparent;
            dataGridViewCellStyle4.ForeColor = System.Drawing.Color.Black;
            this.ColumnConfig.DefaultCellStyle = dataGridViewCellStyle4;
            this.ColumnConfig.HeaderText = "Config";
            this.ColumnConfig.Name = "ColumnConfig";
            this.ColumnConfig.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            this.ColumnConfig.SortMode = System.Windows.Forms.DataGridViewColumnSortMode.Automatic;
            // 
            // ColumnStatus
            // 
            dataGridViewCellStyle5.BackColor = System.Drawing.Color.Transparent;
            dataGridViewCellStyle5.ForeColor = System.Drawing.Color.Black;
            this.ColumnStatus.DefaultCellStyle = dataGridViewCellStyle5;
            this.ColumnStatus.HeaderText = "Status";
            this.ColumnStatus.Name = "ColumnStatus";
            this.ColumnStatus.Resizable = System.Windows.Forms.DataGridViewTriState.True;
            // 
            // button_GenerateMod
            // 
            this.button_GenerateMod.BackColor = System.Drawing.Color.Transparent;
            this.button_GenerateMod.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.button_GenerateMod.Font = new System.Drawing.Font("宋体", 9F);
            this.button_GenerateMod.Location = new System.Drawing.Point(390, 114);
            this.button_GenerateMod.Name = "button_GenerateMod";
            this.button_GenerateMod.Size = new System.Drawing.Size(179, 50);
            this.button_GenerateMod.TabIndex = 38;
            this.button_GenerateMod.Text = "Generate Mod";
            this.button_GenerateMod.UseVisualStyleBackColor = false;
            this.button_GenerateMod.Click += new System.EventHandler(this.splitToolStripMenuItem_Click);
            // 
            // button_ExtractModel
            // 
            this.button_ExtractModel.BackColor = System.Drawing.Color.Transparent;
            this.button_ExtractModel.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.button_ExtractModel.Font = new System.Drawing.Font("宋体", 9F);
            this.button_ExtractModel.Location = new System.Drawing.Point(390, 47);
            this.button_ExtractModel.Name = "button_ExtractModel";
            this.button_ExtractModel.Size = new System.Drawing.Size(179, 50);
            this.button_ExtractModel.TabIndex = 37;
            this.button_ExtractModel.Text = "Extract Model";
            this.button_ExtractModel.UseVisualStyleBackColor = false;
            this.button_ExtractModel.Click += new System.EventHandler(this.mergeToolStripMenuItem_Click);
            // 
            // groupBox_Analysis
            // 
            this.groupBox_Analysis.BackColor = System.Drawing.Color.Transparent;
            this.groupBox_Analysis.Controls.Add(this.menuStrip3);
            this.groupBox_Analysis.Controls.Add(this.textBoxVertexShaderCheckList);
            this.groupBox_Analysis.Controls.Add(this.label_Analysis_VSCheckList);
            this.groupBox_Analysis.Controls.Add(this.label_Analysis_IBHashList);
            this.groupBox_Analysis.Controls.Add(this.textBoxSkipIBList);
            this.groupBox_Analysis.Location = new System.Drawing.Point(3, 266);
            this.groupBox_Analysis.Name = "groupBox_Analysis";
            this.groupBox_Analysis.Size = new System.Drawing.Size(581, 111);
            this.groupBox_Analysis.TabIndex = 42;
            this.groupBox_Analysis.TabStop = false;
            this.groupBox_Analysis.Text = "Analysis";
            // 
            // menuStrip3
            // 
            this.menuStrip3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.menuStrip3.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.menuStrip3.GripMargin = new System.Windows.Forms.Padding(1, 1, 0, 1);
            this.menuStrip3.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_Analysis_SkipIB_ToolStripMenuItem,
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem});
            this.menuStrip3.Location = new System.Drawing.Point(3, 17);
            this.menuStrip3.Name = "menuStrip3";
            this.menuStrip3.Size = new System.Drawing.Size(575, 25);
            this.menuStrip3.TabIndex = 9;
            this.menuStrip3.Text = "menuStrip3";
            // 
            // Menu_Analysis_SkipIB_ToolStripMenuItem
            // 
            this.Menu_Analysis_SkipIB_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_Analysis_SkipIB_ToolStripMenuItem.Name = "Menu_Analysis_SkipIB_ToolStripMenuItem";
            this.Menu_Analysis_SkipIB_ToolStripMenuItem.Size = new System.Drawing.Size(58, 21);
            this.Menu_Analysis_SkipIB_ToolStripMenuItem.Text = "SkipIB";
            this.Menu_Analysis_SkipIB_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_Analysis_SkipIB_ToolStripMenuItem_Click);
            // 
            // Menu_Analysis_VertexShaderCheck_ToolStripMenuItem
            // 
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem.Name = "Menu_Analysis_VertexShaderCheck_ToolStripMenuItem";
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem.Size = new System.Drawing.Size(76, 21);
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem.Text = "VS Check";
            this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_Analysis_VertexShaderCheck_ToolStripMenuItem_Click);
            // 
            // textBoxVertexShaderCheckList
            // 
            this.textBoxVertexShaderCheckList.Location = new System.Drawing.Point(149, 79);
            this.textBoxVertexShaderCheckList.Name = "textBoxVertexShaderCheckList";
            this.textBoxVertexShaderCheckList.Size = new System.Drawing.Size(426, 21);
            this.textBoxVertexShaderCheckList.TabIndex = 8;
            this.textBoxVertexShaderCheckList.Text = "ps-t7,ps-t8,ps-t9,ps-t10,vb1,ib";
            // 
            // label_Analysis_VSCheckList
            // 
            this.label_Analysis_VSCheckList.AutoSize = true;
            this.label_Analysis_VSCheckList.Location = new System.Drawing.Point(6, 82);
            this.label_Analysis_VSCheckList.Name = "label_Analysis_VSCheckList";
            this.label_Analysis_VSCheckList.Size = new System.Drawing.Size(143, 12);
            this.label_Analysis_VSCheckList.TabIndex = 7;
            this.label_Analysis_VSCheckList.Text = "VertexShader Check List";
            // 
            // label_Analysis_IBHashList
            // 
            this.label_Analysis_IBHashList.AutoSize = true;
            this.label_Analysis_IBHashList.Location = new System.Drawing.Point(6, 51);
            this.label_Analysis_IBHashList.Name = "label_Analysis_IBHashList";
            this.label_Analysis_IBHashList.Size = new System.Drawing.Size(137, 12);
            this.label_Analysis_IBHashList.TabIndex = 6;
            this.label_Analysis_IBHashList.Text = "Index Buffer Hash List";
            // 
            // textBoxSkipIBList
            // 
            this.textBoxSkipIBList.Location = new System.Drawing.Point(149, 48);
            this.textBoxSkipIBList.Name = "textBoxSkipIBList";
            this.textBoxSkipIBList.Size = new System.Drawing.Size(426, 21);
            this.textBoxSkipIBList.TabIndex = 5;
            // 
            // groupBox_VertexShaderCheck
            // 
            this.groupBox_VertexShaderCheck.Controls.Add(this.dataGridViewShaderCheckList);
            this.groupBox_VertexShaderCheck.Controls.Add(this.menuStrip6);
            this.groupBox_VertexShaderCheck.Location = new System.Drawing.Point(590, 32);
            this.groupBox_VertexShaderCheck.Name = "groupBox_VertexShaderCheck";
            this.groupBox_VertexShaderCheck.Size = new System.Drawing.Size(630, 345);
            this.groupBox_VertexShaderCheck.TabIndex = 43;
            this.groupBox_VertexShaderCheck.TabStop = false;
            this.groupBox_VertexShaderCheck.Text = "VertexShaderCheck";
            // 
            // dataGridViewShaderCheckList
            // 
            this.dataGridViewShaderCheckList.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(192)))), ((int)(((byte)(255)))));
            this.dataGridViewShaderCheckList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridViewShaderCheckList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColumnShaderHash,
            this.ColumnCheckList,
            this.ColumnForbiden});
            this.dataGridViewShaderCheckList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataGridViewShaderCheckList.Location = new System.Drawing.Point(3, 42);
            this.dataGridViewShaderCheckList.Name = "dataGridViewShaderCheckList";
            this.dataGridViewShaderCheckList.RowTemplate.Height = 23;
            this.dataGridViewShaderCheckList.Size = new System.Drawing.Size(624, 300);
            this.dataGridViewShaderCheckList.TabIndex = 3;
            // 
            // ColumnShaderHash
            // 
            this.ColumnShaderHash.HeaderText = "VertexShader Hash";
            this.ColumnShaderHash.Name = "ColumnShaderHash";
            this.ColumnShaderHash.Width = 130;
            // 
            // ColumnCheckList
            // 
            this.ColumnCheckList.HeaderText = "Slot Check List";
            this.ColumnCheckList.Name = "ColumnCheckList";
            this.ColumnCheckList.Width = 320;
            // 
            // ColumnForbiden
            // 
            this.ColumnForbiden.FalseValue = "False";
            this.ColumnForbiden.HeaderText = "Forbiden";
            this.ColumnForbiden.Name = "ColumnForbiden";
            this.ColumnForbiden.TrueValue = "True";
            this.ColumnForbiden.Width = 60;
            // 
            // menuStrip6
            // 
            this.menuStrip6.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.menuStrip6.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem,
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem,
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem,
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem,
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem,
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem,
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem});
            this.menuStrip6.Location = new System.Drawing.Point(3, 17);
            this.menuStrip6.Name = "menuStrip6";
            this.menuStrip6.Size = new System.Drawing.Size(624, 25);
            this.menuStrip6.TabIndex = 2;
            this.menuStrip6.Text = "menuStrip6";
            // 
            // Menu_VertexShaderCheck_initializeToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem.Name = "Menu_VertexShaderCheck_initializeToolStripMenuItem";
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem.Size = new System.Drawing.Size(73, 21);
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem.Text = "Initialize";
            this.Menu_VertexShaderCheck_initializeToolStripMenuItem.Click += new System.EventHandler(this.Menu_VertexShaderCheck_initializeToolStripMenuItem_Click);
            // 
            // Menu_VertexShaderCheck_Import_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_Import_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem.Size = new System.Drawing.Size(62, 21);
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem.Text = "Import";
            this.Menu_VertexShaderCheck_Import_ToolStripMenuItem.Click += new System.EventHandler(this.ShaderCheck_importToolStripMenuItem_Click);
            // 
            // Menu_VertexShaderCheck_Save_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_Save_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem.Size = new System.Drawing.Size(48, 21);
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem.Text = "Save";
            this.Menu_VertexShaderCheck_Save_ToolStripMenuItem.Click += new System.EventHandler(this.ShaderCheck_saveToolStripMenuItem1_Click);
            // 
            // Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem.Size = new System.Drawing.Size(74, 21);
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem.Text = "Save as...";
            this.Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem.Click += new System.EventHandler(this.ShaderCheck_saveAsToolStripMenuItem_Click);
            // 
            // Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold);
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem.Size = new System.Drawing.Size(183, 21);
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem.Text = "Generate ShaderCheck File";
            this.Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem.Click += new System.EventHandler(this.ShaderCheck_generateToolStripMenuItem_Click);
            // 
            // Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem.Size = new System.Drawing.Size(111, 21);
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem.Text = "View Relations";
            this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem.Click += new System.EventHandler(this.Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem_Click);
            // 
            // Menu_VertexShaderCheck_Update_ToolStripMenuItem
            // 
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem.Name = "Menu_VertexShaderCheck_Update_ToolStripMenuItem";
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem.Size = new System.Drawing.Size(64, 21);
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem.Text = "Update";
            this.Menu_VertexShaderCheck_Update_ToolStripMenuItem.Click += new System.EventHandler(this.ShaderCheck_updateToolStripMenuItem_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(587, 262);
            this.Controls.Add(this.groupBox_VertexShaderCheck);
            this.Controls.Add(this.groupBox_Analysis);
            this.Controls.Add(this.menuStrip4);
            this.Controls.Add(this.groupBoxBasicMod);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main";
            this.Opacity = 0.97D;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MMT";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.Load += new System.EventHandler(this.Main_Load);
            this.menuStrip4.ResumeLayout(false);
            this.menuStrip4.PerformLayout();
            this.groupBoxBasicMod.ResumeLayout(false);
            this.groupBoxBasicMod.PerformLayout();
            this.menuStrip8.ResumeLayout(false);
            this.menuStrip8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.DataGridView_BasicIBList)).EndInit();
            this.groupBox_Analysis.ResumeLayout(false);
            this.groupBox_Analysis.PerformLayout();
            this.menuStrip3.ResumeLayout(false);
            this.menuStrip3.PerformLayout();
            this.groupBox_VertexShaderCheck.ResumeLayout(false);
            this.groupBox_VertexShaderCheck.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridViewShaderCheckList)).EndInit();
            this.menuStrip6.ResumeLayout(false);
            this.menuStrip6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.MenuStrip menuStrip4;
        private System.Windows.Forms.ToolStripMenuItem Menu_Setting_ToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBoxBasicMod;
        private System.Windows.Forms.MenuStrip menuStrip8;
        private System.Windows.Forms.ToolStripMenuItem Menu_BasicMod_SaveConfig_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_BasicMod_InitializeConfig_ToolStripMenuItem;
        private System.Windows.Forms.DataGridView DataGridView_BasicIBList;
        private System.Windows.Forms.Button button_GenerateMod;
        private System.Windows.Forms.Button button_ExtractModel;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenOutputFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenModGenerateFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenMMTLocationFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenModsFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenLatestFrameAnalysisFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_Open3DmigotoFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenShaderFixesFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenLogsFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenLatestLogFile_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem Menu_languages_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 简体中文ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem englishToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenBackupsFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenConfigsFolder_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_Open3DmigotosD3dxini_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripComboBox Menu_GameName_toolStripComboBox;
        private System.Windows.Forms.ToolStripMenuItem Menu_RunToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_Run_run3DmigotoLoaderexeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_Run_run3DmigotoLoaderByPassACEexeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenTypesFolderToolStripMenuItem;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnDrawIB;
        private System.Windows.Forms.DataGridViewButtonColumn ColumnConfig;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnStatus;
        private System.Windows.Forms.GroupBox groupBox_Analysis;
        private System.Windows.Forms.MenuStrip menuStrip3;
        private System.Windows.Forms.ToolStripMenuItem Menu_Analysis_SkipIB_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_Analysis_VertexShaderCheck_ToolStripMenuItem;
        private System.Windows.Forms.TextBox textBoxVertexShaderCheckList;
        private System.Windows.Forms.Label label_Analysis_VSCheckList;
        private System.Windows.Forms.Label label_Analysis_IBHashList;
        private System.Windows.Forms.TextBox textBoxSkipIBList;
        private System.Windows.Forms.GroupBox groupBox_VertexShaderCheck;
        private System.Windows.Forms.DataGridView dataGridViewShaderCheckList;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnShaderHash;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnCheckList;
        private System.Windows.Forms.DataGridViewCheckBoxColumn ColumnForbiden;
        private System.Windows.Forms.MenuStrip menuStrip6;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_initializeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_Import_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_Save_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_SaveAs_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_Update_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_GenerateVertexShaderCheckFile_ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_VertexShaderCheck_ViewRelations_ToolStripMenuItem;
        private System.Windows.Forms.PageSetupDialog pageSetupDialog1;
        private System.Windows.Forms.ToolStripMenuItem Menu_PreferencesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_Run_run3DmigotoLoaderpyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_OpenLatestFrameAnalysislogtxtToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem Menu_SwitchLayoutToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator7;
        private System.Windows.Forms.ToolStripMenuItem Menu_File_openDedupedFolderToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator6;
    }
}

