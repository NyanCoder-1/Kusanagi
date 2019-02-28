#pragma once

namespace Kusanagi {
	class Core;

	namespace View {
		class IMesh {
		protected:
			Kusanagi::Core *core;

		public:
			IMesh(Kusanagi::Core *core) : core(core) {}
			virtual ~IMesh() = 0;

			virtual void Set() = 0;
		};
	}
}