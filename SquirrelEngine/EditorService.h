#pragma once
#include "WorldNut.h"

namespace SQ {
	class EditorService
	{
	public:

		int Init();

		void Shutdown();

		void RenderFullEditorGUI();

		void SetOpenInspectorNut(Nut* nut);
		Nut* GetOpenInspectorNut() { return currentInspectorTarget; }

		void SetGizmoWorldNut(WorldNut* gizmoWorldNut);
		WorldNut* GetCurrentlySelectedWorldNut() { return openGizmoWorldNut; }

		void SetCurrentMovementOperation(ImGuizmo::OPERATION operation);
		ImGuizmo::OPERATION GetCurrentMovementOperation() { return currentGizmoOperation; }
	private:
		ImGuizmo::OPERATION currentGizmoOperation;
		WorldNut* openGizmoWorldNut;
		Nut* currentInspectorTarget;
	};
}