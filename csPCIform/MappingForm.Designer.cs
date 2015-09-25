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
            this.label1 = new System.Windows.Forms.Label();
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
            this.SuspendLayout();
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(327, 401);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(190, 60);
            this.startBtn.TabIndex = 0;
            this.startBtn.Text = "Start";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // dirListBox
            // 
            this.dirListBox.FormattingEnabled = true;
            this.dirListBox.ItemHeight = 18;
            this.dirListBox.Location = new System.Drawing.Point(574, 60);
            this.dirListBox.Name = "dirListBox";
            this.dirListBox.Size = new System.Drawing.Size(370, 526);
            this.dirListBox.TabIndex = 2;
            // 
            // imgWidthTxtbox
            // 
            this.imgWidthTxtbox.Location = new System.Drawing.Point(39, 329);
            this.imgWidthTxtbox.Name = "imgWidthTxtbox";
            this.imgWidthTxtbox.Size = new System.Drawing.Size(100, 25);
            this.imgWidthTxtbox.TabIndex = 3;
            this.imgWidthTxtbox.Text = "5000";
            // 
            // imgHeightTxtbox
            // 
            this.imgHeightTxtbox.Location = new System.Drawing.Point(171, 329);
            this.imgHeightTxtbox.Name = "imgHeightTxtbox";
            this.imgHeightTxtbox.Size = new System.Drawing.Size(100, 25);
            this.imgHeightTxtbox.TabIndex = 4;
            this.imgHeightTxtbox.Text = "5000";
            // 
            // imgWidthLbl
            // 
            this.imgWidthLbl.AutoSize = true;
            this.imgWidthLbl.Location = new System.Drawing.Point(36, 297);
            this.imgWidthLbl.Name = "imgWidthLbl";
            this.imgWidthLbl.Size = new System.Drawing.Size(92, 18);
            this.imgWidthLbl.TabIndex = 5;
            this.imgWidthLbl.Text = "ImageWidth";
            // 
            // imgHeightLbl
            // 
            this.imgHeightLbl.AutoSize = true;
            this.imgHeightLbl.Location = new System.Drawing.Point(168, 297);
            this.imgHeightLbl.Name = "imgHeightLbl";
            this.imgHeightLbl.Size = new System.Drawing.Size(99, 18);
            this.imgHeightLbl.TabIndex = 6;
            this.imgHeightLbl.Text = "ImageHeight";
            // 
            // imgResolutionLbl
            // 
            this.imgResolutionLbl.AutoSize = true;
            this.imgResolutionLbl.Location = new System.Drawing.Point(36, 372);
            this.imgResolutionLbl.Name = "imgResolutionLbl";
            this.imgResolutionLbl.Size = new System.Drawing.Size(129, 18);
            this.imgResolutionLbl.TabIndex = 8;
            this.imgResolutionLbl.Text = "ImageResolution";
            // 
            // imgResolutionTxtbox
            // 
            this.imgResolutionTxtbox.Location = new System.Drawing.Point(39, 404);
            this.imgResolutionTxtbox.Name = "imgResolutionTxtbox";
            this.imgResolutionTxtbox.Size = new System.Drawing.Size(100, 25);
            this.imgResolutionTxtbox.TabIndex = 7;
            this.imgResolutionTxtbox.Text = "5";
            // 
            // urg1comCbbox
            // 
            this.urg1comCbbox.FormattingEnabled = true;
            this.urg1comCbbox.Location = new System.Drawing.Point(36, 60);
            this.urg1comCbbox.Name = "urg1comCbbox";
            this.urg1comCbbox.Size = new System.Drawing.Size(121, 26);
            this.urg1comCbbox.TabIndex = 9;
            // 
            // urg1Lbl
            // 
            this.urg1Lbl.AutoSize = true;
            this.urg1Lbl.Location = new System.Drawing.Point(33, 39);
            this.urg1Lbl.Name = "urg1Lbl";
            this.urg1Lbl.Size = new System.Drawing.Size(52, 18);
            this.urg1Lbl.TabIndex = 10;
            this.urg1Lbl.Text = "URG1";
            // 
            // urg2Lbl
            // 
            this.urg2Lbl.AutoSize = true;
            this.urg2Lbl.Location = new System.Drawing.Point(219, 39);
            this.urg2Lbl.Name = "urg2Lbl";
            this.urg2Lbl.Size = new System.Drawing.Size(52, 18);
            this.urg2Lbl.TabIndex = 12;
            this.urg2Lbl.Text = "URG2";
            // 
            // urg2comCbbox
            // 
            this.urg2comCbbox.FormattingEnabled = true;
            this.urg2comCbbox.Location = new System.Drawing.Point(222, 60);
            this.urg2comCbbox.Name = "urg2comCbbox";
            this.urg2comCbbox.Size = new System.Drawing.Size(121, 26);
            this.urg2comCbbox.TabIndex = 11;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(393, 39);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 18);
            this.label1.TabIndex = 14;
            this.label1.Text = "Arduino";
            // 
            // arduinocomCbbox
            // 
            this.arduinocomCbbox.FormattingEnabled = true;
            this.arduinocomCbbox.Location = new System.Drawing.Point(396, 60);
            this.arduinocomCbbox.Name = "arduinocomCbbox";
            this.arduinocomCbbox.Size = new System.Drawing.Size(121, 26);
            this.arduinocomCbbox.TabIndex = 13;
            // 
            // urg1heightTxtbox
            // 
            this.urg1heightTxtbox.Location = new System.Drawing.Point(36, 134);
            this.urg1heightTxtbox.Name = "urg1heightTxtbox";
            this.urg1heightTxtbox.Size = new System.Drawing.Size(100, 25);
            this.urg1heightTxtbox.TabIndex = 15;
            this.urg1heightTxtbox.Text = "1100.0";
            // 
            // urg1heightLbl
            // 
            this.urg1heightLbl.AutoSize = true;
            this.urg1heightLbl.Location = new System.Drawing.Point(33, 113);
            this.urg1heightLbl.Name = "urg1heightLbl";
            this.urg1heightLbl.Size = new System.Drawing.Size(116, 18);
            this.urg1heightLbl.TabIndex = 16;
            this.urg1heightLbl.Text = "地面からの高さ";
            // 
            // urg1distanceLbl
            // 
            this.urg1distanceLbl.AutoSize = true;
            this.urg1distanceLbl.Location = new System.Drawing.Point(33, 170);
            this.urg1distanceLbl.Name = "urg1distanceLbl";
            this.urg1distanceLbl.Size = new System.Drawing.Size(158, 18);
            this.urg1distanceLbl.TabIndex = 18;
            this.urg1distanceLbl.Text = "基準位置からの距離";
            // 
            // urg1distanceTxtbox
            // 
            this.urg1distanceTxtbox.Location = new System.Drawing.Point(36, 191);
            this.urg1distanceTxtbox.Name = "urg1distanceTxtbox";
            this.urg1distanceTxtbox.Size = new System.Drawing.Size(100, 25);
            this.urg1distanceTxtbox.TabIndex = 17;
            this.urg1distanceTxtbox.Text = "285.0";
            // 
            // urg2distanceTxtbox
            // 
            this.urg2distanceTxtbox.Location = new System.Drawing.Point(222, 191);
            this.urg2distanceTxtbox.Name = "urg2distanceTxtbox";
            this.urg2distanceTxtbox.Size = new System.Drawing.Size(100, 25);
            this.urg2distanceTxtbox.TabIndex = 21;
            this.urg2distanceTxtbox.Text = "443.0";
            // 
            // urg2heightTxtbox
            // 
            this.urg2heightTxtbox.Location = new System.Drawing.Point(222, 134);
            this.urg2heightTxtbox.Name = "urg2heightTxtbox";
            this.urg2heightTxtbox.Size = new System.Drawing.Size(100, 25);
            this.urg2heightTxtbox.TabIndex = 19;
            this.urg2heightTxtbox.Text = "20.0";
            // 
            // urg2heightLbl
            // 
            this.urg2heightLbl.AutoSize = true;
            this.urg2heightLbl.Location = new System.Drawing.Point(219, 113);
            this.urg2heightLbl.Name = "urg2heightLbl";
            this.urg2heightLbl.Size = new System.Drawing.Size(116, 18);
            this.urg2heightLbl.TabIndex = 22;
            this.urg2heightLbl.Text = "地面からの高さ";
            // 
            // urg2distanceLbl
            // 
            this.urg2distanceLbl.AutoSize = true;
            this.urg2distanceLbl.Location = new System.Drawing.Point(219, 170);
            this.urg2distanceLbl.Name = "urg2distanceLbl";
            this.urg2distanceLbl.Size = new System.Drawing.Size(158, 18);
            this.urg2distanceLbl.TabIndex = 23;
            this.urg2distanceLbl.Text = "基準位置からの距離";
            // 
            // reloadBtn
            // 
            this.reloadBtn.Location = new System.Drawing.Point(405, 113);
            this.reloadBtn.Name = "reloadBtn";
            this.reloadBtn.Size = new System.Drawing.Size(112, 40);
            this.reloadBtn.TabIndex = 24;
            this.reloadBtn.Text = "Reload";
            this.reloadBtn.UseVisualStyleBackColor = true;
            this.reloadBtn.Click += new System.EventHandler(this.reloadBtn_Click);
            // 
            // replaceBtn
            // 
            this.replaceBtn.Location = new System.Drawing.Point(134, 222);
            this.replaceBtn.Name = "replaceBtn";
            this.replaceBtn.Size = new System.Drawing.Size(93, 33);
            this.replaceBtn.TabIndex = 25;
            this.replaceBtn.Text = "<=>";
            this.replaceBtn.UseVisualStyleBackColor = true;
            this.replaceBtn.Click += new System.EventHandler(this.replaceBtn_Click);
            // 
            // MappingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(966, 602);
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
            this.Controls.Add(this.label1);
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
        private System.Windows.Forms.Label label1;
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
    }
}

