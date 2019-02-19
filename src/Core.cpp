#include "Core.h"
#include "window/Window.h"
#include "model/Model.h"
#include "view/View.h"
#include "controller/Controller.h"
#include "utils/Format.h"
#include <thread>

Kusanagi::Core::Core()
{
	// init thing
	running = true;
	wnd = new Kusanagi::Window::Window();
	wnd->AddEventListener("close", new Kusanagi::Window::WinWindow::EventClose([=](void* sender)->void {
		Terminate();
	}));
	wnd->AddEventListener("keydown", new Kusanagi::Window::WinWindow::EventKey([=](void* sender, long key)->void {
		// SetWindowTextA(reinterpret_cast<Kusanagi::Window::Window*>(wnd)->GetHandler(), Kusanagi::Utils::Format("keydown %d", key).c_str());
	}));
	wnd->AddEventListener("keyup", new Kusanagi::Window::WinWindow::EventKey([=](void* sender, long key)->void {
		// SetWindowTextA(reinterpret_cast<Kusanagi::Window::Window*>(wnd)->GetHandler(), Kusanagi::Utils::Format("keyup %d", key).c_str());
	}));
	wnd->AddEventListener("mousemove", new Kusanagi::Window::WinWindow::EventMouseMove([=](void* sender, long x, long y)->void {
		SetWindowTextA(reinterpret_cast<Kusanagi::Window::Window*>(wnd)->GetHandler(), Kusanagi::Utils::Format("mousemove %d %d", x, y).c_str());
	}));
	wnd->AddEventListener("type", new Kusanagi::Window::WinWindow::EventType([=](void* sender, wchar_t ch)->void {
		// SetWindowTextW(reinterpret_cast<Kusanagi::Window::Window*>(wnd)->GetHandler(), Kusanagi::Utils::Format(L"type %c", ch).c_str());
	}));
	wnd->AddEventListener("activate", new Kusanagi::Window::WinWindow::EventActivate([=](void* sender, bool state)->void {
		// SetWindowTextA(reinterpret_cast<Kusanagi::Window::Window*>(wnd)->GetHandler(), state ? "activate" : "deactivate");
	}));
	// we should call IWindow::Create after adding 'create' event listener
	wnd->Create();

	model = new Kusanagi::Model::Model(this);
	view = new Kusanagi::View::View(this);
	controller = new Kusanagi::Controller::Controller(this);
}

Kusanagi::Core::~Core()
{
	if (view)
		delete view;
	if (controller)
		delete controller;
	if (model)
		delete model;
	if (wnd)
		delete wnd;
}

int Kusanagi::Core::Run()
{
	// put main loop(s) here
	std::thread modelThread([=]() { while (running) model->Update(); }),
		viewThread([=]() { while (running) view->Update(); });
	while (running)
		wnd->Update();

	if (modelThread.joinable())
		modelThread.join();
	if (viewThread.joinable())
		viewThread.join();

	return 0;
}

void Kusanagi::Core::Terminate()
{
	running = false;
}

Kusanagi::Window::IWindow *Kusanagi::Core::GetWindow() const
{
	return wnd;
}
