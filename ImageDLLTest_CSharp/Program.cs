using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Image;

namespace ImageDLLTest_CSharp
{
    class Program
    {
        static void ImageSearchExTest()
        {
            RECT find = new RECT();
            int nRet = Search.ImageSearchEx("..\\..\\..\\imgs\\src.png", "..\\..\\..\\imgs\\temp.png", ref find, 255);

            Console.WriteLine("find count : {0}", nRet);
            Console.WriteLine("x : {0}, y : {1}, w : {2}, h : {3}"
                , find.left, find.top, find.right - find.left, find.bottom - find.top);
        }
        
        static void ImageSearchEx_AllTest()
        {
            RECT[] find_list = new RECT[50];
            int nRet = Search.ImageSearchEx_All("..\\..\\..\\imgs\\src2.png", "..\\..\\..\\imgs\\temp2.png"
                , find_list, (uint)find_list.Length, 255);
            Console.WriteLine("find count : {0}", nRet);

            for(int idx = 0; idx < nRet; idx++)
            {
                Console.WriteLine("x : {0}, y : {1}, w : {2}, h : {3}"
                , find_list[idx].left, find_list[idx].top, find_list[idx].right - find_list[idx].left, find_list[idx].bottom - find_list[idx].top);
            }
        }


        static void Main(string[] args)
        {
            var value = System.Environment.GetEnvironmentVariable("PATH");
            value += ";E:\\source\\repos\\ImageUtil\\x64\\Debug";
            value += ";E:\\Library\\opencv\\install\\x64\\vc16\\bin";
            System.Environment.SetEnvironmentVariable("PATH", value);

            ImageSearchExTest();
            ImageSearchEx_AllTest();
        }
    }
}
