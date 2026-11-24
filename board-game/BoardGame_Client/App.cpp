#include "App.h"

CApp::CApp()
{
}

CApp::~CApp()
{
}

bool CApp::Init(HINSTANCE hInstance, const wchar_t* title, int width, int height)
{
	if (FAILED(CoInitialize(NULL)))
		return false;

	if ((m_window.Create(hInstance, title, width, height)) == false)
		return false;

	if ((GraphicsCore::GetInstance().Init(m_window.GetHwnd())) == false)
		return false;

	if ((OnInit()) == false)
		return false;

	return true;
}

void CApp::Run()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if ((OnUpdate()) == false)
				break;
			GraphicsCore::GetInstance().BeginDraw();
			OnRender();
			GraphicsCore::GetInstance().EndDraw();
		}
	}
}

void CApp::End()
{
	OnCleanUp();
	GraphicsCore::GetInstance().CleanUp();
	CoUninitialize();
}
