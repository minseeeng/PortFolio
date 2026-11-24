#pragma once
#include "GraphicsCore.h"
#include "UIObject.h"

enum BUTTON_STATE
{
    BUTTON_NORMAL = 0,
    BUTTON_DISABLED = 1,
    BUTTON_HOVER = 2,
    BUTTON_PRESSED = 3
};

class CSprite;

class CButton : public CUIObject
{
public:
    CButton(int id, CSprite* sprite, D2D1_RECT_F rect);
    ~CButton();

    void Render(ID2D1HwndRenderTarget& renderTarget) override;
    bool OnMouse(MouseEvent ev) override;

private:
    BUTTON_STATE GetState();
    bool IsInside(int x, int y);

private:
    CSprite* m_sprite;      
    bool m_inside;
    bool m_pressing;
};

