#pragma once

namespace Kusanagi
{
	class Core;

	namespace View::Render
	{
		class IRender
		{
		protected:
			Kusanagi::Core *core;

		public:
			IRender() = delete;
			IRender(Kusanagi::Core *c) { core = c; }
			virtual ~IRender() {}

			virtual void Init() = 0;
			virtual void DrawBegin() = 0;
			virtual void DrawEnd() = 0;
			virtual void Resize(unsigned long width, unsigned long height) = 0;
		};
	}
}