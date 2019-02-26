#include "window/WinWindow.h"
#include "utils/Exception.h"
#include "utils/Format.h"

Kusanagi::Window::WinWindow::WinWindow() {
	keydownEventID = AddEvent("keydown");
	keyupEventID = AddEvent("keyup");
	mousemoveEventID = AddEvent("mousemove");
	activateEventID = AddEvent("activate");
	closeEventID = AddEvent("close");
	typeEventID = AddEvent("type");
}
Kusanagi::Window::WinWindow::~WinWindow() {

}

void Kusanagi::Window::WinWindow::Create() {
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

	{
		RAWINPUTDEVICE Rid[2];

		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0; RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
		Rid[0].hwndTarget = hWnd;

		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x06;
		Rid[1].dwFlags = 0; RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
		Rid[1].hwndTarget = hWnd;

		if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
			//registration failed. Call GetLastError for the cause of the error
			throw Kusanagi::Utils::Exception(Kusanagi::Utils::Format("RawInput devices registration failed with code %d", GetLastError()), 0x01000003, __FILE__, __LINE__);
	}

	ShowWindow(hWnd, SW_SHOW);
}

void Kusanagi::Window::WinWindow::Update() {
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND Kusanagi::Window::WinWindow::GetHandler() {
	return hWnd;
}

LRESULT __stdcall Kusanagi::Window::WinWindow::StaticProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
	void* ptr = reinterpret_cast<void*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	return ptr ? reinterpret_cast<Kusanagi::Window::WinWindow*>(ptr)->Proc(hWnd, nMsg, wParam, lParam) : DefWindowProcW(hWnd, nMsg, wParam, lParam);
}

LRESULT Kusanagi::Window::WinWindow::Proc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
	switch (nMsg) {
	case WM_CHAR:
		Event(typeEventID, (wchar_t)wParam);
		break;

	case WM_INPUT:
	{
		UINT dwSize;

		// Get RawInput data size
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
			break;

		// Get RawInput data
		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
			break;
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
			Event((raw->data.keyboard.Flags & 1) ? keyupEventID : keydownEventID, raw->data.keyboard.VKey);
		else if (raw->header.dwType == RIM_TYPEMOUSE) {
			if (raw->data.mouse.usButtonFlags & 0x1) Event(keydownEventID, (long)VK_LBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x4) Event(keydownEventID, (long)VK_RBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x10) Event(keydownEventID, (long)VK_MBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x40) Event(keydownEventID, (long)VK_XBUTTON1);
			if (raw->data.mouse.usButtonFlags & 0x100) Event(keydownEventID, (long)VK_XBUTTON2);
			if (raw->data.mouse.usButtonFlags & 0x2) Event(keyupEventID, (long)VK_LBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x8) Event(keyupEventID, (long)VK_RBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x20) Event(keyupEventID, (long)VK_MBUTTON);
			if (raw->data.mouse.usButtonFlags & 0x80) Event(keyupEventID, (long)VK_XBUTTON1);
			if (raw->data.mouse.usButtonFlags & 0x200) Event(keyupEventID, (long)VK_XBUTTON2);
			if (raw->data.mouse.lLastX || raw->data.mouse.lLastY)
				Event(mousemoveEventID, (long)raw->data.mouse.lLastX, (long)raw->data.mouse.lLastY);
		}

		delete[] lpb;
		break;
	}

	case WM_ACTIVATE:
		Event(activateEventID, (bool)(wParam != WA_INACTIVE));
		break;

	case WM_CLOSE:
		Event(closeEventID);
		break;
	default:
		return DefWindowProcW(hWnd, nMsg, wParam, lParam);
	}

	return 0;
}
