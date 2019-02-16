#include "Core.h"
#include "window/IWindow.h"
#ifdef _WIN32
#include "window/WinWindow.h"
#endif

Kusanagi::Core::Core()
{
	// init thing
	running = true;
#ifdef _WIN32
	wnd = new Kusanagi::Window::WinWindow();
	wnd->AddEventListener("close", new Kusanagi::Window::WinWindow::EventClose([=](void* sender)->void {
		this->running = false;
	}));
#endif
	// we should call IWindow::Create after adding 'create' event listener
	wnd->Create();
}

int Kusanagi::Core::Run()
{
	// put main loop here

	while (running)
	{
		wnd->Update();
	}

	return 0;
}

Kusanagi::Window::IWindow * Kusanagi::Core::GetWindow() const
{
	return wnd;
}
