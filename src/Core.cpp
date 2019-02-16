#include "Core.h"
#include "window/Window.h"
#include "model/Model.h"
#include "view/View.h"
#include "controller/Controller.h"

Kusanagi::Core::Core()
{
	// init thing
	running = true;
	wnd = new Kusanagi::Window::Window();
	wnd->AddEventListener("close", new Kusanagi::Window::WinWindow::EventClose([=](void* sender)->void {
		this->running = false;
	}));
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

Kusanagi::Window::IWindow *Kusanagi::Core::GetWindow() const
{
	return wnd;
}
