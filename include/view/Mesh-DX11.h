#pragma once

#include "view/IMesh.h"

namespace Kusanagi::View {
	class MeshDX11 : public Kusanagi::View::IMesh {
	public:
		MeshDX11(Kusanagi::Core *core);
		~MeshDX11() override;

		void Set() override;
	};
}