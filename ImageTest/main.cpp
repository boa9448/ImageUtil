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
	cv::Mat src = cv::imread("..\\imgs\\src.png");
	cv::Mat temp = cv::imread("..\\imgs\\temp.png");

	double t1 = cv::getTickCount();

	std::vector<cv::Rect> find;
	int result = image::ImageSearchEx(src, temp, find, EXCEPT_RED);

	std::cout << (cv::getTickCount() - t1) / cv::getTickFrequency() << std::endl;

	for(int idx = 0; idx < result; idx++)
		cv::rectangle(src, find[idx], cv::Scalar(0, 255, 0), 2);

	cv::imshow("view", src);
	cv::waitKey();
	cv::destroyAllWindows();
}

int main()
{
	ImageSearchExTest();
	ImageSearchExAllTest();

	return 0;
}