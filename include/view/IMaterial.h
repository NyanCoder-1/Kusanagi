#pragma once

namespace Kusanagi {
	class Core;

	namespace View {
		class IMaterial {
		protected:
			Kusanagi::Core *core;

		public:
			IMaterial() = delete;
			IMaterial(Kusanagi::Core *_core) : core(_core) {}
			virtual ~IMaterial() {}

			virtual void Set() = 0;
		};
	}
}