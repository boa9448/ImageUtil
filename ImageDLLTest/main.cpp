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
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

	std::cout << "find count : " << result << std::endl;
}

void ImageSearchExAllTest()
{
	auto start = std::chrono::high_resolution_clock::now();

	RECT find_list[50];
	int result = ImageSearchEx_All(L"..\\imgs\\src.png", L"..\\imgs\\temp.png", find_list, 50, RGB(255, 0, 0));

	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

	std::cout << "find count : " << result << std::endl;
}

int main()
{
	ImageSearchExTest();
}