// ImageDLL.cpp : DLL을 위해 내보낸 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ImageDLL.h"
#include "../libs/imageUtil.h"

// 내보낸 변수의 예제입니다.
IMAGEDLL_API int nImageDLL=0;

// 내보낸 함수의 예제입니다.
IMAGEDLL_API int fnImageDLL(void)
{
    return 0;
}

// 내보낸 클래스의 생성자입니다.
CImageDLL::CImageDLL()
{
    return;
}


//=============================================================================
//헬퍼 함수임 애들 나중에 삭제해야함
//=============================================================================
std::string UnicodeToAnsi(LPCWSTR lpszString, UINT nCodePage)
{
	INT nCount = 0;
	CHAR szTemp[MAX_PATH] = { 0, };
	std::string strConvert;

	nCount = WideCharToMultiByte(nCodePage, 0, lpszString, -1, szTemp, MAX_PATH, 0, 0);
	if (nCount == 0)
	{
		if (GetLastError() == 122)
		{
			nCount = WideCharToMultiByte(nCodePage, 0, lpszString, -1, szTemp, 0, 0, 0);
			CHAR* lpTempBuf = new CHAR[nCount];

			nCount = WideCharToMultiByte(nCodePage, 0, lpszString, -1, lpTempBuf, nCount, 0, 0);
			if (nCount == 0)
			{
				delete[] lpTempBuf;
				return std::string();
			}

			strConvert = lpTempBuf;
			delete[] lpTempBuf;
			return strConvert;
		}
		return std::string();
	}

	strConvert = szTemp;
	return strConvert;
}

//=============================================================================
//헬퍼 함수임 애들 나중에 삭제해야함
//=============================================================================
std::wstring AnsiToUnicode(LPCSTR lpszString, UINT nCodePage)
{
	INT nCount = 0;
	WCHAR szTemp[MAX_PATH] = { 0, };
	std::wstring strConvert;

	nCount = MultiByteToWideChar(nCodePage, 0, lpszString, -1, szTemp, MAX_PATH);
	if (nCount == 0)
	{
		if (GetLastError() == 122)
		{
			nCount = MultiByteToWideChar(nCodePage, 0, lpszString, -1, szTemp, 0);
			WCHAR* lpTempBuf = new WCHAR[nCount];

			nCount = MultiByteToWideChar(nCodePage, 0, lpszString, -1, lpTempBuf, nCount);
			if (nCount == 0)
			{
				delete[] lpTempBuf;
				return std::wstring();
			}
			strConvert = lpTempBuf;
			delete[] lpTempBuf;
			return strConvert;
		}
		return std::wstring();
	}

	strConvert = szTemp;
	return strConvert;
}


std::map<std::wstring, cv::Mat> g_img_map;

VOID ClearImageMap()
{
	g_img_map.clear();
	return;
}

INT ImageSearchEx(IN LPCWSTR src_path, IN LPCWSTR temp_path, IN OUT RECT* find, IN COLORREF except_color)
{
	auto src_it = g_img_map.find(src_path);
	auto temp_it = g_img_map.find(temp_path);

	cv::Mat src = src_it != g_img_map.end() ? src_it->second : cv::imread(UnicodeToAnsi(src_path, CP_ACP));
	cv::Mat temp = temp_it != g_img_map.end() ? temp_it->second : cv::imread(UnicodeToAnsi(temp_path, CP_ACP));

	cv::Rect find_(0, 0, 0, 0);
	INT nRet = image::ImageSearchEx(src, temp, find_, except_color);
	if (nRet <= 0) return nRet;

	find->left = find_.x;
	find->top = find_.y;
	find->right = find_.x + find_.width;
	find->bottom = find_.y + find_.height;
	return nRet;
}

INT ImageSearchEx_All(IN LPCWSTR src_path, IN LPCWSTR temp_path, IN OUT RECT* find, IN UINT find_size, IN COLORREF except_color)
{
	auto src_it = g_img_map.find(src_path);
	auto temp_it = g_img_map.find(temp_path);

	cv::Mat src = src_it != g_img_map.end() ? src_it->second : cv::imread(UnicodeToAnsi(src_path, CP_ACP));
	cv::Mat temp = temp_it != g_img_map.end() ? temp_it->second : cv::imread(UnicodeToAnsi(temp_path, CP_ACP));

	std::vector<cv::Rect> find_;
	INT nRet = image::ImageSearchEx(src, temp, find_, except_color);
	if (nRet <= 0) return nRet;

	INT loopCount = nRet > (INT)find_size ? find_size : nRet;
	for (INT idx = 0; idx < loopCount; idx++)
	{
		find[idx].left = find_[idx].x;
		find[idx].top = find_[idx].y;
		find[idx].right = find_[idx].x + find_[idx].width;
		find[idx].bottom = find_[idx].y + find_[idx].height;
	}

	return nRet;
}

INT ImageSearchEx_Raw(IN IMAGE* src, IN IMAGE* temp, IN OUT RECT* find, IN COLORREF except_color)
{
	cv::Mat src_(src->height, src->width, src->channels == 3 ? CV_8UC3 : CV_8UC4, src->data);
	cv::Mat temp_(temp->height, temp->width, src->channels == 3 ? CV_8UC3 : CV_8UC4, temp->data);

	cv::Rect find_(0, 0, 0, 0);
	INT nRet = image::ImageSearchEx(src_, temp_, find_, except_color);
	if (nRet <= 0) return nRet;

	find->left = find_.x;
	find->top = find_.y;
	find->right = find_.x + find_.width;
	find->bottom = find_.y + find_.height;
	return nRet;
}

INT ImageSearchEx_Raw_All(IN IMAGE* src, IN IMAGE* temp, IN OUT RECT* find, IN UINT find_size, IN COLORREF except_color)
{

	cv::Mat src_(src->height, src->width, src->channels == 3 ? CV_8UC3 : CV_8UC4, src->data);
	cv::Mat temp_(temp->height, temp->width, src->channels == 3 ? CV_8UC3 : CV_8UC4, temp->data);

	std::vector<cv::Rect> find_;
	INT nRet = image::ImageSearchEx(src_, temp_, find_, except_color);
	if (nRet <= 0) return nRet;

	INT loopCount = nRet > (INT)find_size ? find_size : nRet;
	for (INT idx = 0; idx < loopCount; idx++)
	{
		find[idx].left = find_[idx].x;
		find[idx].top = find_[idx].y;
		find[idx].right = find_[idx].x + find_[idx].width;
		find[idx].bottom = find_[idx].y + find_[idx].height;
	}

	return nRet;
}