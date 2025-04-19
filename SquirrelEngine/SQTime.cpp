#include "PCH.h"
#include "SQTime.h"
namespace SQ {
	void Time::Init(int targetFPS)
	{
		this->targetFPS = targetFPS;
		timePointAtStart = timePointLastFrame = clock.now();

		frameTimeHistory.resize(30);
	}

	float Time::Delta()
	{
		return (float)std::chrono::duration_cast<std::chrono::microseconds>(timePointLastFrame - timePointSecondLastFrame).count() / 1000000.0f;
	}

	float Time::TimeSinceStart()
	{
		return (float)std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - timePointAtStart).count() / 1000.0f;
	}

	void Time::ImGuiRenderDebugInfo()
	{
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Time"))
		{
			frameTimeHistory.erase(frameTimeHistory.begin());
			frameTimeHistory.push_back(Delta() * 1000.0f);
			
			ImGui::PlotLines("Frame Time", frameTimeHistory.data(), frameTimeHistory.size(), 0, nullptr, 0, 160, ImVec2{0, 60});
			ImGui::Text(("Time Elapsed: " + std::to_string(TimeSinceStart())).c_str());

			ImGui::TreePop();
		}
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
		float waitTime = (1.0f / (float)targetFPS) - frameTime;
		if (waitTime > 0) std::this_thread::sleep_for(std::chrono::microseconds((long long)(waitTime * 1000000.0f)));
	}
}