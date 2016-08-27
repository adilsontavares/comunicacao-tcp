#pragma once

class Console
{
private:

	Console();

public:

	static void log(const char *format, ...);
	static void warning(const char *format, ...);
	static void error(const char *format, ...);
	static void write(const char *format, ...);
	static void writeLine(const char *format, ...);
};

