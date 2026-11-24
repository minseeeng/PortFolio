#pragma once
#include <d2d1.h>
#include <wincodec.h>

#pragma comment(lib,"windowscodecs.lib")

class CWicFactory
{
public:
	CWicFactory();
	~CWicFactory();

	bool Init();
	void CleanUp();

	bool Load(ID2D1HwndRenderTarget& renderTarget, const wchar_t* path, ID2D1Bitmap** bitmap);

private:
	IWICImagingFactory* m_wicFactory;
};
