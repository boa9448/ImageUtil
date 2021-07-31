#include "../libs/imageUtil.h"

void ImageSearchExTest()
{
	cv::Mat src = cv::imread("..\\imgs\\src.png");
	cv::Mat temp = cv::imread("..\\imgs\\temp.png");

	double t1 = cv::getTickCount();

	cv::Rect find;
	int result = image::ImageSearchEx(src, temp, find, EXCEPT_RED);
	cv::rectangle(src, find, cv::Scalar(0, 255, 0), 2);

	std::cout << (cv::getTickCount() - t1) / cv::getTickFrequency() << std::endl;

	cv::imshow("view", src);
	cv::waitKey();
	cv::destroyAllWindows();
}

void ImageSearchExAllTest()
{
	double t1 = cv::getTickCount();

	RECT find_list[50];
	int result = ImageSearchEx_All(L"..\\imgs\\src.png", L"..\\imgs\\temp.png", find_list, 50, EXCEPT_RED);

	std::cout << (cv::getTickCount() - t1) / cv::getTickFrequency() << std::endl;
}

int main()
{
	ImageSearchExAllTest();

	return 0;
}