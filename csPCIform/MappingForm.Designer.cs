namespace csPCIform
{
    partial class MappingForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MappingForm));
            this.startBtn = new System.Windows.Forms.Button();
            this.dirListBox = new System.Windows.Forms.ListBox();
            this.imgWidthTxtbox = new System.Windows.Forms.TextBox();
            this.imgHeightTxtbox = new System.Windows.Forms.TextBox();
            this.imgWidthLbl = new System.Windows.Forms.Label();
            this.imgHeightLbl = new System.Windows.Forms.Label();
            this.imgResolutionLbl = new System.Windows.Forms.Label();
            this.imgResolutionTxtbox = new System.Windows.Forms.TextBox();
            this.urg1comCbbox = new System.Windows.Forms.ComboBox();
            this.urg1Lbl = new System.Windows.Forms.Label();
            this.urg2Lbl = new System.Windows.Forms.Label();
            this.urg2comCbbox = new System.Windows.Forms.ComboBox();
            this.arduinocomLbl = new System.Windows.Forms.Label();
            this.arduinocomCbbox = new System.Windows.Forms.ComboBox();
            this.urg1heightTxtbox = new System.Windows.Forms.TextBox();
            this.urg1heightLbl = new System.Windows.Forms.Label();
            this.urg1distanceLbl = new System.Windows.Forms.Label();
            this.urg1distanceTxtbox = new System.Windows.Forms.TextBox();
            this.urg2distanceTxtbox = new System.Windows.Forms.TextBox();
            this.urg2heightTxtbox = new System.Windows.Forms.TextBox();
            this.urg2heightLbl = new System.Windows.Forms.Label();
            this.urg2distanceLbl = new System.Windows.Forms.Label();
            this.reloadBtn = new System.Windows.Forms.Button();
            this.replaceBtn = new System.Windows.Forms.Button();
            this.alldirCheckbox = new System.Windows.Forms.CheckBox();
            this.devmngerBtn = new System.Windows.Forms.Button();
            this.savedirpathTxtbox = new System.Windows.Forms.TextBox();
            this.savadirpathBtn = new System.Windows.Forms.Button();
            this.savedirpathDlog = new System.Windows.Forms.FolderBrowserDialog();
            this.intervalLbl = new System.Windows.Forms.Label();
            this.intervalTxtbox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(199, 351);
            this.startBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(114, 40);
            this.startBtn.TabIndex = 0;
            this.startBtn.Text = "Start";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // dirListBox
            // 
            this.dirListBox.FormattingEnabled = true;
            this.dirListBox.ItemHeight = 12;
            this.dirListBox.Location = new System.Drawing.Point(344, 52);
            this.dirListBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.dirListBox.Name = "dirListBox";
            this.dirListBox.Size = new System.Drawing.Size(224, 340);
            this.dirListBox.TabIndex = 2;
            this.dirListBox.SelectedIndexChanged += new System.EventHandler(this.dirListBox_SelectedIndexChanged);
            // 
            // imgWidthTxtbox
            // 
            this.imgWidthTxtbox.Location = new System.Drawing.Point(24, 313);
            this.imgWidthTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.imgWidthTxtbox.Name = "imgWidthTxtbox";
            this.imgWidthTxtbox.Size = new System.Drawing.Size(62, 19);
            this.imgWidthTxtbox.TabIndex = 3;
            this.imgWidthTxtbox.Text = "1000";
            // 
            // imgHeightTxtbox
            // 
            this.imgHeightTxtbox.Location = new System.Drawing.Point(103, 313);
            this.imgHeightTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.imgHeightTxtbox.Name = "imgHeightTxtbox";
            this.imgHeightTxtbox.Size = new System.Drawing.Size(62, 19);
            this.imgHeightTxtbox.TabIndex = 4;
            this.imgHeightTxtbox.Text = "1000";
            // 
            // imgWidthLbl
            // 
            this.imgWidthLbl.AutoSize = true;
            this.imgWidthLbl.Location = new System.Drawing.Point(22, 291);
            this.imgWidthLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.imgWidthLbl.Name = "imgWidthLbl";
            this.imgWidthLbl.Size = new System.Drawing.Size(63, 12);
            this.imgWidthLbl.TabIndex = 5;
            this.imgWidthLbl.Text = "ImageWidth";
            // 
            // imgHeightLbl
            // 
            this.imgHeightLbl.AutoSize = true;
            this.imgHeightLbl.Location = new System.Drawing.Point(101, 291);
            this.imgHeightLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.imgHeightLbl.Name = "imgHeightLbl";
            this.imgHeightLbl.Size = new System.Drawing.Size(68, 12);
            this.imgHeightLbl.TabIndex = 6;
            this.imgHeightLbl.Text = "ImageHeight";
            // 
            // imgResolutionLbl
            // 
            this.imgResolutionLbl.AutoSize = true;
            this.imgResolutionLbl.Location = new System.Drawing.Point(22, 341);
            this.imgResolutionLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.imgResolutionLbl.Name = "imgResolutionLbl";
            this.imgResolutionLbl.Size = new System.Drawing.Size(89, 12);
            this.imgResolutionLbl.TabIndex = 8;
            this.imgResolutionLbl.Text = "ImageResolution";
            // 
            // imgResolutionTxtbox
            // 
            this.imgResolutionTxtbox.Location = new System.Drawing.Point(24, 363);
            this.imgResolutionTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.imgResolutionTxtbox.Name = "imgResolutionTxtbox";
            this.imgResolutionTxtbox.Size = new System.Drawing.Size(62, 19);
            this.imgResolutionTxtbox.TabIndex = 7;
            this.imgResolutionTxtbox.Text = "5";
            // 
            // urg1comCbbox
            // 
            this.urg1comCbbox.FormattingEnabled = true;
            this.urg1comCbbox.Location = new System.Drawing.Point(24, 64);
            this.urg1comCbbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg1comCbbox.Name = "urg1comCbbox";
            this.urg1comCbbox.Size = new System.Drawing.Size(74, 20);
            this.urg1comCbbox.TabIndex = 9;
            // 
            // urg1Lbl
            // 
            this.urg1Lbl.AutoSize = true;
            this.urg1Lbl.Location = new System.Drawing.Point(22, 50);
            this.urg1Lbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg1Lbl.Name = "urg1Lbl";
            this.urg1Lbl.Size = new System.Drawing.Size(35, 12);
            this.urg1Lbl.TabIndex = 10;
            this.urg1Lbl.Text = "URG1";
            // 
            // urg2Lbl
            // 
            this.urg2Lbl.AutoSize = true;
            this.urg2Lbl.Location = new System.Drawing.Point(134, 50);
            this.urg2Lbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg2Lbl.Name = "urg2Lbl";
            this.urg2Lbl.Size = new System.Drawing.Size(35, 12);
            this.urg2Lbl.TabIndex = 12;
            this.urg2Lbl.Text = "URG2";
            // 
            // urg2comCbbox
            // 
            this.urg2comCbbox.FormattingEnabled = true;
            this.urg2comCbbox.Location = new System.Drawing.Point(136, 64);
            this.urg2comCbbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg2comCbbox.Name = "urg2comCbbox";
            this.urg2comCbbox.Size = new System.Drawing.Size(74, 20);
            this.urg2comCbbox.TabIndex = 11;
            // 
            // arduinocomLbl
            // 
            this.arduinocomLbl.AutoSize = true;
            this.arduinocomLbl.Location = new System.Drawing.Point(238, 50);
            this.arduinocomLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.arduinocomLbl.Name = "arduinocomLbl";
            this.arduinocomLbl.Size = new System.Drawing.Size(44, 12);
            this.arduinocomLbl.TabIndex = 14;
            this.arduinocomLbl.Text = "Arduino";
            // 
            // arduinocomCbbox
            // 
            this.arduinocomCbbox.FormattingEnabled = true;
            this.arduinocomCbbox.Location = new System.Drawing.Point(240, 64);
            this.arduinocomCbbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.arduinocomCbbox.Name = "arduinocomCbbox";
            this.arduinocomCbbox.Size = new System.Drawing.Size(74, 20);
            this.arduinocomCbbox.TabIndex = 13;
            // 
            // urg1heightTxtbox
            // 
            this.urg1heightTxtbox.Location = new System.Drawing.Point(24, 113);
            this.urg1heightTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg1heightTxtbox.Name = "urg1heightTxtbox";
            this.urg1heightTxtbox.Size = new System.Drawing.Size(62, 19);
            this.urg1heightTxtbox.TabIndex = 15;
            this.urg1heightTxtbox.Text = "1100.0";
            // 
            // urg1heightLbl
            // 
            this.urg1heightLbl.AutoSize = true;
            this.urg1heightLbl.Location = new System.Drawing.Point(22, 99);
            this.urg1heightLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg1heightLbl.Name = "urg1heightLbl";
            this.urg1heightLbl.Size = new System.Drawing.Size(77, 12);
            this.urg1heightLbl.TabIndex = 16;
            this.urg1heightLbl.Text = "地面からの高さ";
            // 
            // urg1distanceLbl
            // 
            this.urg1distanceLbl.AutoSize = true;
            this.urg1distanceLbl.Location = new System.Drawing.Point(22, 137);
            this.urg1distanceLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg1distanceLbl.Name = "urg1distanceLbl";
            this.urg1distanceLbl.Size = new System.Drawing.Size(105, 12);
            this.urg1distanceLbl.TabIndex = 18;
            this.urg1distanceLbl.Text = "基準位置からの距離";
            // 
            // urg1distanceTxtbox
            // 
            this.urg1distanceTxtbox.Location = new System.Drawing.Point(24, 151);
            this.urg1distanceTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg1distanceTxtbox.Name = "urg1distanceTxtbox";
            this.urg1distanceTxtbox.Size = new System.Drawing.Size(62, 19);
            this.urg1distanceTxtbox.TabIndex = 17;
            this.urg1distanceTxtbox.Text = "285.0";
            // 
            // urg2distanceTxtbox
            // 
            this.urg2distanceTxtbox.Location = new System.Drawing.Point(136, 151);
            this.urg2distanceTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg2distanceTxtbox.Name = "urg2distanceTxtbox";
            this.urg2distanceTxtbox.Size = new System.Drawing.Size(62, 19);
            this.urg2distanceTxtbox.TabIndex = 21;
            this.urg2distanceTxtbox.Text = "443.0";
            // 
            // urg2heightTxtbox
            // 
            this.urg2heightTxtbox.Location = new System.Drawing.Point(136, 113);
            this.urg2heightTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.urg2heightTxtbox.Name = "urg2heightTxtbox";
            this.urg2heightTxtbox.Size = new System.Drawing.Size(62, 19);
            this.urg2heightTxtbox.TabIndex = 19;
            this.urg2heightTxtbox.Text = "20.0";
            // 
            // urg2heightLbl
            // 
            this.urg2heightLbl.AutoSize = true;
            this.urg2heightLbl.Location = new System.Drawing.Point(134, 99);
            this.urg2heightLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg2heightLbl.Name = "urg2heightLbl";
            this.urg2heightLbl.Size = new System.Drawing.Size(77, 12);
            this.urg2heightLbl.TabIndex = 22;
            this.urg2heightLbl.Text = "地面からの高さ";
            // 
            // urg2distanceLbl
            // 
            this.urg2distanceLbl.AutoSize = true;
            this.urg2distanceLbl.Location = new System.Drawing.Point(134, 137);
            this.urg2distanceLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.urg2distanceLbl.Name = "urg2distanceLbl";
            this.urg2distanceLbl.Size = new System.Drawing.Size(105, 12);
            this.urg2distanceLbl.TabIndex = 23;
            this.urg2distanceLbl.Text = "基準位置からの距離";
            // 
            // reloadBtn
            // 
            this.reloadBtn.Location = new System.Drawing.Point(245, 99);
            this.reloadBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.reloadBtn.Name = "reloadBtn";
            this.reloadBtn.Size = new System.Drawing.Size(67, 27);
            this.reloadBtn.TabIndex = 24;
            this.reloadBtn.Text = "Reload";
            this.reloadBtn.UseVisualStyleBackColor = true;
            this.reloadBtn.Click += new System.EventHandler(this.reloadBtn_Click);
            // 
            // replaceBtn
            // 
            this.replaceBtn.Location = new System.Drawing.Point(83, 172);
            this.replaceBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.replaceBtn.Name = "replaceBtn";
            this.replaceBtn.Size = new System.Drawing.Size(56, 22);
            this.replaceBtn.TabIndex = 25;
            this.replaceBtn.Text = "<=>";
            this.replaceBtn.UseVisualStyleBackColor = true;
            this.replaceBtn.Click += new System.EventHandler(this.replaceBtn_Click);
            // 
            // alldirCheckbox
            // 
            this.alldirCheckbox.AutoSize = true;
            this.alldirCheckbox.Location = new System.Drawing.Point(484, 33);
            this.alldirCheckbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.alldirCheckbox.Name = "alldirCheckbox";
            this.alldirCheckbox.Size = new System.Drawing.Size(96, 16);
            this.alldirCheckbox.TabIndex = 26;
            this.alldirCheckbox.Text = "All directories";
            this.alldirCheckbox.UseVisualStyleBackColor = true;
            this.alldirCheckbox.CheckedChanged += new System.EventHandler(this.alldirCheckbox_CheckedChanged);
            // 
            // devmngerBtn
            // 
            this.devmngerBtn.Location = new System.Drawing.Point(7, 8);
            this.devmngerBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.devmngerBtn.Name = "devmngerBtn";
            this.devmngerBtn.Size = new System.Drawing.Size(94, 23);
            this.devmngerBtn.TabIndex = 27;
            this.devmngerBtn.Text = "DeviceManager";
            this.devmngerBtn.UseVisualStyleBackColor = true;
            this.devmngerBtn.Click += new System.EventHandler(this.devmngerBtn_Click);
            // 
            // savedirpathTxtbox
            // 
            this.savedirpathTxtbox.BackColor = System.Drawing.SystemColors.Control;
            this.savedirpathTxtbox.Location = new System.Drawing.Point(344, 11);
            this.savedirpathTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.savedirpathTxtbox.Multiline = true;
            this.savedirpathTxtbox.Name = "savedirpathTxtbox";
            this.savedirpathTxtbox.Size = new System.Drawing.Size(201, 19);
            this.savedirpathTxtbox.TabIndex = 28;
            // 
            // savadirpathBtn
            // 
            this.savadirpathBtn.Location = new System.Drawing.Point(548, 10);
            this.savadirpathBtn.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.savadirpathBtn.Name = "savadirpathBtn";
            this.savadirpathBtn.Size = new System.Drawing.Size(19, 19);
            this.savadirpathBtn.TabIndex = 29;
            this.savadirpathBtn.Text = "...";
            this.savadirpathBtn.UseVisualStyleBackColor = true;
            this.savadirpathBtn.Click += new System.EventHandler(this.savadirpathBtn_Click);
            // 
            // intervalLbl
            // 
            this.intervalLbl.AutoSize = true;
            this.intervalLbl.Location = new System.Drawing.Point(22, 213);
            this.intervalLbl.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.intervalLbl.Name = "intervalLbl";
            this.intervalLbl.Size = new System.Drawing.Size(90, 12);
            this.intervalLbl.TabIndex = 31;
            this.intervalLbl.Text = "Interval[millisec]";
            // 
            // intervalTxtbox
            // 
            this.intervalTxtbox.Location = new System.Drawing.Point(24, 234);
            this.intervalTxtbox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.intervalTxtbox.Name = "intervalTxtbox";
            this.intervalTxtbox.Size = new System.Drawing.Size(62, 19);
            this.intervalTxtbox.TabIndex = 30;
            this.intervalTxtbox.Text = "20";
            this.intervalTxtbox.TextChanged += new System.EventHandler(this.intervalTxtbox_TextChanged);
            // 
            // MappingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(580, 401);
            this.Controls.Add(this.intervalLbl);
            this.Controls.Add(this.intervalTxtbox);
            this.Controls.Add(this.savadirpathBtn);
            this.Controls.Add(this.savedirpathTxtbox);
            this.Controls.Add(this.devmngerBtn);
            this.Controls.Add(this.alldirCheckbox);
            this.Controls.Add(this.replaceBtn);
            this.Controls.Add(this.reloadBtn);
            this.Controls.Add(this.urg2distanceLbl);
            this.Controls.Add(this.urg2heightLbl);
            this.Controls.Add(this.urg2distanceTxtbox);
            this.Controls.Add(this.urg2heightTxtbox);
            this.Controls.Add(this.urg1distanceLbl);
            this.Controls.Add(this.urg1distanceTxtbox);
            this.Controls.Add(this.urg1heightLbl);
            this.Controls.Add(this.urg1heightTxtbox);
            this.Controls.Add(this.arduinocomLbl);
            this.Controls.Add(this.arduinocomCbbox);
            this.Controls.Add(this.urg2Lbl);
            this.Controls.Add(this.urg2comCbbox);
            this.Controls.Add(this.urg1Lbl);
            this.Controls.Add(this.urg1comCbbox);
            this.Controls.Add(this.imgResolutionLbl);
            this.Controls.Add(this.imgResolutionTxtbox);
            this.Controls.Add(this.imgHeightLbl);
            this.Controls.Add(this.imgWidthLbl);
            this.Controls.Add(this.imgHeightTxtbox);
            this.Controls.Add(this.imgWidthTxtbox);
            this.Controls.Add(this.dirListBox);
            this.Controls.Add(this.startBtn);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "MappingForm";
            this.Text = "MappingForm";
            this.Load += new System.EventHandler(this.MappingForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.ListBox dirListBox;
        private System.Windows.Forms.TextBox imgWidthTxtbox;
        private System.Windows.Forms.TextBox imgHeightTxtbox;
        private System.Windows.Forms.Label imgWidthLbl;
        private System.Windows.Forms.Label imgHeightLbl;
        private System.Windows.Forms.Label imgResolutionLbl;
        private System.Windows.Forms.TextBox imgResolutionTxtbox;
        private System.Windows.Forms.ComboBox urg1comCbbox;
        private System.Windows.Forms.Label urg1Lbl;
        private System.Windows.Forms.Label urg2Lbl;
        private System.Windows.Forms.ComboBox urg2comCbbox;
        private System.Windows.Forms.Label arduinocomLbl;
        private System.Windows.Forms.ComboBox arduinocomCbbox;
        private System.Windows.Forms.TextBox urg1heightTxtbox;
        private System.Windows.Forms.Label urg1heightLbl;
        private System.Windows.Forms.Label urg1distanceLbl;
        private System.Windows.Forms.TextBox urg1distanceTxtbox;
        private System.Windows.Forms.TextBox urg2distanceTxtbox;
        private System.Windows.Forms.TextBox urg2heightTxtbox;
        private System.Windows.Forms.Label urg2heightLbl;
        private System.Windows.Forms.Label urg2distanceLbl;
        private System.Windows.Forms.Button reloadBtn;
        private System.Windows.Forms.Button replaceBtn;
        private System.Windows.Forms.CheckBox alldirCheckbox;
        private System.Windows.Forms.Button devmngerBtn;
        private System.Windows.Forms.TextBox savedirpathTxtbox;
        private System.Windows.Forms.Button savadirpathBtn;
        private System.Windows.Forms.FolderBrowserDialog savedirpathDlog;
        private System.Windows.Forms.Label intervalLbl;
        private System.Windows.Forms.TextBox intervalTxtbox;
    }
}

