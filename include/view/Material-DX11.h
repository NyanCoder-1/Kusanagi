#pragma once

#include "view/IMaterial.h"

namespace Kusanagi::View {
	class MaterialDX11 : public Kusanagi::View::IMaterial {
	public:
		MaterialDX11();
		~MaterialDX11() override;

		void Set() override;
	};
}