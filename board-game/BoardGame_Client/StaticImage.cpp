#include "StaticImage.h"

CStaticImage::CStaticImage(D2D1_RECT_F rect, ID2D1Bitmap* bitmap)
    : CUIObject(-1, rect, false), m_bgBitmap(bitmap)
{
}

CStaticImage::~CStaticImage()
{
}

void CStaticImage::Render(ID2D1HwndRenderTarget& renderTarget)
{
    if (m_bgBitmap)
    {
        D2D1_SIZE_F size = m_bgBitmap->GetSize();
        D2D1_RECT_F src = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
        D2D1_SIZE_F destSize = renderTarget.GetSize();

        renderTarget.DrawBitmap(m_bgBitmap, m_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
    }
}

