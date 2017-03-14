#pragma once

class TimeManager
{
public :
	TimeManager() : lastFrame(0)
	{}

	TimeManager(double startTime) : lastFrame(startTime)
	{}

	void UpdateTime(double currentTime)
	{
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
	}

	double GetDeltaTime()
	{
		return deltaTime;
	}

private:
	double deltaTime;
	double lastFrame;
};