#include <windows.h>
#include <ppl.h>
#include <vector>
#include "opencv2/opencv.hpp"

#define OPENCV_VERSION CVAUX_STR(CV_VERSION_MAJOR)"" CVAUX_STR(CV_VERSION_MINOR)"" CVAUX_STR(CV_VERSION_REVISION)
#ifdef _DEBUG
#pragma comment(lib, "opencv_world" OPENCV_VERSION "d.lib")
#else
#pragma comment(lib, "opencv_world" OPENCV_VERSION ".lib")
#endif

#define EXCEPT_WHITE							RGB(255,255,255)
#define EXCEPT_RED								RGB(255,0,0)
#define EXCEPT_GREEN							RGB(0,255,0)
#define EXCEPT_BLUE								RGB(0,0,255)


#define IMAGE_EMPTY								(INT)-1
#define IMAGE_SIZE_ERROR						(INT)-2
#define IMAGE_SEARCH_INTERNAL_EXCEPTION			(INT)-3
#define IMAGE_SIZE_MISMATCH

namespace image
{

	cv::Mat hWndToMat(HWND hWnd, INT x = 0, INT y = 0, INT nWidth = 0, INT nHeigh = 0);
	COLORREF mGetPixel(cv::Mat& Image, UINT x, UINT y, INT nChannels);
	VOID mSetPixel(cv::Mat& Image, INT x, INT y, COLORREF Color, INT nChannels);
	INT ImageSearchEx(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rRect, COLORREF ExceptColor);
	INT ImageSearchEx(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, COLORREF ExceptColor);
	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rFindRect, COLORREF ExceptColor);
	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, COLORREF ExceptColor);
	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rResult, FLOAT fRate, COLORREF ExceptColor);
	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, FLOAT fRate, COLORREF ExceptColor);
}