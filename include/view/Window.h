#pragma once

namespace Kusanagi::View
{
	class Window
	{
	private:
		HWND hWnd;

	public:
		Window();
		~Window();

		HWND GetHandler();

	private:
		void create();

		static LRESULT __stdcall StaticProc(HWND, UINT, WPARAM, LPARAM);

	public:
		LRESULT Proc(HWND, UINT, WPARAM, LPARAM);
	};
}