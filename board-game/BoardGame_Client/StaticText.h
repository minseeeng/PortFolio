#pragma once
#include "UIObject.h"
#include "PacketType.h"

#define STATIC_TEXT_MAX 128

class CStaticText : public CUIObject
{
public:
	CStaticText(int id, D2D1_RECT_F rect, D2D1_COLOR_F color);
	~CStaticText();

	void Render(ID2D1HwndRenderTarget& renderTarget) override;
	void SetText(const wchar_t* text);

private:
	wchar_t m_text[STATIC_TEXT_MAX];
	D2D1_COLOR_F m_color;
};

