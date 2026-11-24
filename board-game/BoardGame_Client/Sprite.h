#pragma once
#include "GraphicsCore.h"
#include <vector>

class CSprite
{
public:
	CSprite(ID2D1Bitmap* bitmap, std::vector<RECT>& rects);
	~CSprite();

	void Render(ID2D1HwndRenderTarget& renderTarget, int index, const D2D1_RECT_F& dest);

	D2D1_SIZE_F GetFrameSize(int index);
	D2D1_RECT_F GetFrameRect(int index);
	ID2D1Bitmap* GetBitmap() { return m_bitmap; }

private:
	ID2D1Bitmap* m_bitmap;
	std::vector<RECT> m_rects;
};

