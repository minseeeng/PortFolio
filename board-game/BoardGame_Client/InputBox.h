#pragma once
#include "GraphicsCore.h"
#include "PacketType.h"
#include "UIObject.h"

#define INPUT_MAX CHAT_MAX

class CInputBox : public CUIObject
{
public:
	CInputBox(int id, D2D1_RECT_F rect, ID2D1Bitmap* bitmap, int maxLen);
	~CInputBox();

	void Render(ID2D1HwndRenderTarget& renderTarget) override;
	void OnChar(wchar_t ch) override;
	const wchar_t* GetText() { return m_text; }
	int GetLength() { return m_len; }
	void Clear() { m_len = 0; m_text[0] = L'\0'; }

private:
	ID2D1Bitmap* m_bitmap;
	wchar_t m_text[INPUT_MAX + 1];
	int m_len;
	int m_maxLen;
};

