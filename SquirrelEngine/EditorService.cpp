#include "PCH.h"
#include "EditorService.h"
#include "Services.h"

namespace SQ {
	int EditorService::Init() {
		currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		openGizmoWorldNut = nullptr;

		return 0;
	}

	void EditorService::Shutdown() {}

	void EditorService::RenderFullEditorGUI()
	{
		ImGui::Begin("Info");
		Services::GetPoolAllocationService()->ImGuiPoolUsageRender();
		Services::GetTime()->ImGuiRenderDebugInfo();
		Services::GetResourceManager()->ImGuiRenderDebugInfo();
		ImGui::End();
		ImGui::Begin("Scene");
		Services::GetTree()->ImGuiRenderDebugInfo();
		ImGui::End();
		ImGui::Begin("Physics");
		Services::GetPhysics()->ImGuiRenderDebugInfo();
		ImGui::End();
		ImGui::Begin("Input");
		Services::GetInput()->ImGuiRenderDebugInfo();
		ImGui::End();
		ImGui::Begin("Inspector");
		if (currentInspectorTarget != nullptr) {
			currentInspectorTarget->ImGuiRenderMyInspector();
			
		}
		ImGui::End();
		/*ImGui::Begin("Resource");
		if (currentResourceInspectorTarget != nullptr) {
			currentResourceInspectorTarget->ImGuiRenderMyInspector();
		}
		ImGui::End();*/
	}

	void EditorService::SetOpenInspectorNut(Nut* nut)
	{
		currentInspectorTarget = nut;
		if (dynamic_cast<WorldNut*>(currentInspectorTarget) != nullptr) {
			SetGizmoWorldNut(dynamic_cast<WorldNut*>(currentInspectorTarget));
		}
		else SetGizmoWorldNut(nullptr);
	}

	void EditorService::SetGizmoWorldNut(WorldNut* gizmoWorldNut)
	{
		openGizmoWorldNut = gizmoWorldNut;
	}

	void EditorService::SetCurrentMovementOperation(ImGuizmo::OPERATION operation)
	{
		currentGizmoOperation = operation;
	}
}