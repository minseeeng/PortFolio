#pragma once
#include "CNetCore.h"
#include "CNetClient.h"
#include "App.h"
#include "Scene.h"
#include "WindowSize.h"
#include "SceneManager.h"

class CBoardGameApp : public CApp
{
public:
    CBoardGameApp();
    ~CBoardGameApp();

protected:
    bool OnInit() override;
    void OnRender() override; 
    bool OnUpdate() override;
    void OnCleanUp() override;

private:
    bool SetNetWork();

private:
    CNetCore m_netCore;
    CNetClient m_netClient;
    CSceneManager m_sceneManager;
};

