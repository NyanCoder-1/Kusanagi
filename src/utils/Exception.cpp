#include "utils/Exception.h"


Kusanagi::Utils::Exception::Exception(const std::string &message, const unsigned long code, const char* file, const unsigned long line) : c(code), msg(message), sourceFile(file), exceptionCausedLine(line) {}

const char *Kusanagi::Utils::Exception::what() const throw() {
	return msg.c_str();
}
unsigned long Kusanagi::Utils::Exception::code() const throw() {
	return c;
}

const char * Kusanagi::Utils::Exception::file() const noexcept
{
	return sourceFile;
}

unsigned long Kusanagi::Utils::Exception::line() const noexcept
{
	return exceptionCausedLine;
}
