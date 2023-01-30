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
    //获取图标的信息
    ICONINFO iconInfo;
    GetIconInfo(hIcon, &iconInfo);
    if (GetLastError() != 0)
    {
        printf("GetIconInfo Error: %d\n", GetLastError());
    }
    //获取图标的大小
    int iconWidth = iconInfo.xHotspot * 2;
    int iconHeight = iconInfo.yHotspot * 2;
    //创建一个内存DC
    HDC hMemDC = CreateCompatibleDC(NULL);
    if (GetLastError() != 0)
    {
        printf("CreateCompatibleDC Error: %d\n", GetLastError());
    }
    //创建一个内存位图
    HBITMAP hMemBitmap = CreateCompatibleBitmap(hdc, iconWidth, iconHeight);
    if (GetLastError() != 0)
    {
        printf("CreateCompatibleBitmap Error: %d\n", GetLastError());
    }
    //将位图选入内存DC
    SelectObject(hMemDC, hMemBitmap);
    if (GetLastError() != 0)
    {
        printf("SelectObject Error: %d\n", GetLastError());
    }
    //根据缩放比例绘制图标
    DrawIconEx(hMemDC, 0, 0, hIcon, iconWidth * scale, iconHeight * scale, 0, NULL, DI_NORMAL);
    if (GetLastError() != 0)
    {
        printf("DrawIconEx Error: %d\n", GetLastError());
    }
    //保存png格式的图片
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    Gdiplus::Bitmap bitmap(hMemBitmap, NULL);
    bitmap.Save(CharToWchar(outputFilePath), &pngClsid, NULL);
    //打印错误信息
    if (GetLastError() != 0)
    {
        printf("bitmap.Save Error: %d\n", GetLastError());
    }
    //释放资源
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);
    DeleteObject(hIcon);
}

/**
 * @brief 获取解码器的CLSID
 * @param format 解码器格式
 * @param pClsid CLSID
 * @return 0成功，-1失败
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
 * @brief const char* 转换为 TCHAR*
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
* const char* 转 WCHAR*
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
 * const char* 转 LPCWSTR
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