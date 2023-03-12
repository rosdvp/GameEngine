#pragma once

namespace BlahEngine
{
class Logger
{
public:
	Logger();
	~Logger();

	static void Print(const char* tag, const char* message, ...);
	static void Debug(const char* tag, const char* message, ...);

private:
	static Logger* _instance;

	void Log(const char* type, const char* tag, const char* text);
};
}
