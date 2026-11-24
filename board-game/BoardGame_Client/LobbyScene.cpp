#include "LobbyScene.h"
#include "WindowSize.h"
#include "ResourceManager.h"
#include "CNetClient.h"

CLobbyScene::CLobbyScene(CNetClient* netClient)
    : CScene(netClient), m_userCount(0), m_chatCount(0), m_roomCount(0)
{
}

CLobbyScene::~CLobbyScene()
{
}

bool CLobbyScene::Init()
{
    m_userCount = 0;
    m_chatCount = 0;
    m_roomCount = 0;

    D2D1_RECT_F chatRect = D2D1::RectF(490.0f, 724.0f, 790.0f, 896.0f);
    D2D1_RECT_F okRect = D2D1::RectF(800.0f, 799.0f, 871.0f, 822.0f);
    D2D1_RECT_F exitRect = D2D1::RectF(1140.0f, 780.0f, 1235.0f, 811.0f);
    D2D1_RECT_F createRoomRect = D2D1::RectF(1140.0f, 724.0f, 1235.0f, 770.0f);

    ID2D1Bitmap* chatBoxBitmap = CResourceManager::GetInstance().GetBitmap(L"Resource\\jngSangMaSelectText.bmp");
    if (chatBoxBitmap == nullptr)
        return false;

    m_uiList.push_back(new CInputBox(LOBBY_UI_CHAT, chatRect, chatBoxBitmap, CHAT_MAX));

    CSprite* okButtonSprite = CResourceManager::GetInstance().GetSprite(L"Resource\\_Popup_OKBtn.bmp", L"Resource\\_Popup_OKBtn.spr");
    if (okButtonSprite == nullptr)
        return false;

    m_uiList.push_back(new CButton(LOBBY_UI_OK, okButtonSprite, okRect));

    CSprite* exitButtonSprite = CResourceManager::GetInstance().GetSprite(L"Resource\\_ExitBtn.bmp", L"Resource\\_ExitBtn.spr");
    if (exitButtonSprite == nullptr)
        return false;

    m_uiList.push_back(new CButton(LOBBY_UI_EXIT, exitButtonSprite, exitRect));

    CSprite* roomButtonSprite = CResourceManager::GetInstance().GetSprite(L"Resource\\_CreateRoomBtn.bmp", L"Resource\\_CreateRoomBtn.spr");
    if (roomButtonSprite == nullptr)
        return false;

    m_uiList.push_back(new CButton(LOBBY_UI_ROOM, roomButtonSprite, createRoomRect));

    return true;
}

void CLobbyScene::Render()
{
    GraphicsCore::GetInstance().Clear(D2D1::ColorF(D2D1::ColorF::White));
    for (int i = 0; i < m_chatCount; i++)
    {
        D2D1_RECT_F lineRect = D2D1::RectF(40.0f, 40.0f + i * 30.0f, 980.0f, 40.0f + (i + 1) * 30.0f);
        GraphicsCore::GetInstance().DrawText(m_chatLog[i], (int)wcslen(m_chatLog[i]), lineRect, D2D1::ColorF(D2D1::ColorF::Black));
    }

    for (int i = 0; i < m_userCount; i++)
    {
        D2D1_RECT_F userRect = D2D1::RectF(1000.0f, 70.0f + i * 30.0f, 1240.0f, 100.0f + i * 30.0f);
        GraphicsCore::GetInstance().DrawText(m_userList[i], (int)wcslen(m_userList[i]), userRect, D2D1::ColorF(D2D1::ColorF::Black));
    }

    for (int i = 0; i < m_roomCount; i++)
    {
        wchar_t line[NICK_MAX + 16];
        _snwprintf_s(line, _countof(line), _TRUNCATE, L"%s의 방입니다", m_roomList[i].owner);
        D2D1_RECT_F roomRect = D2D1::RectF(40.0f, 400.0f + i * 30.0f, 980.0f, 430.0f + i * 30.0f);
        GraphicsCore::GetInstance().DrawText(line, (int)wcslen(line), roomRect, D2D1::ColorF(D2D1::ColorF::Black));
    }

    CScene::Render();
}


void CLobbyScene::OnChat(const char* nick, const char* msg)
{
    wchar_t nickW[NICK_MAX + 1];
    wchar_t msgW[CHAT_MAX + 1];

    if (MultiByteToWideChar(CP_ACP, 0, nick, -1, nickW, NICK_MAX + 1) == 0)
        return;
    if (MultiByteToWideChar(CP_ACP, 0, msg, -1, msgW, CHAT_MAX + 1) == 0)
        return;

    wchar_t line[CHAT_LINE_SIZE];
    _snwprintf_s(line, CHAT_LINE_SIZE, _TRUNCATE, L"%s : %s", nickW, msgW);
    AddChatLog(line);
}

void CLobbyScene::OnClick(int id)
{
    switch (id)
    {
    case LOBBY_UI_OK:
        SendChat();
        break;
    case LOBBY_UI_EXIT:
        AddChatLog(L"나가기 클릭");
        break;
    case LOBBY_UI_ROOM:
        if (m_netClient->Send(TYPE_ROOM_CREATE, "", 0))
            FindUI(LOBBY_UI_ROOM)->SetInteractable(false);
        break;
    }
}

void CLobbyScene::AddChatLog(const wchar_t* line)
{
    if (m_chatCount == CHAT_LOG_MAX)
    {
        for (int i = 0; i < CHAT_LOG_MAX - 1; i++)
            wcscpy_s(m_chatLog[i], CHAT_LINE_SIZE, m_chatLog[i + 1]);
        m_chatCount--;
    }

    wcscpy_s(m_chatLog[m_chatCount], CHAT_LINE_SIZE, line);
    m_chatCount++;
}

bool CLobbyScene::SendChat()
{
    CInputBox* chatBox = static_cast<CInputBox*>(FindUI(LOBBY_UI_CHAT));
    if (chatBox == nullptr)
        return false;

    if (chatBox->GetLength() == 0)
        return false;

    char chat[CHAT_MAX * 2 + 1];
    int len = WideCharToMultiByte(CP_ACP, 0, chatBox->GetText(), -1, chat, sizeof(chat), NULL, NULL);
    if (len == 0)
        return false;

    if (m_netClient->Send(TYPE_CHAT_SEND, chat, len - 1) == false)
        return false;

    chatBox->Clear();
    return true;
}

void CLobbyScene::OnChar(wchar_t ch)
{
    if (ch == L'\r')                
    {
        SendChat();
        return;
    }

    CScene::OnChar(ch);
}

SCENE_TYPE CLobbyScene::OnPacket(unsigned short type, const char* payload)
{
    if (type == TYPE_CHAT_RECV)
        OnChat(payload, payload + NICK_SIZE);    
    else if(type == TYPE_ENTRY_LOBBY)
    {
        if (m_userCount == CLIENT_MAX)    
            return SCENE_NONE;

        if (MultiByteToWideChar(CP_ACP, 0, payload, -1, m_userList[m_userCount], NICK_MAX + 1) == 0)
            return SCENE_NONE;

        m_userCount++;
    }
    else if (type == TYPE_EXIT_LOBBY)
    {
        wchar_t nickW[NICK_MAX + 1];

        if (MultiByteToWideChar(CP_ACP, 0, payload, -1, nickW, NICK_MAX + 1) == 0)
            return SCENE_NONE;

        for (int i = 0; i < m_userCount; i++)
        {
            if (wcscmp(m_userList[i], nickW) == 0)
            {
                for (int j = i; j < m_userCount - 1; j++)
                {
                    wcscpy_s(m_userList[j], NICK_MAX + 1, m_userList[j + 1]);
                }
                m_userCount--;
                break;
            }
        }
    }
    else if (type == TYPE_ROOM_ADD)
    {
        if (m_roomCount == ROOM_MAX)
            return SCENE_NONE;

        unsigned short id;
        memcpy(&id, payload + NICK_SIZE, sizeof(id));
        if (MultiByteToWideChar(CP_ACP, 0, payload, -1, m_roomList[m_roomCount].owner, NICK_MAX + 1) == 0)
            return SCENE_NONE;

        m_roomList[m_roomCount].id = id;
        m_roomCount++;
    }
    else if (type == TYPE_ROOM_REMOVE)
    {
        unsigned short id;
        memcpy(&id, payload + NICK_SIZE, sizeof(id));
        for (int i = 0; i < m_roomCount; i++)
        {
            if (m_roomList[i].id == id)
            {
                for (int j = i; j < m_roomCount - 1; j++)
                    m_roomList[j] = m_roomList[j + 1];
                m_roomCount--;
                break;
            }
        }
    }
    else if (type == TYPE_ROOM_ENTER_OK)
    {
        return SCENE_GAME;
    }
    else if (type == TYPE_ROOM_ENTER_FAIL)
    {
        AddChatLog(L"방을 만들 수 없습니다");
        FindUI(LOBBY_UI_ROOM)->SetInteractable(true);
    }
    return SCENE_NONE;
}
