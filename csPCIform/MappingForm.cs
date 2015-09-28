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
using System.Management;

namespace csPCIform
{
    public partial class MappingForm : Form
    {
        // もろもろの変数
        private bool            isRunning;
        private SharedMemoryInt shMemInt;
        private string          saveDirectoryName;
        private string[]        directories;


        public MappingForm()
        {
            InitializeComponent();
            isRunning = false;
            shMemInt = new SharedMemoryInt("MappingFormInt");

            saveDirectoryName = "../Debug";
            directories = System.IO.Directory.GetDirectories(saveDirectoryName, "*");
            int index;
            // パスを除去してディレクトリ名のみにする
            for (int i = 0; i < directories.Length; i++)
            {
                index = directories[i].LastIndexOf("\\");
                directories[i] = directories[i].Substring(index + 1);
            }
        }

        private string[] GetDeviceNames()
        {
            var deviceNameList = new System.Collections.ArrayList();
            var check = new System.Text.RegularExpressions.Regex("(COM[1-9][0-9]?[0-9]?)");

            ManagementClass device = new ManagementClass("Win32_SerialPort");
            ManagementObjectCollection manageObjCol = device.GetInstances();

            //全てのデバイスを探索しシリアル通信が行われるデバイスを随時追加する
            foreach (ManagementObject manageObj in manageObjCol)
            {
                //Nameプロパティを取得
                var namePropertyValue = manageObj.GetPropertyValue("Name");
                if (namePropertyValue == null)
                {
                    continue;
                }

                //Nameプロパティ文字列の一部が"(COM1)～(COM999)"と一致するときリストに追加"
                string name = namePropertyValue.ToString();
                //MessageBox.Show(name);
                if (check.IsMatch(name))
                {
                    deviceNameList.Add(name);
                }
            }

            //戻り値作成
            if (deviceNameList.Count > 0)
            {
                string[] deviceNames = new string[deviceNameList.Count];
                int index = 0;
                foreach (var name in deviceNameList)
                {
                    deviceNames[index++] = name.ToString();
                }
                return deviceNames;
            }
            else
            {
                return null;
            }
        }

        // COMポートの一覧をセットする
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

        // ディレクトリ名のリストボックスを更新
        private void addDirectoryName( ListBox listbox )
        {
            // ListBoxの内容をいったん破棄
            listbox.Items.Clear();

            // ディレクトリの一覧を取得してListBoxに結果を表示する
            string[] subFolders = System.IO.Directory.GetDirectories(saveDirectoryName, "*");
            int index;
            // パスを除去してディレクトリ名のみにする
            for(int i = 0 ; i < subFolders.Length ; i++ )
            {
                index = subFolders[i].LastIndexOf("\\");
                subFolders[i] = subFolders[i].Substring(index + 1);
            }

            // 全ディレクトリ名を表示するかどうか
            if (alldirCheckbox.Checked)
            {
                listbox.Items.AddRange(subFolders);
            }
            else
            {
                // 増えたディレクトリ名を検索して差分のみ追加
                int newDirNum = subFolders.Length - directories.Length ; // 増えたディレクトリ数
                string[] newDirName = new string[newDirNum];            // ディレクトリ名を追加する配列を増分だけ確保
                for(int i = subFolders.Length - 1 ; newDirNum != 0 ; i--)
                {
                    if(Array.IndexOf(directories,subFolders[i]) == -1)
                    {
                        newDirName[newDirNum - 1] = subFolders[i];
                        newDirNum--;
                    }
                }
                listbox.Items.AddRange(newDirName);
            }
                        
        }

        private void MappingForm_Load(object sender, EventArgs e)
        {
            COMportNameSet();

            addDirectoryName( dirListBox );
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
                //Mappingプロセスに終了の合図を送る
                shMemInt.setShMemData(1);

                addDirectoryName(dirListBox);

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
        
        // 選択した項目のディレクトリをエクスプローラで開く
        private void dirListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (dirListBox.SelectedIndex == -1 ) return;
            Process.Start("EXPLORER.EXE", System.IO.Path.GetFullPath(saveDirectoryName + "\\" + dirListBox.Text));
            dirListBox.ClearSelected();
        }

        // チェックボックスが変化したらListBoxも更新
        private void alldirCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            addDirectoryName(dirListBox);
        }

        private void devmngerBtn_Click(object sender, EventArgs e)
        {
            Process.Start("devmgmt.msc");
        }

    }
}
