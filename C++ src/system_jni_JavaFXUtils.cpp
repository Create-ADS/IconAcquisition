#include "systemIcon.h"

void DrawPng(HICON hIcon,const char* outputFilePath, double scale)
{
    HDC hdc = GetDC(NULL);
    scale = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0;
    ReleaseDC(NULL, hdc);
    if (GetLastError() != 0)
    {
        printf("init Error: %d\n", GetLastError());
    }
    //��ȡͼ�����Ϣ
    ICONINFO iconInfo;
    GetIconInfo(hIcon, &iconInfo);
    if (GetLastError() != 0)
    {
        printf("GetIconInfo Error: %d\n", GetLastError());
    }
    //��ȡͼ��Ĵ�С
    int iconWidth = iconInfo.xHotspot * 2;
    int iconHeight = iconInfo.yHotspot * 2;
    //����һ���ڴ�DC
    HDC hMemDC = CreateCompatibleDC(NULL);
    if (GetLastError() != 0)
    {
        printf("CreateCompatibleDC Error: %d\n", GetLastError());
    }
    //����һ���ڴ�λͼ
    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, iconWidth, iconHeight);
    if (GetLastError() != 0)
    {
        printf("CreateCompatibleBitmap Error: %d\n", GetLastError());
    }
    //��λͼѡ���ڴ�DC
    SelectObject(hMemDC, hMemBitmap);
    if (GetLastError() != 0)
    {
        printf("SelectObject Error: %d\n", GetLastError());
    }
    //�������ű�������ͼ��
    DrawIconEx(hMemDC, 0, 0, hIcon, iconWidth * scale, iconHeight * scale, 0, NULL, DI_NORMAL);
    if (GetLastError() != 0)
    {
        printf("DrawIconEx Error: %d\n", GetLastError());
    }
    //����png��ʽ��ͼƬ
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    Gdiplus::Bitmap bitmap(hMemBitmap, NULL);
    bitmap.Save(CharToWchar(outputFilePath), &pngClsid, NULL);
    //��ӡ������Ϣ
    if (GetLastError() != 0)
    {
        printf("bitmap.Save Error: %d\n", GetLastError());
    }
    //�ͷ���Դ
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hIcon);
}

/**
 * @brief ��ȡ��������CLSID
 * @param format ��������ʽ
 * @param pClsid CLSID
 * @return 0�ɹ���-1ʧ��
*/
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0, size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix)
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return found;
}


/**
 * @brief const char* ת��Ϊ TCHAR*
 * @param str const char*
 * @return TCHAR*
*/
TCHAR* CharToTchar(const char* str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	TCHAR* p = new TCHAR[len + 1];
	memset(p, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, str, -1, p, len);
	return p;
}

/**
* const char* ת WCHAR*
*/
WCHAR* CharToWchar(const char* str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	WCHAR* p = new WCHAR[len + 1];
	memset(p, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, str, -1, p, len);
	return p;
}

/**
 * const char* ת LPCWSTR
*/
LPCWSTR CharToLPCWSTR(const char* str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    WCHAR* p = new WCHAR[len + 1];
    memset(p, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, str, -1, p, len);
    return p;
}

int main()
{
    SHFILEINFO shFileInfo = { 0 };
	BOOL ret;
	CoInitialize(NULL);
    ret = SHGetFileInfo(CharToLPCWSTR("C:\\Users\\adm\\Desktop\\iconsext.exe"), 0, &shFileInfo, sizeof(shFileInfo), SHGFI_ICON | SHGFI_SYSICONINDEX);
    if (!ret || shFileInfo.hIcon == NULL || shFileInfo.hIcon == INVALID_HANDLE_VALUE)
	{
		CoUninitialize();
		printf("SHGetFileInfo Error: %d\n", GetLastError());
        return -1;
	}
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    DrawPng(shFileInfo.hIcon, "E:\\Run\\test.png", 1.0);
    if (GetLastError() != 0)
    {
        printf("DrawPng Error: %d\n", GetLastError());
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);
	DestroyIcon(shFileInfo.hIcon);
	CoUninitialize();
}