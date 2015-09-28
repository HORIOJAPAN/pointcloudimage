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

    // Int型の共有メモリを扱うクラス
    class SharedMemoryInt
    {
        String FILEMAPNAME;
        MemoryMappedFile mmf;

        public SharedMemoryInt(String str)
        {
            FILEMAPNAME = str;
            mmf = MemoryMappedFile.CreateNew(FILEMAPNAME, 10000);
        }

        ~SharedMemoryInt()
        {
            mmf.Dispose();
        }

        public Int32 getShMemData(int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryReader reader = new BinaryReader(stream);
                for (int i = 0; i < offset; i++) reader.ReadInt32();
                return reader.ReadInt32();
            }
        }
        public void setShMemData(Int32 data, int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryWriter writer = new BinaryWriter(stream);
                writer.Seek(offset * sizeof(Int32), SeekOrigin.Begin);
                writer.Write(data);
            }
        }

    }
    class SharedMemoryChar
    {
        String FILEMAPNAME;
        MemoryMappedFile mmf;

        public SharedMemoryChar(String str)
        {
            FILEMAPNAME = str;
            mmf = MemoryMappedFile.CreateNew(FILEMAPNAME, 10000);
        }

        ~SharedMemoryChar()
        {
            mmf.Dispose();
        }

        public Int32 getShMemData(int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryReader reader = new BinaryReader(stream);
                for (int i = 0; i < offset; i++) reader.ReadChar();
                return reader.ReadChar();
            }
        }
        public void setShMemData(char data, int offset = 0)
        {
            using (MemoryMappedViewStream stream = mmf.CreateViewStream())
            {
                BinaryWriter writer = new BinaryWriter(stream);
                writer.Seek(offset * sizeof(char), SeekOrigin.Begin);
                writer.Write(data);
            }
        }

    }


}
