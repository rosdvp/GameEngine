#include "pch.h"
#include "TimeModule.h"

#include "Logger.h"

using namespace BlahEngine;

TimeModule::TimeModule() :
	_framesCount(0),
	_totalSecs(0),
	_frameDeltaSecs(0)
{}

TimeModule::~TimeModule()
{
	Logger::Debug("time module", "destroyed");
}

void TimeModule::Init()
{
	_startTime = std::chrono::steady_clock::now();
	_frameStartTime = std::chrono::steady_clock::now();

	Logger::Debug("time module", "initialized");
}

void TimeModule::Run()
{
	auto currTime = std::chrono::steady_clock::now();

	_frameDeltaSecs = std::chrono::duration_cast<std::chrono::microseconds>(currTime - _frameStartTime).count()
		/ 1000000.0f;
	_frameStartTime = currTime;

	_totalSecs = std::chrono::duration_cast<std::chrono::microseconds>(currTime - _startTime).count()
		/ 1000000.0f;

	_framesCount++;
}

unsigned TimeModule::GetFramesCount() const
{
	return _framesCount;
}

float TimeModule::GetTotalSecs() const
{
	return _totalSecs;
}

float TimeModule::GetFrameDeltaSecs() const
{
	return _frameDeltaSecs;
}
