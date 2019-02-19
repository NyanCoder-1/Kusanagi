#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "window/IWindow.h"
#include "utils/EventManager.h"

namespace Kusanagi::Window
{
	class WinWindow : public IWindow
	{
	public:
		// EventTypes
		typedef Kusanagi::Utils::EventListenerTemplate<> EventClose;
		typedef Kusanagi::Utils::EventListenerTemplate<long> EventKey;
		typedef Kusanagi::Utils::EventListenerTemplate<long, long> EventMouseMove;
		typedef Kusanagi::Utils::EventListenerTemplate<wchar_t> EventType;
		typedef Kusanagi::Utils::EventListenerTemplate<bool> EventActivate;

	private:
		HWND hWnd;

	public:
		WinWindow();
		~WinWindow();

		void Create() override;
		void Update() override;

		HWND GetHandler();

	private:
		static LRESULT __stdcall StaticProc(HWND, UINT, WPARAM, LPARAM);

	public:
		LRESULT Proc(HWND, UINT, WPARAM, LPARAM);
	};
}