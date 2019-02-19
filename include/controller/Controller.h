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

			void KeyDown(long key);
			void KeyUp(long key);
			void MouseMove(long x, long h);
			void Type(wchar_t ch);
			void Activate(bool state);
		};
	}
}