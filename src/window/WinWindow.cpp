#include "window/WinWindow.h"
#include "utils/Exception.h"

Kusanagi::Window::WinWindow::WinWindow()
{

}
Kusanagi::Window::WinWindow::~WinWindow()
{

}

void Kusanagi::Window::WinWindow::Create()
{
	// Регистрация класса
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Kusanagi::Window::WinWindow::StaticProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandleW(nullptr);
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Kusanagi Window";
	wcex.hIconSm = nullptr;
	if (!RegisterClassEx(&wcex))
		throw Kusanagi::Utils::Exception("Can't register window class", 0x01000001, __FILE__, __LINE__);

	// Создание окна
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindowExW(0L, L"Kusanagi Window", L"Kusanagi", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wcex.hInstance, NULL);
	if (!hWnd)
		throw Kusanagi::Utils::Exception("Can't create window", 0x01000002, __FILE__, __LINE__);
	SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_SHOW);
}

void Kusanagi::Window::WinWindow::Update()
{
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Kusanagi::Window::WinWindow::GetHandler()
{
	return hWnd;
}

LRESULT __stdcall Kusanagi::Window::WinWindow::StaticProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	void* ptr = reinterpret_cast<void*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	return ptr ? reinterpret_cast<Kusanagi::Window::WinWindow*>(ptr)->Proc(hWnd, nMsg, wParam, lParam) : DefWindowProcW(hWnd, nMsg, wParam, lParam);
}

LRESULT Kusanagi::Window::WinWindow::Proc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
	case WM_CLOSE:
		Event("close");
		break;
	default:
		return DefWindowProcW(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}
