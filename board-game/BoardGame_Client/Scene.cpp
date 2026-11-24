#include "Scene.h"
#include "CNetClient.h"

CScene::CScene(CNetClient* netClient)
    :m_netClient(netClient)
{
}

CScene::~CScene()
{
    CleanUp();
}

void CScene::Render()
{
    ID2D1HwndRenderTarget* renderTarget = GraphicsCore::GetInstance().GetRenderTarget();

    for (CUIObject* ui : m_uiList)
    {
        ui->Render(*renderTarget);
    }
}

void CScene::CleanUp()
{
    for (CUIObject* ui : m_uiList)
        delete ui;
    m_uiList.clear();
}

void CScene::OnChar(wchar_t ch)
{
    for (CUIObject* ui : m_uiList)
    {
        if (ui->IsInteractable() == false)
            continue;

        ui->OnChar(ch);
    }
}

void CScene::OnMouse(MouseEvent ev)
{
    for (CUIObject* ui : m_uiList)
    {
        if (ui->IsInteractable() == false)
            continue;

        if (ui->OnMouse(ev))
            OnClick(ui->GetId());
    }
}

CUIObject* CScene::FindUI(int id)
{
    for (CUIObject* ui : m_uiList)
    {
        if (ui->GetId() == id)
            return ui;
    }
    return nullptr;
}

