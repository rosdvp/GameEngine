#pragma once

namespace BlahEngine
{
class TimeModule
{
public:
	TimeModule();
	~TimeModule();

	void Init();

	void Run();

	unsigned int GetFramesCount() const;
	float GetTotalSecs() const;
	float GetFrameDeltaSecs() const;

private:
	std::chrono::time_point<std::chrono::steady_clock> _startTime;
	std::chrono::time_point<std::chrono::steady_clock> _prevFrameTime;

	unsigned int _framesCount;
	float _totalSecs;
	float _frameDeltaSecs;
};
}
