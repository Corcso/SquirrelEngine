#pragma once
#include "WorldNut.h"

namespace SQ {
	class EditorService
	{
	public:

		int Init(std::string initialNutPath);

		void Shutdown();

		void RenderTopMenuBar();
		void RenderFullEditorGUI();
		void RenderViewportGUI(Mat4 LHViewMatrix, Mat4 LHProjMatrix, ImTextureID viewportImage);

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

		std::string openScenePath;
		std::string openSceneName;

		void LoadNewSceneFromFile(std::string scenePath);
		void CreateNewScene();
		void SaveCurrentScene();

		// ImGui Elements
		bool imguielement_openCreateNewScenePopup;
		char imguielement_newSceneName[64];

		bool imguielement_openSaveNewScenePopup;
		char imguielement_newScenePath[64];
	};
}