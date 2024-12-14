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

	void Time::FrameEnd()
	{
		timePointSecondLastFrame = timePointLastFrame;
		timePointLastFrame = clock.now();
	}
}