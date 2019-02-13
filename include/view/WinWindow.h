#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "View\IWindow.h"

namespace Kusanagi::View
{
	class WinWindow : public IWindow
	{
	private:
		HWND hWnd;

	public:
		WinWindow();
		~WinWindow();

		void Create() override;
		void Update() override;

		HWND GetHandler();

	private:
		void create();

		static LRESULT __stdcall StaticProc(HWND, UINT, WPARAM, LPARAM);

	public:
		LRESULT Proc(HWND, UINT, WPARAM, LPARAM);
	};
}