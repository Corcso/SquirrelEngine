#pragma once
#include "WorldNut.h"

namespace SQ {
	class EditorService
	{
	public:

		int Init();

		void Shutdown();

		void SetGizmoWorldNut(WorldNut* gizmoWorldNut);
		WorldNut* GetCurrentlySelectedWorldNut() { return openGizmoWorldNut; }

		void SetCurrentMovementOperation(ImGuizmo::OPERATION operation);
		ImGuizmo::OPERATION GetCurrentMovementOperation() { return currentGizmoOperation; }
	private:
		ImGuizmo::OPERATION currentGizmoOperation;
		WorldNut* openGizmoWorldNut;
	};
}