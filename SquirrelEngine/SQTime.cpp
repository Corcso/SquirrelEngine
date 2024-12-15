#include "PCH.h"
#include "SQTime.h"
namespace SQ {
	void Time::Init()
	{
		timePointAtStart = timePointLastFrame = clock.now();
	}

	float Time::Delta()
	{
		return (float)std::chrono::duration_cast<std::chrono::microseconds>(timePointLastFrame - timePointSecondLastFrame).count() / 1000000.0f;
	}

	float Time::TimeSinceStart()
	{
		return (float)std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - timePointAtStart).count() / 1000.0f;
	}

	void Time::FrameStart()
	{
		frameTargetStartTime = clock.now();
	}

	void Time::FrameEnd()
	{
		timePointSecondLastFrame = timePointLastFrame;
		timePointLastFrame = clock.now();
		frameTargetEndTime = clock.now();
	}
	void Time::WaitForTargetFPS()
	{
		float frameTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(timePointLastFrame - timePointSecondLastFrame).count() / 1000000.0f;
		float waitTime = (1.0f / 120.0f) - frameTime;
		if (waitTime > 0) std::this_thread::sleep_for(std::chrono::microseconds((long long)(waitTime * 1000000.0f)));
	}
}