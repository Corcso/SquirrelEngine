#include "PCH.h"
#include "EditorService.h"

namespace SQ {
	int EditorService::Init() {
		currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		openGizmoWorldNut = nullptr;

		return 0;
	}

	void EditorService::Shutdown() {}

	void EditorService::SetGizmoWorldNut(WorldNut* gizmoWorldNut)
	{
		openGizmoWorldNut = gizmoWorldNut;
	}

	void EditorService::SetCurrentMovementOperation(ImGuizmo::OPERATION operation)
	{
		currentGizmoOperation = operation;
	}
}