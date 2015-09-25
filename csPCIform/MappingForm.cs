using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.Ports;
using System.Diagnostics;
using System.Threading;

namespace csPCIform
{
    public partial class MappingForm : Form
    {

        bool isRunning;
        SharedMemoryInt shMemInt;
        SharedMemoryChar shMemChar;

        public MappingForm()
        {
            InitializeComponent();
            isRunning = false;
            shMemInt = new SharedMemoryInt("MappingFormInt");
            shMemChar = new SharedMemoryChar("MappingFormChar");
        }

        private void COMportNameSet()
        {
            //! 利用可能なシリアルポート名の配列を取得する.
            string[] PortList = SerialPort.GetPortNames();

            urg1comCbbox.Items.Clear();
            urg2comCbbox.Items.Clear();
            arduinocomCbbox.Items.Clear();

            //! シリアルポート名をコンボボックスにセットする.
            foreach (string PortName in PortList)
            {
                urg1comCbbox.Items.Add(PortName);
                urg2comCbbox.Items.Add(PortName);
                arduinocomCbbox.Items.Add(PortName);
            }
            if (urg1comCbbox.Items.Count > 0)
            {
                urg1comCbbox.SelectedIndex = 0;
                urg2comCbbox.SelectedIndex = 0;
                arduinocomCbbox.SelectedIndex = 0;
            }
        }

        private void MappingForm_Load(object sender, EventArgs e)
        {
            COMportNameSet();

        }

        private void reloadBtn_Click(object sender, EventArgs e)
        {
            COMportNameSet();
        }

        private void startBtn_Click(object sender, EventArgs e)
        {

            if (!isRunning)
            {
                // 終了の合図用メモリを初期化しておく
                shMemInt.setShMemData(0);

                // 以下プロセスの起動系
                Process myProcess = new Process();
                //引数
                String args =
                    //URG1のCOM，URG2のCOM，ArduinoのCOM
                    urg1comCbbox.Text.ToString().Substring(3) + " " + urg2comCbbox.Text.ToString().Substring(3) + " " + arduinocomCbbox.Text.ToString().Substring(3) + " "
                    //URG1の高さと距離
                    + urg1heightTxtbox.Text.ToString() + " " + urg1distanceTxtbox.Text.ToString() + " "
                    //URG2の高さと距離
                    + urg2heightTxtbox.Text.ToString() + " " + urg2distanceTxtbox.Text.ToString() + " "
                    //画像の幅，高さ，解像度
                    + imgWidthTxtbox.Text.ToString() + " " + imgHeightTxtbox.Text.ToString() + " " + imgResolutionTxtbox.Text.ToString();

                myProcess.StartInfo.FileName = "../../../Debug\\cvPCDtest.exe";
                myProcess.StartInfo.Arguments = args;
                myProcess.StartInfo.WindowStyle = ProcessWindowStyle.Normal;
                myProcess.Start();

                isRunning = true;
                startBtn.Text = "Stop";
            }
            else
            {
                shMemInt.setShMemData(1);
                string dirname1 = "", dirname2 = "";

                for (int i = 0; i < shMemInt.getShMemData(1);i++ )
                {
                    dirname1 += shMemChar.getShMemData(i);
                }
                for (int i = 0; i < shMemInt.getShMemData(2); i++)
                {
                    dirname2 += shMemChar.getShMemData(i + shMemInt.getShMemData(1));
                }

                dirListBox.Items.Add(dirname1);
                dirListBox.Items.Add(dirname2);
                isRunning = false;
                startBtn.Text = "Start";
            }
        }

        private void replaceBtn_Click(object sender, EventArgs e)
        {
            String tmp;
            tmp = urg1heightTxtbox.Text.ToString();
            urg1heightTxtbox.Text = urg2heightTxtbox.Text;
            urg2heightTxtbox.Text = tmp;

            tmp = urg1distanceTxtbox.Text.ToString();
            urg1distanceTxtbox.Text = urg2distanceTxtbox.Text;
            urg2distanceTxtbox.Text = tmp;

        }

    }
}
