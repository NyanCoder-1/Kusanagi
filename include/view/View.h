#pragma once

namespace Kusanagi
{
	class Core;

	namespace View
	{
		class View
		{
		private:
			Kusanagi::Core *core;

		public:
			View(Kusanagi::Core *core);
		};
	}
}