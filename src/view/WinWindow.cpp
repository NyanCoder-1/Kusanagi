#include "view/WinWindow.h"

Kusanagi::View::WinWindow::WinWindow()
{

}
Kusanagi::View::WinWindow::~WinWindow()
{

}

void Kusanagi::View::WinWindow::Create()
{
}

void Kusanagi::View::WinWindow::Update()
{
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Kusanagi::View::WinWindow::GetHandler()
{
	return hWnd;
}

LRESULT __stdcall Kusanagi::View::WinWindow::StaticProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	void* ptr = reinterpret_cast<void*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	return ptr ? reinterpret_cast<Kusanagi::View::WinWindow*>(ptr)->Proc(hWnd, nMsg, wParam, lParam) : DefWindowProcW(hWnd, nMsg, wParam, lParam);
}

LRESULT Kusanagi::View::WinWindow::Proc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CLOSE:

		break;
	default:
		return DefWindowProcW(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}
