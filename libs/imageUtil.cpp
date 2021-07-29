#include <map>
#include "imageUtil.h"



namespace image
{
	cv::Mat hWndToMat(HWND hWnd, INT x, INT y, INT nWidth, INT nHeigh)
	{
		HDC hwindowDC, hwindowCompatibleDC;

		INT srcheight, srcwidth;
		HBITMAP hbwindow;
		cv::Mat src;
		BITMAPINFOHEADER  bi;
		INT nBitCount = 32;

		hwindowDC = GetDC(hWnd);
		hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
		SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

		if (hWnd != NULL)
		{
			RECT windowsize;    // get the height and width of the screen
			GetClientRect(hWnd, &windowsize);

			srcheight = windowsize.bottom;
			srcwidth = windowsize.right;
		}
		else
		{
			srcheight = GetSystemMetrics(SM_CYSCREEN);
			srcwidth = GetSystemMetrics(SM_CXSCREEN);
		}

		srcwidth = nWidth != 0 ? nWidth : srcwidth;
		srcheight = nHeigh != 0 ? nHeigh : srcheight;

		nBitCount = ::GetDeviceCaps(hwindowDC, BITSPIXEL);
		if (nBitCount < 24)
		{
			DeleteDC(hwindowCompatibleDC);
			ReleaseDC(hWnd, hwindowDC);
			return cv::Mat();
		}

		src.create(srcheight, srcwidth, nBitCount == 24 ? CV_8UC3 : CV_8UC4); // 24비트와 32비트만 생각함 그 이하는 처리x
		if (src.empty())
		{
			return cv::Mat();
		}

		// create a bitmap
		hbwindow = CreateCompatibleBitmap(hwindowDC, srcwidth, srcheight);
		bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
		bi.biWidth = srcwidth;
		bi.biHeight = -srcheight;  //this is the line that makes it draw upside down or not
		bi.biPlanes = 1;
		bi.biBitCount = nBitCount;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		// use the previously created device context with the bitmap
		SelectObject(hwindowCompatibleDC, hbwindow);
		// copy from the window device context to the bitmap device context
		::BitBlt(hwindowCompatibleDC, 0, 0, srcwidth, srcheight, hwindowDC, x, y, SRCCOPY);//원본은 StretchBlt 호출이었으나 바꿈
		//StretchBlt( hwindowCompatibleDC, 0,0, width, height, hwindowDC, 0, 0,srcwidth,srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
		GetDIBits(hwindowCompatibleDC, hbwindow, 0, srcheight, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

		// avoid memory leak
		DeleteObject(hbwindow);
		DeleteDC(hwindowCompatibleDC);
		ReleaseDC(hWnd, hwindowDC);

		return src;
	}

	COLORREF mGetPixel(cv::Mat& Image, UINT x, UINT y, INT nChannels)
	{
		BYTE b, g, r;

		//rgb의 역순
		b = Image.data[y * Image.cols * nChannels + x * nChannels + 0];
		g = Image.data[y * Image.cols * nChannels + x * nChannels + 1];
		r = Image.data[y * Image.cols * nChannels + x * nChannels + 2];

		return RGB(r, g, b);
	}
	VOID mSetPixel(cv::Mat& Image, INT x, INT y, COLORREF Color, INT nChannels)
	{
		Image.data[y * Image.cols * nChannels + x * nChannels + 0] = GetBValue(Color);
		Image.data[y * Image.cols * nChannels + x * nChannels + 1] = GetGValue(Color);
		Image.data[y * Image.cols * nChannels + x * nChannels + 2] = GetRValue(Color);

		return;
	}


	INT ImageSearchEx(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rRect, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		COLORREF SrcCol, TempCol;
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();

		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}

		for (INT x = 0; x < nWidth; x++)
		{
			for (INT y = 0; y < nHeight; y++)
			{
				for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
				{
					INT y2 = (x2 == 0 ? nStartY : 0);
					for (; y2 < rTemp.rows; y2++)
					{
						SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
						TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);
						if (TempCol != ExceptColor && SrcCol != TempCol)
						{
							goto FIND_NEXT;
						}
					}
				}
				rRect = { x, y, rTemp.cols, rTemp.rows };
				return 1;
			FIND_NEXT:;
			}
		}

		return 0;
	}


	INT ImageSearchEx(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		COLORREF SrcCol, TempCol;
		INT nFindCount = 0;
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();

		//예외색상이 있는 경우에 시작 위치를 미리 계산
		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}


		rResultList.clear();

		for (INT x = 0; x < nWidth; x++)
		{
			for (INT y = 0; y < nHeight; y++)
			{
				for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
				{
					INT y2 = (x2 == 0 ? nStartY : 0);
					for (; y2 < rTemp.rows; y2++)
					{
						SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
						TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);
						if (TempCol != ExceptColor && SrcCol != TempCol)
						{
							goto FIND_NEXT;
						}
					}
				}
				rResultList.push_back({ x, y, rTemp.cols, rTemp.rows });
				nFindCount++;
			FIND_NEXT:;
			}
		}

		return nFindCount;
	}
	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rFindRect, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		thread_local COLORREF SrcCol, TempCol;
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();
		BOOL bFind = FALSE;
		CRITICAL_SECTION cs = { 0, };

		//예외색상이 있는 경우에 시작 위치를 미리 계산
		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}

		InitializeCriticalSection(&cs);
		concurrency::parallel_for(INT(0), nWidth, [&](INT x)
			{
				for (INT y = 0; bFind == FALSE && y < nHeight; y++)
				{
					for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
					{
						INT y2 = (x2 == 0 ? nStartY : 0);
						for (; y2 < rTemp.rows; y2++)
						{
							SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
							TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);
							if (TempCol != ExceptColor && SrcCol != TempCol)
							{
								goto FIND_NEXT;
							}
						}
					}
					EnterCriticalSection(&cs);
					rFindRect = { x, y, rTemp.cols, rTemp.rows };
					bFind = TRUE;
					LeaveCriticalSection(&cs);
					break;
				FIND_NEXT:;
				}
			});
		DeleteCriticalSection(&cs);


		return bFind ? 1 : 0;
	}

	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		thread_local COLORREF SrcCol, TempCol;
		INT nFindCount = 0;
		CRITICAL_SECTION cs = { 0, };
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();

		//예외색상이 있는 경우에 시작 위치를 미리 계산
		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}

		try
		{

			InitializeCriticalSection(&cs);

			rResultList.clear();

			concurrency::parallel_for(INT(0), nWidth, [&](INT x)
				{
					for (INT y = 0; y < nHeight; y++)
					{
						for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
						{
							INT y2 = (x2 == 0 ? nStartY : 0);
							for (; y2 < rTemp.rows; y2++)
							{
								SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
								TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);
								if (TempCol != ExceptColor && SrcCol != TempCol)
								{
									goto FIND_NEXT;
								}
							}
						}
						EnterCriticalSection(&cs);
						rResultList.push_back({ x, y, rTemp.cols, rTemp.rows });
						nFindCount++;
						LeaveCriticalSection(&cs);
					FIND_NEXT:;
					}
				});


			DeleteCriticalSection(&cs);
		}
		catch (const std::exception& e)
		{
			std::cout << "ImageSearch Error" << std::endl;
			std::cout << e.what() << std::endl;
			nFindCount = IMAGE_SEARCH_INTERNAL_EXCEPTION;
		}

		return nFindCount;
	}

	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, cv::Rect& rResult, FLOAT fRate, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		INT nTotalCount = rTemp.rows * rTemp.cols;
		thread_local COLORREF SrcCol, TempCol;
		thread_local INT nPixelCount;
		INT nBreakCount = (INT)(nTotalCount * fRate);
		INT nFindCount = 0;
		CRITICAL_SECTION cs;
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();
		BOOL bFind = FALSE;

		//예외색상이 있는 경우에 시작 위치를 미리 계산
		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}

		try
		{

			InitializeCriticalSection(&cs);
			concurrency::parallel_for(INT(0), nWidth, [&](INT x)
				{
					for (INT y = 0; bFind == FALSE && y < nHeight; y++)
					{
						nPixelCount = nTotalCount;

						for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
						{
							INT y2 = (x2 == 0 ? nStartY : 0);
							for (; y2 < rTemp.rows; y2++)
							{
								SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
								TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);

								if (TempCol != ExceptColor && SrcCol != TempCol)
								{
									nPixelCount--;
								}

								if (nPixelCount < nBreakCount)
								{

									goto FIND_NEXT;
								}
							}
						}
						EnterCriticalSection(&cs);
						rResult = { x, y, rTemp.cols, rTemp.rows };
						bFind = TRUE;
						LeaveCriticalSection(&cs);
					FIND_NEXT:;
					}
				});


			DeleteCriticalSection(&cs);
		}
		catch (const std::exception& e)
		{
			std::cout << "ImageSearch Error" << std::endl;
			std::cout << e.what() << std::endl;
			nFindCount = IMAGE_SEARCH_INTERNAL_EXCEPTION;
		}

		return nFindCount;
	}

	INT ImageSearchEx_Parallel(cv::Mat& rSrc, cv::Mat& rTemp, std::vector<cv::Rect>& rResultList, FLOAT fRate, COLORREF ExceptColor)
	{
		if (rSrc.empty() || rTemp.empty())
		{
			return IMAGE_EMPTY;
		}

		//템플릿의 크기가 소스의 크기를 넘는다면
		if (rSrc.cols < rTemp.cols || rSrc.rows < rTemp.rows)
		{
			return IMAGE_SIZE_ERROR;
		}

		INT nWidth = rSrc.cols - rTemp.cols + 1;
		INT nHeight = rSrc.rows - rTemp.rows + 1;
		INT nTotalCount = rTemp.rows * rTemp.cols;
		thread_local COLORREF SrcCol, TempCol;
		thread_local INT nPixelCount;
		INT nBreakCount = (INT)(nTotalCount * fRate);
		INT nFindCount = 0;
		CRITICAL_SECTION cs;
		INT nStartX = 0;
		INT nStartY = 0;
		INT nSrcChannels = rSrc.channels();
		INT nTempChannels = rTemp.channels();

		//예외색상이 있는 경우에 시작 위치를 미리 계산
		if (mGetPixel(rTemp, 0, 0, nTempChannels) == ExceptColor)
		{
			for (INT x = 0; x < rTemp.cols; x++)
			{
				for (INT y = 0; y < rTemp.rows; y++)
				{
					if (mGetPixel(rTemp, x, y, nTempChannels) != ExceptColor)
					{
						nStartX = x;
						nStartY = y;
						break;
					}
				}
				if (nStartX != 0 || nStartY != 0)
					break;
			}
		}

		try
		{

			InitializeCriticalSection(&cs);

			rResultList.clear();

			concurrency::parallel_for(INT(0), nWidth, [&](INT x)
				{
					for (INT y = 0; y < nHeight; y++)
					{
						nPixelCount = nTotalCount;

						for (INT x2 = nStartX; x2 < rTemp.cols; x2++)
						{
							INT y2 = (x2 == 0 ? nStartY : 0);
							for (; y2 < rTemp.rows; y2++)
							{
								SrcCol = mGetPixel(rSrc, x + x2, y + y2, nSrcChannels);
								TempCol = mGetPixel(rTemp, x2, y2, nTempChannels);

								if (TempCol != ExceptColor && SrcCol != TempCol)
								{
									nPixelCount--;
								}

								if (nPixelCount < nBreakCount)
								{

									goto FIND_NEXT;
								}
							}
						}
						EnterCriticalSection(&cs);
						rResultList.push_back({ x, y, rTemp.cols, rTemp.rows });
						nFindCount++;
						LeaveCriticalSection(&cs);
					FIND_NEXT:;
					}
				});


			DeleteCriticalSection(&cs);
		}
		catch (const std::exception& e)
		{
			std::cout << "ImageSearch Error" << std::endl;
			std::cout << e.what() << std::endl;
			nFindCount = IMAGE_SEARCH_INTERNAL_EXCEPTION;
		}

		return nFindCount;
	}
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

INT ImageSearchEx(LPCWSTR src_path, LPCWSTR temp_path, RECT* find, COLORREF except_color)
{
	auto src_it = g_img_map.find(src_path);
	auto temp_it = g_img_map.find(temp_path);

	cv::Mat src = src_it != g_img_map.end() ? src_it->second : cv::imread(UnicodeToAnsi(src_path, CP_ACP));
	cv::Mat temp = temp_it != g_img_map.end() ? temp_it->second : cv::imread(UnicodeToAnsi(temp_path, CP_ACP));
	
	cv::Rect find_(0, 0, 0, 0);
	INT nRet = image::ImageSearchEx(src, temp, find_, except_color);

	find->left = find_.x;
	find->top = find_.y;
	find->right = find_.x + find_.width;
	find->bottom = find_.y + find_.height;
	return nRet;
}

INT ImageSearchEx_All(LPCWSTR src_path, LPCWSTR temp_path, RECT* find, UINT find_size, COLORREF except_color)
{
	auto src_it = g_img_map.find(src_path);
	auto temp_it = g_img_map.find(temp_path);

	cv::Mat src = src_it != g_img_map.end() ? src_it->second : cv::imread(UnicodeToAnsi(src_path, CP_ACP));
	cv::Mat temp = temp_it != g_img_map.end() ? temp_it->second : cv::imread(UnicodeToAnsi(temp_path, CP_ACP));

	std::vector<cv::Rect> find_;
	INT nRet = image::ImageSearchEx(src, temp, find_, except_color);

	for (INT idx = 0; idx < find_size; idx++)
	{
		find[idx].left = find_[idx].x;
		find[idx].top = find_[idx].y;
		find[idx].right = find_[idx].x + find_[idx].width;
		find[idx].bottom = find_[idx].y + find_[idx].height;
	}

	return nRet;
}
