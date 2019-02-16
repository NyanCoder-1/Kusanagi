#pragma once

namespace Kusanagi
{
	namespace Window
	{
		class IWindow;
	}
	class Core
	{
	private:
		bool running;
		Kusanagi::Window::IWindow *wnd;

	public:
		Core();

		int Run();

	public:
		Kusanagi::Window::IWindow *GetWindow() const;
	};
}