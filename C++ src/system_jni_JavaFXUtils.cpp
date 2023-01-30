#include "system_jni_JavaFXUtils.h"

/*
 * Class:     screen_creator_util_SystemTools
 * Method:    getIcon
 * Signature: ([B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_system_jni_JavaFXUtils_getIconForPng(JNIEnv* env, jclass clasz, jbyteArray bytes, jint pixel)
{
	jbyte* bytess = env->GetByteArrayElements(bytes, 0);
	jsize size1 = env->GetArrayLength(bytes);
	TCHAR* path = CharToTchar(ByteToChar(bytess, size1));
	SHFILEINFO shFileInfo = { 0 };
	BOOL ret;
	CoInitialize(NULL);
	
	ret = SHGetFileInfo(path, 0, &shFileInfo, sizeof(shFileInfo), SHGFI_ICON | SHGFI_SYSICONINDEX);
	if (!ret || shFileInfo.hIcon == NULL || shFileInfo.hIcon == INVALID_HANDLE_VALUE)
	{
		CoUninitialize();
		return NULL;
	}
	
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	int size = 0;
	BYTE* buffer = SaveIconToPngBytes(shFileInfo, pixel, pixel, size);
	if (buffer == NULL)
	{
		delete[] buffer;
		Gdiplus::GdiplusShutdown(gdiplusToken);
		DestroyIcon(shFileInfo.hIcon);
		CoUninitialize();
		return NULL;
	}
	jbyteArray array = env->NewByteArray(size);
	env->SetByteArrayRegion(array, 0, size, (jbyte*)buffer);
	
	delete[] buffer;
	Gdiplus::GdiplusShutdown(gdiplusToken);
	DestroyIcon(shFileInfo.hIcon);
	CoUninitialize();
	return array;
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
 * @brief 获取文件图标并保存为PNG格式字节数组
 * @param SHFILEINFO 文件图标信息
 * @param int w 指定宽度
 * @param int h 指定高度
 * @param int size PNG字节数组大小
 * @return BYTE* PNG字节数组
*/
BYTE* SaveIconToPngBytes(SHFILEINFO shFileInfo, int w, int h, int& size)
{
	HICON hIcon = shFileInfo.hIcon;
	
	HDC hdc = GetDC(NULL);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	
	DrawIconEx(hMemDC, 0, 0, hIcon, w, h, 0, NULL, DI_NORMAL);
	SelectObject(hMemDC, hOldBitmap);
	
	DeleteDC(hMemDC);
	ReleaseDC(NULL, hdc);
	
	ICONINFO iconInfo = { 0 };
	iconInfo.fIcon = TRUE;
	iconInfo.hbmColor = hBitmap;
	iconInfo.hbmMask = hBitmap;
	HICON hIcon2 = CreateIconIndirect(&iconInfo);

	DeleteObject(hBitmap);
	
	BYTE* bytes = SaveHIconToPngBytes(hIcon2, size);

	DestroyIcon(hIcon);
	DestroyIcon(hIcon2);
	return bytes;
}

/**
 * @brief 保存HICON到PNG格式字节数组
 * @param hIcon HICON
 * @param size 字节数组大小
*/
BYTE* SaveHIconToPngBytes(HICON hIcon, int& size)
{
	if (hIcon == NULL)
	{
		return NULL;
	}

	ICONINFO icInfo = { 0 };
	if (!::GetIconInfo(hIcon, &icInfo))
	{
		return NULL;
	}

	BITMAP bitmap;
	GetObject(icInfo.hbmColor, sizeof(BITMAP), &bitmap);

	Gdiplus::Bitmap* pBitmap = NULL;
	Gdiplus::Bitmap* pWrapBitmap = NULL;

	do
	{
		if (bitmap.bmBitsPixel != 32)
		{
			pBitmap = Gdiplus::Bitmap::FromHICON(hIcon);
		}
		else
		{
			pWrapBitmap = Gdiplus::Bitmap::FromHBITMAP(icInfo.hbmColor, NULL);
			if (!pWrapBitmap)
				break;

			Gdiplus::BitmapData bitmapData;
			Gdiplus::Rect rcImage(0, 0, pWrapBitmap->GetWidth(), pWrapBitmap->GetHeight());

			pWrapBitmap->LockBits(&rcImage, Gdiplus::ImageLockModeRead, pWrapBitmap->GetPixelFormat(), &bitmapData);
			pBitmap = new (Gdiplus::Bitmap)(bitmapData.Width, bitmapData.Height, bitmapData.Stride, PixelFormat32bppARGB, (BYTE*)bitmapData.Scan0);
			pWrapBitmap->UnlockBits(&bitmapData);
		}

		CLSID encoderCLSID;
		GetEncoderClsid(CharToWchar("image/png"), &encoderCLSID);
		IStream* pStream = NULL;
		CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		Gdiplus::Status st = pBitmap->Save(pStream, &encoderCLSID, NULL);
		if (st != Gdiplus::Ok)
			break;
		STATSTG statstg;
		pStream->Stat(&statstg, STATFLAG_NONAME);
		size = statstg.cbSize.LowPart;
		BYTE* buffer = new BYTE[size];
		LARGE_INTEGER li = { 0 };
		pStream->Seek(li, STREAM_SEEK_SET, NULL);
		ULONG read = 0;
		pStream->Read(buffer, size, &read);
		pStream->Release();
		return buffer;
	} while (false);

	delete pBitmap;
	if (pWrapBitmap)
		delete pWrapBitmap;
	DeleteObject(icInfo.hbmColor);
	DeleteObject(icInfo.hbmMask);

	return NULL;
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
 * byte* 转 char*
 * 字节格式为UTF-8
 */
char* ByteToChar(jbyte* bytes, int len)
{
	char* str = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		str[i] = bytes[i];
	}
	str[len] = '\0';
	return str;
}