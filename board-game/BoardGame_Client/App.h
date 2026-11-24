#pragma once
#include "Window.h"
#include "GraphicsCore.h"

class CApp
{
public:
	CApp();
	virtual ~CApp();

	bool Init(HINSTANCE hInstance, const wchar_t* title, int width, int height);
	void Run();
	void End();

protected:
	// interface
	virtual bool OnInit() { return true; }
	virtual bool OnUpdate() { return true; }
	virtual void OnRender() {}
	virtual void OnCleanUp() {}

protected:
	CWindow m_window;
};
