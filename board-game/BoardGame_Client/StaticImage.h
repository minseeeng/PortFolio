#pragma once
#include "GraphicsCore.h"
#include "UIObject.h"

class CStaticImage : public CUIObject
{
public:
	CStaticImage(D2D1_RECT_F rect, ID2D1Bitmap* bitmap);
	~CStaticImage();

	void Render(ID2D1HwndRenderTarget& renderTarget) override;

private:
	ID2D1Bitmap* m_bgBitmap;
};

