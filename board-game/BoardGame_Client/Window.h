#pragma once
#include "MouseEvent.h"
#include <Windows.h>
#define CHAR_BUF_SIZE 32
#define MOUSE_BUF_SIZE 32

class CWindow
{
public:
	CWindow();
	~CWindow();

	bool Create(HINSTANCE hInstance, const wchar_t* title, int width, int height);
	HWND GetHwnd() { return m_hwnd; }

	bool GetChar(wchar_t& ch);
	bool GetMouse(MouseEvent& ev);

private:
	LRESULT MainProc(UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SetUpProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void PushMouse(MOUSE_TYPE type, LPARAM lParam);

private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int m_width;
	int m_height;
	wchar_t m_charBuf[CHAR_BUF_SIZE];
	int m_charCount;
	MouseEvent m_mouseBuf[MOUSE_BUF_SIZE];
	int m_mouseCount;
};
