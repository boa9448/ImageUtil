#pragma comment(lib, "ImageDLL.lib")

#include <iostream>
#include <chrono>
#include "../ImageDLL/ImageDLL.h"

void ImageSearchExTest()
{
	LPCWSTR src_path = L"..\\imgs\\src.png";
	LPCWSTR temp_path = L"..\\imgs\\temp.png";

	auto start = std::chrono::high_resolution_clock::now();

	RECT find = { 0, };
	int result = ImageSearchEx(src_path, temp_path, &find, RGB(255, 0, 0));

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "search time : " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

	std::cout << "find count : " << result << std::endl;
}

void ImageSearchExAllTest()
{
	auto start = std::chrono::high_resolution_clock::now();

	RECT find_list[50];
	int result = ImageSearchEx_All(L"..\\imgs\\src2.png", L"..\\imgs\\temp2.png", find_list, 50, RGB(255, 0, 0));

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "search time : " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl;

	std::cout << "find count : " << result << std::endl;
	for (int idx = 0; idx < result; idx++)
	{
		std::cout << find_list[idx].left << ", " << find_list[idx].top << ", " 
			<< find_list[idx].right << ", " << find_list[idx].bottom << ", " << std::endl;
	}
}

int main()
{
	for (int idx = 0; idx < 10; idx++)
	{
		std::cout << "=============================================================" << std::endl;
		ImageSearchExTest();
		ImageSearchExAllTest();
	}
}