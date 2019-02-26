#include "view/View.h"

#include "Core.h"
#include "view/render/Render.h"

Kusanagi::View::View::View(Kusanagi::Core *c) : core(c)
{
	render = new Kusanagi::View::Render::Render(core);
	render->Init();
}

Kusanagi::View::View::~View()
{
	if (render)
		delete render;
}

void Kusanagi::View::View::Update()
{
	render->DrawBegin();

	render->DrawEnd();
}