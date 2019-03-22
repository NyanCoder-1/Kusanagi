#pragma once

namespace Kusanagi {
	class Core;

	namespace View {
		class IMesh {
		protected:
			Kusanagi::Core *core;

		public:
			IMesh() = delete;
			IMesh(Kusanagi::Core *_core) : core(_core) {}
			virtual ~IMesh() {}

			virtual void Set() = 0;
		};
	}
}