#pragma once

#ifndef ENTRY
#include "Core.h"

int main()
{
	Kusanagi::Core *core = new Kusanagi::Core();

	return core->Run();
}
#endif