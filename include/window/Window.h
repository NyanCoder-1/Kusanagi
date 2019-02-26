#pragma once

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "window/WinWindow.h"
namespace Kusanagi::Window {
	typedef Kusanagi::Window::WinWindow Window;
}
#endif // defined(_WIN32)