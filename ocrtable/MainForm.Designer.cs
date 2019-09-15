namespace ocrtable
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.label1 = new System.Windows.Forms.Label();
            this.imgTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.docTextBox = new System.Windows.Forms.TextBox();
            this.goButton = new System.Windows.Forms.Button();
            this.exitButton = new System.Windows.Forms.Button();
            this.browseOpenButton = new System.Windows.Forms.Button();
            this.browseSaveButton = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.groupBox = new System.Windows.Forms.GroupBox();
            this.multilineCheckBox = new System.Windows.Forms.CheckBox();
            this.allOcrCheckBox = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.upDown = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.isMergedCheckBox = new System.Windows.Forms.CheckBox();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.backgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.chineseCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.upDown)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 11);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(185, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Please input image file path：";
            // 
            // imgTextBox
            // 
            this.imgTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.imgTextBox.Location = new System.Drawing.Point(14, 30);
            this.imgTextBox.Name = "imgTextBox";
            this.imgTextBox.Size = new System.Drawing.Size(282, 21);
            this.imgTextBox.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(191, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Please input result file path：";
            // 
            // docTextBox
            // 
            this.docTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.docTextBox.Location = new System.Drawing.Point(14, 78);
            this.docTextBox.Name = "docTextBox";
            this.docTextBox.Size = new System.Drawing.Size(282, 21);
            this.docTextBox.TabIndex = 4;
            // 
            // goButton
            // 
            this.goButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.goButton.Location = new System.Drawing.Point(362, 11);
            this.goButton.Name = "goButton";
            this.goButton.Size = new System.Drawing.Size(75, 23);
            this.goButton.TabIndex = 8;
            this.goButton.Text = "Run";
            this.goButton.UseVisualStyleBackColor = true;
            this.goButton.Click += new System.EventHandler(this.goButton_Click);
            // 
            // exitButton
            // 
            this.exitButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.exitButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.exitButton.Location = new System.Drawing.Point(362, 40);
            this.exitButton.Name = "exitButton";
            this.exitButton.Size = new System.Drawing.Size(75, 23);
            this.exitButton.TabIndex = 9;
            this.exitButton.Text = "Exit";
            this.exitButton.UseVisualStyleBackColor = true;
            this.exitButton.Click += new System.EventHandler(this.exitButton_Click);
            // 
            // browseOpenButton
            // 
            this.browseOpenButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.browseOpenButton.Location = new System.Drawing.Point(302, 26);
            this.browseOpenButton.Name = "browseOpenButton";
            this.browseOpenButton.Size = new System.Drawing.Size(33, 23);
            this.browseOpenButton.TabIndex = 2;
            this.browseOpenButton.Text = "...";
            this.browseOpenButton.UseVisualStyleBackColor = true;
            this.browseOpenButton.Click += new System.EventHandler(this.browseOpenButton_Click);
            // 
            // browseSaveButton
            // 
            this.browseSaveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.browseSaveButton.Location = new System.Drawing.Point(302, 78);
            this.browseSaveButton.Name = "browseSaveButton";
            this.browseSaveButton.Size = new System.Drawing.Size(33, 23);
            this.browseSaveButton.TabIndex = 5;
            this.browseSaveButton.Text = "...";
            this.browseSaveButton.UseVisualStyleBackColor = true;
            this.browseSaveButton.Click += new System.EventHandler(this.browseSaveButton_Click);
            // 
            // progressBar
            // 
            this.progressBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBar.Location = new System.Drawing.Point(12, 282);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(323, 23);
            this.progressBar.TabIndex = 7;
            // 
            // groupBox
            // 
            this.groupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox.Controls.Add(this.chineseCheckBox);
            this.groupBox.Controls.Add(this.multilineCheckBox);
            this.groupBox.Controls.Add(this.allOcrCheckBox);
            this.groupBox.Controls.Add(this.label4);
            this.groupBox.Controls.Add(this.upDown);
            this.groupBox.Controls.Add(this.label3);
            this.groupBox.Controls.Add(this.isMergedCheckBox);
            this.groupBox.Location = new System.Drawing.Point(14, 107);
            this.groupBox.Name = "groupBox";
            this.groupBox.Size = new System.Drawing.Size(321, 159);
            this.groupBox.TabIndex = 6;
            this.groupBox.TabStop = false;
            this.groupBox.Text = "Options";
            // 
            // multilineCheckBox
            // 
            this.multilineCheckBox.AutoSize = true;
            this.multilineCheckBox.Location = new System.Drawing.Point(20, 101);
            this.multilineCheckBox.Name = "multilineCheckBox";
            this.multilineCheckBox.Size = new System.Drawing.Size(228, 16);
            this.multilineCheckBox.TabIndex = 3;
            this.multilineCheckBox.Text = "Do not remove line breaks in cells";
            this.multilineCheckBox.UseVisualStyleBackColor = true;
            // 
            // allOcrCheckBox
            // 
            this.allOcrCheckBox.AutoSize = true;
            this.allOcrCheckBox.Checked = true;
            this.allOcrCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.allOcrCheckBox.Location = new System.Drawing.Point(20, 54);
            this.allOcrCheckBox.Name = "allOcrCheckBox";
            this.allOcrCheckBox.Size = new System.Drawing.Size(138, 16);
            this.allOcrCheckBox.TabIndex = 1;
            this.allOcrCheckBox.Text = "Recognize all texts";
            this.allOcrCheckBox.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(265, 124);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = "pixels";
            // 
            // upDown
            // 
            this.upDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.upDown.Location = new System.Drawing.Point(216, 121);
            this.upDown.Name = "upDown";
            this.upDown.Size = new System.Drawing.Size(43, 21);
            this.upDown.TabIndex = 5;
            this.upDown.Value = new decimal(new int[] {
            20,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 124);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(191, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Height of each line is at least";
            // 
            // isMergedCheckBox
            // 
            this.isMergedCheckBox.AutoSize = true;
            this.isMergedCheckBox.Checked = true;
            this.isMergedCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.isMergedCheckBox.Location = new System.Drawing.Point(20, 78);
            this.isMergedCheckBox.Name = "isMergedCheckBox";
            this.isMergedCheckBox.Size = new System.Drawing.Size(216, 16);
            this.isMergedCheckBox.TabIndex = 2;
            this.isMergedCheckBox.Text = "Recognize merged cells in tables";
            this.isMergedCheckBox.UseVisualStyleBackColor = true;
            // 
            // openFileDialog
            // 
            this.openFileDialog.Filter = "图像文件|*.BMP;*.PNG;*.JPG;*.JPEG;*.GIF;*.TIF;*.TIFF|所有文件 (*.*)|*.*";
            // 
            // saveFileDialog
            // 
            this.saveFileDialog.DefaultExt = "docx";
            this.saveFileDialog.Filter = "Word文档 (*.docx)|*.docx";
            // 
            // backgroundWorker
            // 
            this.backgroundWorker.WorkerReportsProgress = true;
            this.backgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_DoWork);
            this.backgroundWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_ProgressChanged);
            this.backgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker_RunWorkerCompleted);
            // 
            // chineseCheckBox
            // 
            this.chineseCheckBox.AutoSize = true;
            this.chineseCheckBox.Checked = true;
            this.chineseCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chineseCheckBox.Location = new System.Drawing.Point(20, 32);
            this.chineseCheckBox.Name = "chineseCheckBox";
            this.chineseCheckBox.Size = new System.Drawing.Size(258, 16);
            this.chineseCheckBox.TabIndex = 0;
            this.chineseCheckBox.Text = "Recognize simplified Chinese characters";
            this.chineseCheckBox.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.exitButton;
            this.ClientSize = new System.Drawing.Size(444, 312);
            this.Controls.Add(this.groupBox);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.browseSaveButton);
            this.Controls.Add(this.browseOpenButton);
            this.Controls.Add(this.exitButton);
            this.Controls.Add(this.goButton);
            this.Controls.Add(this.docTextBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.imgTextBox);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "OCR with Tables";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.groupBox.ResumeLayout(false);
            this.groupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.upDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox imgTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox docTextBox;
        private System.Windows.Forms.Button goButton;
        private System.Windows.Forms.Button exitButton;
        private System.Windows.Forms.Button browseOpenButton;
        private System.Windows.Forms.Button browseSaveButton;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.GroupBox groupBox;
        private System.Windows.Forms.CheckBox isMergedCheckBox;
        private System.Windows.Forms.CheckBox allOcrCheckBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown upDown;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog;
        private System.Windows.Forms.CheckBox multilineCheckBox;
        private System.ComponentModel.BackgroundWorker backgroundWorker;
        private System.Windows.Forms.CheckBox chineseCheckBox;
    }
}

