#include "SceneManager.h"
#include "Scene.h"

CSceneManager::CSceneManager(int max)
{
    m_sceneList.assign(max, nullptr);
    m_current = nullptr;
    m_next = SCENE_NONE;
}

CSceneManager::~CSceneManager()
{
    Clear();
}

bool CSceneManager::Add(SCENE_TYPE type, CScene* scene)
{
    if (scene == nullptr)
        return false;

    if (type < 0 || type >=  (int)m_sceneList.size())
    {
        delete scene;                
        return false;
    }

    CScene* oldScene = m_sceneList[type];
    if (oldScene == scene)
        return true;

    bool wasCurrent = (oldScene != nullptr && oldScene == m_current);
    if (wasCurrent)
    {
        oldScene->CleanUp();
        m_current = nullptr;       
    }

    delete oldScene;
    m_sceneList[type] = scene;       

    if (wasCurrent)
        return ChangeScene(type);     

    return true;
}

bool CSceneManager::ChangeScene(SCENE_TYPE type)
{
    if (type < 0 || type >= (int)m_sceneList.size())
        return false;

    if (m_sceneList[type] == nullptr)
        return false;
    
    if (m_current != nullptr)
        m_current->CleanUp();

    m_current = m_sceneList[type];
    
    if ((m_current->Init()) == false)
        return false;

    return true;
}

void CSceneManager::Clear()
{
    for (int i = (int)m_sceneList.size() - 1; i >= 0; i--)
    {
        delete m_sceneList[i];
        m_sceneList[i] = nullptr;
    }
    m_current = nullptr;
}

void CSceneManager::Render()
{
    if (m_current)
        m_current->Render();
}

void CSceneManager::Update()
{
    if (m_current)
        m_current->Update();
}

void CSceneManager::OnChar(wchar_t ch)
{
    if (m_current)
        m_current->OnChar(ch);
}

void CSceneManager::OnMouse(MouseEvent ev)
{
    if (m_current)
        m_current->OnMouse(ev);
}

void CSceneManager::OnPacket(unsigned short type, const char* payload)
{
    if (m_current == nullptr)
        return;

    SCENE_TYPE next = m_current->OnPacket(type, payload);  

    if (next != SCENE_NONE)
        m_next = next;
}

bool CSceneManager::ApplyChange()
{
    if (m_next == SCENE_NONE)       
        return true;

    SCENE_TYPE next = m_next;
    m_next = SCENE_NONE;          
    return ChangeScene(next);
}
