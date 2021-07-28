using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ImageDLLTest_CSharp
{
    class Program
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public Int32 left;
            public Int32 top;
            public Int32 right;
            public Int32 bottom;
        }

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx")]
        public static extern int ImageSearchEx([MarshalAs(UnmanagedType.LPWStr)]string src_path, [MarshalAs(UnmanagedType.LPWStr)]string temp_path
            , ref RECT find , ulong except_color);
        static void Main(string[] args)
        {
            RECT find = new RECT();
            int nRet = ImageSearchEx("..\\..\\..\\imgs\\src.png", "..\\..\\..\\imgs\\temp.png", ref find, 255);

            Console.WriteLine(nRet);
        }
    }
}
