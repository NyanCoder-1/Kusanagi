#pragma once

#include "utils/EventManager.h"

namespace Kusanagi::Window {
	class IWindow : public Kusanagi::Utils::EventManager {
	public:
		virtual ~IWindow() {}

		virtual void Create() = 0;
		virtual void Update() = 0;
	};
}