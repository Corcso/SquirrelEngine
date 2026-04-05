#pragma once
#include "PCH.h"
#include "Resource.h"
#include "WorldNut.h"
#include "EditorFileBrowser.h"

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

		std::string openResourcePath;
		std::shared_ptr<Resource> openResource;
		std::string openResourceExtension;

		void LoadNewSceneFromFile(std::string scenePath);
		void CreateNewScene();
		void SaveCurrentScene();

		void LoadResourceFromFile(std::string resourcePath);
		void CreateNewResource();
		void SaveCurrentResource();

		// ImGui Elements
		EditorFileBrowser fileBrowser;

		void DisplayScenePopups();

		bool imguielement_openCreateNewScenePopup;
		char imguielement_newSceneName[64];

		bool imguielement_openSaveNewScenePopup;
		char imguielement_newScenePath[64];

		void DisplayResourcePopups();

		const char* imguielement_resourceTypesText[2] = { "Collision Shape", "StandardMaterial"}; // TODO Move to ser type dict of some sort like deserializing nodes
		const char* imguielement_resourceTypesExt[2] = { ".shape", ".mat"};
		int imguielement_resourceTypeSelected = 0;

		bool imguielement_openCreateNewResourcePopup;
		char imguielement_newResourceName[64];

		bool imguielement_openSaveNewResourcePopup;
		char imguielement_newResourcePath[64];
	};
}