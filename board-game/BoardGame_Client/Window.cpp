#include "Window.h"

CWindow::CWindow()
	: m_hwnd(nullptr), m_hInstance(nullptr), m_width(0), m_height(0),
	m_charCount(0), m_mouseCount(0)
{
}

CWindow::~CWindow()
{
}

bool CWindow::Create(HINSTANCE hInstance, const wchar_t* title, int width, int height)
{
	const wchar_t* className = L"BoardGameWindow";

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CWindow::SetUpProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = nullptr;
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
		return false;

	m_hInstance = hInstance;
	m_width = width;
	m_height = height;

	DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, style, FALSE);

	m_hwnd = CreateWindow(
		className,
		title,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance,
		this);

	if (!m_hwnd)
		return false;

	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	return true;
}

bool CWindow::GetChar(wchar_t& ch)
{
	if (m_charCount == 0)
		return false;

	ch = m_charBuf[0];
	m_charCount--;
	memmove(m_charBuf, m_charBuf + 1, m_charCount * sizeof(wchar_t));
	return true;
}

bool CWindow::GetMouse(MouseEvent& ev)
{
	if (m_mouseCount == 0)
		return false;

	ev = m_mouseBuf[0];
	m_mouseCount--;
	memmove(m_mouseBuf, m_mouseBuf + 1, m_mouseCount * sizeof(MouseEvent));
	return true;
}

LRESULT CWindow::MainProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{           
	case WM_CHAR:
		if (m_charCount < CHAR_BUF_SIZE)
			m_charBuf[m_charCount++] = (wchar_t)wParam;
		return 0;
	case WM_MOUSEMOVE:
		PushMouse(MOUSE_MOVE, lParam);
		return 0;
	case WM_LBUTTONDOWN:
		PushMouse(MOUSE_DOWN, lParam);
		return 0;
	case WM_LBUTTONUP:
		PushMouse(MOUSE_UP, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(m_hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK CWindow::SetUpProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		CWindow* pWindow = reinterpret_cast<CWindow*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		pWindow->m_hwnd = hwnd;
	}

	CWindow* pWindow = reinterpret_cast<CWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	if (pWindow)
		return pWindow->MainProc(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CWindow::PushMouse(MOUSE_TYPE type, LPARAM lParam)
{
	MouseEvent ev;
	ev.type = type;
	ev.x = LOWORD(lParam);
	ev.y = HIWORD(lParam);

	if (type == MOUSE_MOVE && m_mouseCount > 0 && m_mouseBuf[m_mouseCount - 1].type == ev.type)
	{
		m_mouseBuf[m_mouseCount - 1] = ev;
		return;
	}
	
	if (m_mouseCount < MOUSE_BUF_SIZE)
		m_mouseBuf[m_mouseCount++] = ev;
}
