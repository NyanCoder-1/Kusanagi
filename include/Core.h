#pragma once

namespace Kusanagi
{
	namespace Window
	{
		class IWindow;
	}
	namespace Model
	{
		class Model;
	}
	namespace View
	{
		class View;
	}
	namespace Controller
	{
		class Controller;
	}

	class Core
	{
	private:
		bool running;
		Kusanagi::Window::IWindow *wnd;
		Kusanagi::Model::Model *model;
		Kusanagi::View::View *view;
		Kusanagi::Controller::Controller *controller;

	public:
		Core();
		~Core();

		int Run();
		void Terminate();

	public:
		Kusanagi::Window::IWindow *GetWindow() const;
	};
}