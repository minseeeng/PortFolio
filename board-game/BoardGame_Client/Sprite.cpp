#include "Sprite.h"

CSprite::CSprite(ID2D1Bitmap* bitmap, std::vector<RECT>& rects)
	:m_bitmap(nullptr)
{
	m_bitmap = bitmap;
	m_rects = rects;
}


CSprite::~CSprite()
{
	if (m_bitmap) { m_bitmap->Release(); m_bitmap = nullptr; }
}


void CSprite::Render(ID2D1HwndRenderTarget& renderTarget, int index, const D2D1_RECT_F& dest)
{
	if (index < 0 || index >= (int)m_rects.size())
		return;

	if (m_bitmap)
	{
		D2D1_RECT_F src = D2D1::RectF((float)m_rects[index].left, (float)m_rects[index].top, (float)m_rects[index].right, (float)m_rects[index].bottom);
		renderTarget.DrawBitmap(m_bitmap, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	}
}


D2D1_SIZE_F CSprite::GetFrameSize(int index)
{
	if (index < 0 || index >= (int)m_rects.size())
		return D2D1::SizeF(0.0f, 0.0f);

	float width = (float)(m_rects[index].right - m_rects[index].left);
	float height = (float)(m_rects[index].bottom - m_rects[index].top);
	D2D1_SIZE_F size = { width, height };

	return size;
}

D2D1_RECT_F CSprite::GetFrameRect(int index)
{
	if (index < 0 || index >= (int)m_rects.size())
		return D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);

	const RECT& rect = m_rects[index];
	return D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom);
}
