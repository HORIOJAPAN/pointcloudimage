using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.MemoryMappedFiles;
using System.IO;

namespace csPCIform
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MappingForm());
        }
    }


    class SharedMemory
    {
        String FILEMAPNAME;
        MemoryMappedFile mmf;

        public SharedMemory(String str)
        {
            FILEMAPNAME = str;
            mmf = MemoryMappedFile.CreateNew(FILEMAPNAME, 10000);
        }

        ~SharedMemory()
        {
            mmf.Dispose();
        }

        public string getShMemData(int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryReader reader = new BinaryReader(stream);
                for (int i = 0; i < offset; i++) reader.ReadString();
                return reader.ReadString();
            }
        }
        public void setShMemData(string data, int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryWriter writer = new BinaryWriter(stream);
                //writer.Seek(offset * sizeof(string), SeekOrigin.Begin);
                writer.Write(data);
            }
        }

    }


}
