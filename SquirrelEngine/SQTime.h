#pragma once
#include <chrono>
#include "Tree.h"
namespace SQ {
	/// <summary>
	/// <para>Time Service</para>
	/// <para>Used for all things time, like delta time and time since start</para>
	/// </summary>
	class Time
	{
	public:
		friend class Tree;

		Time() {};

		/// <summary>
		/// <para>Setup time service, this is the starting point for all timers</para>
		/// </summary>
		void Init();

		/// <summary>
		/// <para>Gets deltaTime, time the last frame took from start to finish. </para>
		/// <para>Microsecond precision.
		/// </summary>
		/// <returns>Delta Time</returns>
		float Delta();

		/// <summary>
		/// <para>Gets time since start. Compares against right now so time since start may vary inside a frame. </para>
		/// <para>Millisecond precision</para>
		/// </summary>
		/// <returns>Delta Time</returns>
		float TimeSinceStart();

		
	private:
		
		void FrameStart();

		/// <summary>
		/// <para>Private frame end function, sets time points for second last and last frame</para>
		/// <para>Used by tree, tree is a friend class</para>
		/// </summary>
		void FrameEnd();

		void WaitForTargetFPS();

		std::chrono::high_resolution_clock clock;

		std::chrono::time_point<std::chrono::high_resolution_clock> frameTargetStartTime;
		std::chrono::time_point<std::chrono::high_resolution_clock> frameTargetEndTime;

		std::chrono::time_point<std::chrono::high_resolution_clock> timePointAtStart;
		std::chrono::time_point<std::chrono::high_resolution_clock> timePointLastFrame;
		std::chrono::time_point<std::chrono::high_resolution_clock> timePointSecondLastFrame;
	};

}