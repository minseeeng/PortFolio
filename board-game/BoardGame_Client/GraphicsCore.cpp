#include "GraphicsCore.h"

GraphicsCore::GraphicsCore()
	: m_factory(nullptr), m_renderTarget(nullptr), m_brush(nullptr)
{
}

GraphicsCore::~GraphicsCore()
{
	CleanUp();
}

bool GraphicsCore::Init(HWND hwnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (FAILED(hr))
		return false;

	if ((m_wic.Init()) == false)
		return false;

	RECT rc;
	GetClientRect(hwnd, &rc);

	hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_renderTarget);
	if (FAILED(hr))
		return false;

	if ((m_write.Init()) == false)
		return false;

	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brush);
	if (FAILED(hr))
		return false;

	return true;
}

void GraphicsCore::CleanUp()
{
	if (m_brush) { m_brush->Release(); m_brush = nullptr; }
	if (m_renderTarget) { m_renderTarget->Release(); m_renderTarget = nullptr; }
	m_wic.CleanUp();
	m_write.CleanUp();
	if (m_factory) { m_factory->Release(); m_factory = nullptr; }
}

void GraphicsCore::BeginDraw()
{
	m_renderTarget->BeginDraw();
}

void GraphicsCore::Clear(const D2D1_COLOR_F& color)
{
	m_renderTarget->Clear(color);
}

void GraphicsCore::EndDraw()
{
	m_renderTarget->EndDraw();
}

bool GraphicsCore::LoadBitmapFromFile(const wchar_t* path, ID2D1Bitmap** bitmap)
{
	return m_wic.Load(*m_renderTarget, path, bitmap);
}

void GraphicsCore::DrawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& dest, const D2D1_RECT_F& src)
{
	m_renderTarget->DrawBitmap(bitmap, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
}

void GraphicsCore::Resize(UINT width, UINT height)
{
	if (m_renderTarget)
		m_renderTarget->Resize(D2D1::SizeU(width, height));
}

void GraphicsCore::DrawText(const wchar_t* text, int len, const D2D1_RECT_F& rect, const D2D1_COLOR_F& color)
{
	m_brush->SetColor(color);
	m_renderTarget->DrawTextW(text, len, m_write.GetTextFormat(), rect, m_brush);
}
