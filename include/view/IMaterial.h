#pragma once

namespace Kusanagi {
	class Core;

	namespace View {
		class IMaterial {
		protected:
			Kusanagi::Core *core;

		public:
			IMaterial(Kusanagi::Core *core) : core(core) {}
			virtual ~IMaterial() = 0;

			virtual void Set() = 0;
		};
	}
}