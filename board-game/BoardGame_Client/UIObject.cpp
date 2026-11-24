#include "UIObject.h"

CUIObject::CUIObject(int id, D2D1_RECT_F rect, bool interactable)
	: m_id(id), m_rect(rect), m_interactable(interactable)
{
}

CUIObject::~CUIObject()
{
}
