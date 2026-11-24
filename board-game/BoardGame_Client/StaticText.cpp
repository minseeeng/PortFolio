#include "StaticText.h"

CStaticText::CStaticText(int id, D2D1_RECT_F rect, D2D1_COLOR_F color)
	:CUIObject(id, rect, false), m_color(color)
{
	m_text[0] = L'\0';
}

CStaticText::~CStaticText()
{
}

void CStaticText::Render(ID2D1HwndRenderTarget& renderTarget)
{
	GraphicsCore::GetInstance().DrawText(m_text, (int)wcslen(m_text), m_rect, m_color);
}

void CStaticText::SetText(const wchar_t* text)
{
	wcsncpy_s(m_text, _countof(m_text), text, _TRUNCATE);
}
