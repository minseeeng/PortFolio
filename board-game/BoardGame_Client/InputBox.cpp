#include "InputBox.h"

CInputBox::CInputBox(int id, D2D1_RECT_F rect, ID2D1Bitmap* bitmap, int maxLen)
    : CUIObject(id, rect, true), m_bitmap(bitmap), m_len(0), m_maxLen(maxLen)
{
    if (m_maxLen > INPUT_MAX)
        m_maxLen = INPUT_MAX;

    m_text[0] = L'\0';
}

CInputBox::~CInputBox()
{
}

void CInputBox::Render(ID2D1HwndRenderTarget& renderTarget)
{
    if (m_bitmap)
    {
        D2D1_SIZE_F size = m_bitmap->GetSize();
        D2D1_RECT_F src = D2D1::RectF(0.0f, 0.0f, size.width, size.height);

        renderTarget.DrawBitmap(m_bitmap, m_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
        GraphicsCore::GetInstance().DrawText(m_text, m_len, m_rect, D2D1::ColorF(D2D1::ColorF::Black));
    }
}

void CInputBox::OnChar(wchar_t ch)
{
    if (ch == VK_BACK)
    {
        if (m_len > 0)
            m_text[--m_len] = L'\0';
    }
    else if (ch >= L' ' && m_len < m_maxLen)
    {
        m_text[m_len++] = ch;
        m_text[m_len] = L'\0';
    }
}
