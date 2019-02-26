#pragma once

#if defined(_WIN32)
#if defined(DX11)
#include "view/render/Render-DX11.h"
namespace Kusanagi::View::Render {
	typedef Kusanagi::View::Render::RenderDX11 Render;
}
#endif // defined(DX11)
#endif // defined(_WIN32)