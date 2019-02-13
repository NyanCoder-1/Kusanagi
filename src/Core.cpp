#include "Core.h"

Kusanagi::Core::Core()
{
	// init thing
}

int Kusanagi::Core::Run()
{
	// put main loop here

	while (running)
	{
		window->Update();
	}

	return 0;
}