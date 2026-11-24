#pragma once
#include "GraphicsCore.h"
#include "MouseEvent.h"

class CUIObject
{
public:
	CUIObject(int id, D2D1_RECT_F rect, bool interactable);
	virtual ~CUIObject();

    virtual void Render(ID2D1HwndRenderTarget& renderTarget) = 0;  
    virtual bool OnMouse(MouseEvent ev) { return false; }
    virtual void OnChar(wchar_t ch) {}

    D2D1_RECT_F GetRect() { return m_rect; }
    int GetId() { return m_id; }
    bool IsInteractable() { return m_interactable; }
    void SetInteractable(bool on) { m_interactable = on; }

protected:
    int m_id;
    D2D1_RECT_F m_rect;
    bool m_interactable;
};

