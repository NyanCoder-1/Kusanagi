#pragma once

#include "EntryPoint.h"
#include "Core.h"
#include "model/Model.h"
#include "view/View.h"
#include "controller/Controller.h"


#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if defined(DX11)
#include "view/render/Render-DX11.h"
namespace Kusanagi::View::Render
{
	typedef Kusanagi::View::Render::RenderDX11 Render;
}
#endif // defined(DX11)
#endif // defined(_WIN32)