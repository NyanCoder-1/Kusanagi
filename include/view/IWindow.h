#pragma once

namespace Kusanagi::View
{
	class IWindow
	{
	public:
		virtual void Create() = 0;
		virtual void Update() = 0;
	};
}