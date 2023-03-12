#include "pch.h"
#include "Logger.h"

#include <iostream>

using namespace BlahEngine;

Logger* Logger::_instance = nullptr;

Logger::Logger()
{
	if (_instance == nullptr)
		_instance = this;
	else
		throw std::exception("logger already exists");
}

Logger::~Logger()
{
	_instance = nullptr;
}


void Logger::Print(const char* tag, const char* message, ...)
{
	if (_instance == nullptr)
		return;

	va_list args;
	va_start(args, message);
	int len     = _vscprintf(message, args) + 1;
	auto buffer = static_cast<char*>(malloc(len * sizeof(char)));
	vsprintf_s(buffer, len, message, args);

	_instance->Log("info", tag, buffer);

	va_end(args);
	free(buffer);
}

void Logger::Debug(const char* tag, const char* message, ...)
{
#ifdef _DEBUG
	if (_instance == nullptr)
		return;

	va_list args;
	va_start(args, message);
	int len     = _vscprintf(message, args) + 1;
	auto buffer = static_cast<char*>(malloc(len * sizeof(char)));
	vsprintf_s(buffer, len, message, args);

	_instance->Log("debug", tag, buffer);

	va_end(args);
	free(buffer);
#endif
}

void Logger::Log(const char* type, const char* tag, const char* text)
{
	printf("[%s] [%s] %s\n", type, tag, text);
}