#pragma once

#include <string>

namespace Kusanagi::Utils {
	std::string Format(const std::string fmt_str, ...);
	std::wstring Format(const std::wstring fmt_str, ...);
}