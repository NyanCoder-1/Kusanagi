#pragma once

namespace Kusanagi
{
	class Core;

	namespace Controller
	{
		class Controller
		{
		private:
			Kusanagi::Core *core;
		public:
			Controller(Kusanagi::Core *core);
		};
	}
}