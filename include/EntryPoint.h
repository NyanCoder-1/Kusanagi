#pragma once

#ifndef ENTRY
#include <string>
#include "Core.h"
#include "utils\Exception.h"
#include "utils\Format.h"
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // MessageBox
#endif

int main()
try {
	Kusanagi::Core core;

	return core.Run();
}
catch (const Kusanagi::Utils::Exception &e)
{
#if defined(_WIN32)
	MessageBoxA(nullptr, Kusanagi::Utils::Format("%s\n\nError code: 0x%08X\nSource: %s:%d", e.what(), e.code(), e.file(), e.line()).c_str(), "Error", 0);
#endif
	return -1;
}
catch (...)
{
#if defined(_WIN32)
	MessageBoxA(nullptr, "Unhandled exception", "Error", 0);
#endif
	return -2;
}
#endif