#pragma once

class Error
{
private:

	Error();

	static void kill();

public:

	static void assert(bool condition, const char *format, ...);
	static void fatal(const char *format, ...);
};

