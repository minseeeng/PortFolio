#include "NickNameScene.h"
#include "WindowSize.h"
#include "GraphicsCore.h"
#include <stdio.h>
#include "ResourceManager.h"
#include "CNetClient.h"

CNickNameScene::CNickNameScene(CNetClient* netClient)
    :  CScene(netClient)
{
    
}

CNickNameScene::~CNickNameScene()
{
}

bool CNickNameScene::Init()
{
    D2D1_RECT_F backGroundRect = D2D1::RectF(0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);
    D2D1_RECT_F nickNameRect = D2D1::RectF(490.0f, 394.0f, 790.0f, 566.0f);
    D2D1_RECT_F okRect = D2D1::RectF(605.0f, 533.0f, 676.0f, 556.0f);
    D2D1_RECT_F failTextRect = D2D1::RectF(0, 576, 1280, 606);
   
    ID2D1Bitmap* backGroundBitmap = CResourceManager::GetInstance().GetBitmap(L"Resource\\jngboardone1.bmp");
    if (backGroundBitmap == nullptr)
        return false;
    m_uiList.push_back(new CStaticImage(backGroundRect, backGroundBitmap));


    ID2D1Bitmap* nickNameBitmap = CResourceManager::GetInstance().GetBitmap(L"Resource\\jngSangMaSelectText.bmp");
    if (nickNameBitmap == nullptr)
        return false;
    m_uiList.push_back(new CInputBox(NICK_UI_INPUT, nickNameRect, nickNameBitmap, NICK_MAX));
    
    CSprite* buttonSprite = CResourceManager::GetInstance().GetSprite(L"Resource\\_Popup_OKBtn.bmp", L"Resource\\_Popup_OKBtn.spr");
    if (buttonSprite == nullptr)
        return false;
    m_uiList.push_back(new CButton(NICK_UI_OK, buttonSprite, okRect));

    m_uiList.push_back(new CStaticText(NICK_UI_FAIL, failTextRect, D2D1::ColorF(D2D1::ColorF::Red)));
    
    return true;
}

void CNickNameScene::Render()
{
    GraphicsCore::GetInstance().Clear(D2D1::ColorF(D2D1::ColorF::White));
    CScene::Render();
}

void CNickNameScene::Update()
{
}

void CNickNameScene::OnChar(wchar_t ch)
{
    if (ch == L'\r')
    {
        SendNickName();
        return;
    }

    CScene::OnChar(ch);
    CStaticText* failText = static_cast<CStaticText*>(FindUI(NICK_UI_FAIL));
    if (failText != nullptr)
        failText->SetText(L"");
}

SCENE_TYPE CNickNameScene::OnPacket(unsigned short type, const char* payload)
{
    switch (type)
    {
    case TYPE_NICK:
        return SCENE_LOBBY;
    case TYPE_NICK_FAIL:         
    {
        CStaticText* failText = static_cast<CStaticText*>(FindUI(NICK_UI_FAIL));
        if (failText != nullptr)
            failText->SetText(L"닉네임을 사용할 수 없습니다");
        FindUI(NICK_UI_OK)->SetInteractable(true);
    }
        break;
    }

    return SCENE_NONE;
}

bool CNickNameScene::SendNickName()
{
    CUIObject* okButton = FindUI(NICK_UI_OK);
    if (okButton->IsInteractable() == false)
        return false;

    CInputBox* nickBox = static_cast<CInputBox*>(FindUI(NICK_UI_INPUT));
    if (nickBox == nullptr)
        return false;

    if (nickBox->GetLength() == 0)
        return false;

    char nick[NICK_SIZE];
    int len = WideCharToMultiByte(CP_ACP, 0, nickBox->GetText(), -1, nick, sizeof(nick), NULL, NULL);
    if (len == 0)
        return false;

    if (m_netClient->Send(TYPE_NICK, nick, len - 1) == false)
        return false;

    okButton->SetInteractable(false);
    return true;
    // gpm : 가지고 있는 대상이 뭐냐??
    //       보낼 대상이 뭐냐?? 그게 지금 보이는가??
}

void CNickNameScene::OnClick(int id)
{
    switch (id)
    {
    case NICK_UI_OK:
        SendNickName();
        break;
    }
}