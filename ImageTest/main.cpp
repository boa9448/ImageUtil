#include "../libs/imageUtil.h"

int main()
{
	cv::Mat src;
	cv::Mat temp;
	src = cv::imread("..\\imgs\\src.png");
	temp = cv::imread("..\\imgs\\temp.png");

	for (int idx = 0; idx < 10; idx++)
	{
		double t1 = cv::getTickCount();

		cv::Rect find;
		int result = image::ImageSearchEx(src, temp, find, EXCEPT_RED);
		cv::rectangle(src, find, cv::Scalar(0, 255, 0), 2);

		std::cout << (cv::getTickCount() - t1) / cv::getTickFrequency() << std::endl;
	}

	cv::imshow("view", src);
	cv::waitKey();
	cv::destroyAllWindows();

	return 0;
}