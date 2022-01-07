// 다음 ifdef 블록은 DLL에서 내보내는 작업을 더 간소화하는 매크로를 만드는
// 표준 방법입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 IMAGEDLL_EXPORTS 기호로
// 컴파일됩니다. 이 DLL을 사용하는 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 포함된 다른 모든 프로젝트에서는
// IMAGEDLL_API 함수를 DLL에서 가져오는 것으로 표시되는 반면, 이 DLL에서는
// 이 매크로로 정의된 기호가 내보내지는 것으로 표시됩니다.
#ifdef IMAGEDLL_EXPORTS
#define IMAGEDLL_API __declspec(dllexport)
#else
#define IMAGEDLL_API __declspec(dllimport)
#endif

#include <Windows.h>

// 이 클래스는 dll에서 내보낸 것입니다.
class IMAGEDLL_API CImageDLL {
public:
	CImageDLL(void);
	// TODO: 여기에 메서드를 추가합니다.
};

extern IMAGEDLL_API int nImageDLL;

IMAGEDLL_API int fnImageDLL(void);

extern "C" IMAGEDLL_API VOID ClearImageMap();
extern "C" IMAGEDLL_API INT ImageSearchEx(IN LPCWSTR src_path, IN LPCWSTR temp_path, IN OUT RECT * find, IN COLORREF except_color);
extern "C" IMAGEDLL_API INT ImageSearchEx_All(IN LPCWSTR src_path, IN LPCWSTR temp_path, IN OUT RECT * find, IN UINT find_size, IN COLORREF except_color);

typedef struct image_
{
	BYTE* data;
	INT width;
	INT height;
	INT channels;
}IMAGE;

extern "C" IMAGEDLL_API INT ImageSearchEx_Raw(IN IMAGE * src, IN IMAGE * temp, IN OUT RECT * find, IN COLORREF except_color);
extern "C" IMAGEDLL_API INT ImageSearchEx_Raw_All(IN IMAGE * src, IN IMAGE * temp, IN OUT RECT * find, IN UINT find_size, IN COLORREF except_color);