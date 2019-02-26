#pragma once

namespace Kusanagi {
	class Core;

	namespace View {
		namespace Render {
			class IRender;
		}
		class View {
		private:
			Kusanagi::Core *core;
			Kusanagi::View::Render::IRender *render;

		public:
			View(Kusanagi::Core *core);
			~View();

			void Update();
		};
	}
}