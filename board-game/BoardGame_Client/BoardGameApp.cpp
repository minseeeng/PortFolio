#include "BoardGameApp.h"
#include "NickNameScene.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include <stdio.h>
#include "SceneType.h"
#include "ResourceManager.h"

CBoardGameApp::CBoardGameApp()
    :m_netClient(&m_netCore), m_sceneManager(SCENE_MAX)
{
}

CBoardGameApp::~CBoardGameApp()
{
}

bool CBoardGameApp::OnInit()
{
    if ((SetNetWork() == false))
    {
        MessageBox(m_window.GetHwnd(), L"서버 접속 실패", L"Error", MB_OK);
        return false;
    }

    if (m_sceneManager.Add(SCENE_NICKNAME, new CNickNameScene(&m_netClient)) == false)
        return false;

    if (m_sceneManager.Add(SCENE_LOBBY, new CLobbyScene(&m_netClient)) == false)
        return false;

    if (m_sceneManager.Add(SCENE_GAME, new CGameScene(&m_netClient)) == false)
        return false;

    if (m_sceneManager.ChangeScene(SCENE_NICKNAME) == false)
    {
        MessageBox(m_window.GetHwnd(), L"리소스 로딩 실패", L"Error", MB_OK);
        return false;
    }
    return true;
}

void CBoardGameApp::OnRender()
{
    m_sceneManager.Render();
}

bool CBoardGameApp::OnUpdate()
{
    if (m_sceneManager.ApplyChange() == false)        
    {
        MessageBox(m_window.GetHwnd(), L"리소스 로딩 실패", L"Error", MB_OK);
        return false;
    }

    wchar_t ch;
    while (m_window.GetChar(ch))
    {
        m_sceneManager.OnChar(ch);
    }

    MouseEvent ev;
    while (m_window.GetMouse(ev))
    {
        m_sceneManager.OnMouse(ev);
    }

    if (m_netClient.Recv() == false)
    {
        MessageBox(m_window.GetHwnd(), L"Recv 실패", L"Error", MB_OK);
        return false;
    }
    
    unsigned short type;
    char buffer[BUF_SIZE];
    while (m_sceneManager.IsChangeReserved() == false)
    {
        PACKET_RESULT result = m_netClient.GetPacket(type, buffer);
        if (result == PACKET_NONE)
            break;

        if (result == PACKET_ERROR)
        {
            MessageBox(m_window.GetHwnd(), L"잘못된 패킷", L"Error", MB_OK);
            return false;
        }

        m_sceneManager.OnPacket(type, buffer);
    }
    
    return true;
}

void CBoardGameApp::OnCleanUp()
{
    m_sceneManager.Clear();
    CResourceManager::GetInstance().Clear();
    m_netCore.CleanUp();
    WSACleanup();
}

bool CBoardGameApp::SetNetWork()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        return false;
    }

    if ((m_netCore.CreateSocket() == false))
        return false;

    m_netCore.SetAddress("127.0.0.1", SERVER_PORT);

    if((m_netCore.Connect()==false))
        return false;

    return true;
}