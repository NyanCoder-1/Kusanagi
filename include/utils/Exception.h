#pragma once

#include <exception>
#include <string>

namespace Kusanagi::Utils {

	class Exception : public std::exception {
	private:
		std::string msg;
		const char* sourceFile;
		unsigned long c, exceptionCausedLine;

	public:
		// prefered throwing: Exception("<exception_description>", <convenient_exception_code>, __FILE__, __LINE__)
		explicit Exception(const std::string &message, const unsigned long code = -1, const char* file = "<unknown>", const unsigned long line = 0);
		~Exception() throw() override {}

		const char* what() const throw() override;
		virtual unsigned long code() const noexcept;
		const char* file() const noexcept;
		virtual unsigned long line() const noexcept;
	};
}