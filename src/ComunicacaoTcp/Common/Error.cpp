#include "Error.h"
#include "Console.h"
#include <iostream>
#include <stdarg.h>

using namespace std;

Error::Error()
{
}

void Error::assert(bool condition, const char * format, ...)
{
	if (!condition)
	{
		cerr << "ASSERTION FAILED: ";

		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);

		kill();
	}
}

void Error::fatal(const char * format, ...)
{
	cerr << "FATAL ERROR: ";

	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	kill();
}

void Error::kill()
{
	fflush(stdin);
	getchar();
	exit(1);
}