using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Image
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RECT
    {
        public Int32 left;
        public Int32 top;
        public Int32 right;
        public Int32 bottom;
    }

    class Search
    {

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx")]
        public static extern int ImageSearchEx([MarshalAs(UnmanagedType.LPWStr)]string src_path, [MarshalAs(UnmanagedType.LPWStr)]string temp_path
            , ref RECT find , uint except_color);

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx_All")]
        public static extern int ImageSearchEx_All([MarshalAs(UnmanagedType.LPWStr)]string src_path, [MarshalAs(UnmanagedType.LPWStr)]string temp_path
            , [In, Out]RECT[] find_list, uint find_size , uint except_color);

        [DllImport("ClearImageMap", SetLastError = true, EntryPoint = "ClearImageMap")]
        public static extern void ClearImageMap();
    }
}
