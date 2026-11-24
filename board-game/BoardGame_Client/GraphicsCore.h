#pragma once
#include <Windows.h>
#include <d2d1.h>
#include "WicFactory.h"
#include "WriteFactory.h"

#pragma comment(lib,"d2d1.lib")

class GraphicsCore
{
public:
	static GraphicsCore& GetInstance()
	{
		static GraphicsCore instance;
		return instance;
	}

private:
	GraphicsCore();
	GraphicsCore(const GraphicsCore&) = delete;
	GraphicsCore& operator=(const GraphicsCore&) = delete;
	~GraphicsCore();

public:
	bool Init(HWND hwnd);
	void CleanUp();
	void BeginDraw();
	void Clear(const D2D1_COLOR_F& color);
	void EndDraw();

	bool LoadBitmapFromFile(const wchar_t* path, ID2D1Bitmap** bitmap);
	void DrawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& dest, const D2D1_RECT_F& src);
	void Resize(UINT width, UINT height);

	ID2D1HwndRenderTarget* GetRenderTarget() { return m_renderTarget; }
	void DrawText(const wchar_t* text, int len, const D2D1_RECT_F& rect, const D2D1_COLOR_F& color);

private:
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	CWicFactory m_wic;
	CWriteFactory m_write;
	ID2D1SolidColorBrush* m_brush;
};
