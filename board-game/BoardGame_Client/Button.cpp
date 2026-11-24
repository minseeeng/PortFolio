#include "Button.h"
#include "Sprite.h"

CButton::CButton(int id, CSprite* sprite, D2D1_RECT_F rect)
	: CUIObject(id, rect, true), m_sprite(sprite), m_inside(false), m_pressing(false)
{
}

CButton::~CButton()
{
}

void CButton::Render(ID2D1HwndRenderTarget& renderTarget)
{
	m_sprite->Render(renderTarget, GetState(), m_rect);
}

bool CButton::OnMouse(MouseEvent ev)
{
	m_inside = IsInside(ev.x, ev.y);

	if (m_inside && ev.type == MOUSE_DOWN)
		m_pressing = true;

	if (m_pressing && ev.type == MOUSE_UP)
	{
		m_pressing = false;
		return m_inside;
	}

	return false;
}


BUTTON_STATE CButton::GetState()
{
	if (m_interactable == false)
		return BUTTON_DISABLED;

	if (m_pressing && m_inside)
		return BUTTON_PRESSED;
	else if (m_inside)
		return BUTTON_HOVER;
	else
		return BUTTON_NORMAL;
}

bool CButton::IsInside(int x, int y)
{
	if (m_rect.left <= x && x <= m_rect.right &&
		m_rect.top <= y && y <= m_rect.bottom)
		return true;
	else
		return false;
}
