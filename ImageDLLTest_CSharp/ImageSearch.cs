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

    [StructLayout(LayoutKind.Sequential)]
    public struct IMAGE
    {
        UIntPtr data;
        Int32 width;
        Int32 height;
        Int32 channels;
    }

    class Search
    {

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx")]
        public static extern int ImageSearchEx([MarshalAs(UnmanagedType.LPWStr)]string src_path, [MarshalAs(UnmanagedType.LPWStr)]string temp_path
            , ref RECT find , UInt32 except_color);

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx_All")]
        public static extern int ImageSearchEx_All([MarshalAs(UnmanagedType.LPWStr)]string src_path, [MarshalAs(UnmanagedType.LPWStr)]string temp_path
            , [In, Out]RECT[] find_list, uint find_size , UInt32 except_color);

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ClearImageMap")]
        public static extern void ClearImageMap();

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx_Raw")]
        public static extern int ImageSearchEx_Raw(ref IMAGE src, ref IMAGE temp, ref RECT find, UInt32 except_color);

        [DllImport("ImageDLL.dll", SetLastError = true, EntryPoint = "ImageSearchEx_All")]
        public static extern int ImageSearchEx_Raw_All(ref IMAGE src, ref IMAGE temp, [In, Out] RECT[] find_list, uint find_size, UInt32 except_color);

        public static UInt32 RGB(byte r, byte g, byte b)
        {
            return (UInt32)(r | (g << 8) | (b << 16));
        }
    }
}
