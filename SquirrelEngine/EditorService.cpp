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

	void EditorService::RenderViewportGUI(Mat4 LHViewMatrix, Mat4 LHProjMatrix, ImTextureID viewportImage)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);
        float squareImageLength = MIN(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        // Set operation for Guizmo, we do here as the buttons which change it 
        if (ImGui::Button("T")) {
            SetCurrentMovementOperation(ImGuizmo::OPERATION::TRANSLATE);
        }
        ImGui::SameLine();
        if (ImGui::Button("R")) {
            SetCurrentMovementOperation(ImGuizmo::OPERATION::ROTATE);
        }
        ImGui::SameLine();
        if (ImGui::Button("S")) {
            SetCurrentMovementOperation(ImGuizmo::OPERATION::SCALE);
        }
        ImGui::BeginChild("ViewportInterior", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
        // Render viewport
        ImGui::Image(viewportImage, ImVec2(squareImageLength, squareImageLength));

        // Gizmo Render
        if (GetCurrentlySelectedWorldNut() != nullptr) {
            // Setup Guizmo window
            ImGuizmo::SetRect(ImGui::GetWindowContentRegionMin().x + ImGui::GetWindowPos().x, ImGui::GetWindowContentRegionMin().y + ImGui::GetWindowPos().y, squareImageLength, squareImageLength);
            ImGuizmo::SetDrawlist();
            ImGuizmo::PushID(GetCurrentlySelectedWorldNut());

            // Get world matrix to edit, and delta matrix of changes
            Mat4 world = GetCurrentlySelectedWorldNut()->GetGlobalSRTWorldMatrix();
            Mat4 delta;



            // Maniuplate the world matrix by delta in that operation
            ImGuizmo::Manipulate(&(LHViewMatrix[0][0]), &(LHProjMatrix[0][0]), GetEditorService()->GetCurrentMovementOperation(), ImGuizmo::MODE::WORLD, &(world[0][0]), &(delta[0][0]));

            // Only if there is changes
            if (!(delta[0][0] == 1 && delta[0][1] == 0 && delta[0][2] == 0 && delta[0][3] == 0 &&
                delta[1][0] == 0 && delta[1][1] == 1 && delta[1][2] == 0 && delta[1][3] == 0 &&
                delta[2][0] == 0 && delta[2][1] == 0 && delta[2][2] == 1 && delta[2][3] == 0 &&
                delta[3][0] == 0 && delta[3][1] == 0 && delta[3][2] == 0 && delta[3][3] == 1)) {

                // Handle rotation 
                if (GetCurrentMovementOperation() == ImGuizmo::OPERATION::ROTATE) {
                    /*Mat4 rotationBefore = QToM4(openGizmoWorldNut->GetRotation());
                    Mat4 rotationAfter = rotationBefore * delta;
                    openGizmoWorldNut->SetRotation(M4ToQ_RH(rotationAfter));*/
                    Mat4 rotationOnly = SRTTransformToRotation(world);
                    GetCurrentlySelectedWorldNut()->SetGlobalQuaternion(M4ToQ_RH(rotationOnly));
                }
                // Handle scale
                // Please note this is bugged and will very slightly offset scale when things are skewed, should not be noticible its just compounding if doing every frame
                else if (GetCurrentMovementOperation() == ImGuizmo::OPERATION::SCALE) {
                    Vec3 newScale = SRTTransformToScale(world);
                    GetCurrentlySelectedWorldNut()->SetGlobalScale(newScale);
                }
                // Handle position
                else if (GetCurrentMovementOperation() == ImGuizmo::OPERATION::TRANSLATE) {
                    Vec3 newPosition = SRTTransformToTranslate(world);
                    GetCurrentlySelectedWorldNut()->SetGlobalPosition(newPosition);
                }

            }
            ImGuizmo::PopID();
        }
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopStyleVar();
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