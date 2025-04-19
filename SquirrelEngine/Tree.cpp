#include "PCH.h"
#include "Tree.h"
#include "Services.h"

// TEMP
#include "ResourceManager.h"
#include "FPVCamera.h"
#include "ShelledNut.h"
namespace SQ {
	Tree::Tree()
	{
		toQuit = false;
		rootNut.name = "ROOT";
	}

	void Tree::Init(std::string initialNutPath)
	{
		// Load the initial nut
 		std::shared_ptr<ShelledNut> myLittleScene = Services::GetResourceManager()->Retrieve<ShelledNut>(initialNutPath);
		UniquePoolPtr<Nut> sceneReady = myLittleScene->Instantiate();
		// Set as a child of root, need to get observer to call function as ownership is lost. 
		Nut* temp = sceneReady.get();
		temp->SetParent(&rootNut, std::move(sceneReady));
	}

	void Tree::RunLoop()
	{
		// Until we should quit
		while (!toQuit) {
			// Get start time (used for target FPS waiting)
			GetTime()->FrameStart();

			// Process all input
			SQ::Services::GetInput()->ProcessInput();

			// Run physics updates
			GetPhysics()->Update();

			// Run update then late update on all nuts
			Update(&rootNut);
			LateUpdate(&rootNut);

			// Begin graphics render
			Services::GetGraphics()->BeginRender();

			// Get active camera
			CameraNut* activeCamera = GetActiveCamera();
			bool usingTempCamera = false;
			if (activeCamera == nullptr) { activeCamera = new CameraNut; usingTempCamera = true; activeCamera->SetFov(70); }
			// Set camera information
			Services::GetGraphics()->UpdateProjectionMatrix(activeCamera);
			Services::GetGraphics()->SetupCameraForFrame(activeCamera);
			if (usingTempCamera) delete activeCamera;

			// Register all scene lights
			RegisterLights(&rootNut);

			// Render scene
			Render(&rootNut);

			// Debug UI for Demo
			ImGui::Begin("Info");
			Services::GetPoolAllocationService()->ImGuiPoolUsageRender();
			Services::GetTime()->ImGuiRenderDebugInfo();
			Services::GetResourceManager()->ImGuiRenderDebugInfo();
			ImGui::End();
			ImGui::Begin("Scene");
			ImGuiRenderDebugInfo();
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

			// End render and display results
			Services::GetGraphics()->EndRender();

			// Clear lights list
			Services::GetGraphics()->ClearFrameLights();

			// Update input, setting pressed keys down etc. 
			SQ::Services::GetInput()->Update();

			// Destroy any queued for destruction nuts
			DestroyQueued(&rootNut);

			// Get frame end time, used for delta time and target FPS
			GetTime()->FrameEnd();

			// Wait until target FPS reached if processing frame too fast. 
			GetTime()->WaitForTargetFPS();
		}

		// Delete all nuts 
		FreeAllNuts(&rootNut);
	}

	CameraNut* Tree::GetActiveCamera(Nut* nutToLookFrom)
	{
		if (nutToLookFrom == nullptr) nutToLookFrom = &rootNut;

		// Check if this nut is a camera and if its the active one. 
		CameraNut* cameraCast = dynamic_cast<CameraNut*>(nutToLookFrom);

		if (cameraCast != nullptr) {
			if (cameraCast->IsActiveCamera()) return cameraCast;
		}

		// Check all children if it isnt this nut
		unsigned int childCount = nutToLookFrom->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			CameraNut* camFromChild = GetActiveCamera(nutToLookFrom->GetNthChild(c));
			if (camFromChild != nullptr) return camFromChild;
		}

		// We have found no active camera below us, return nullptr
		return nullptr;
	}

	Nut* Tree::GetRootNut() {
		return &rootNut;
	}

	void Tree::Quit()
	{
		toQuit = true;
	}

	void Tree::Update(Nut* nut)
	{
		// Update this nut
		nut->Update();
		
		// Update all children
		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			Update(nut->GetNthChild(c));
		}
	}

	void Tree::LateUpdate(Nut* nut)
	{
		// Late update this nut
		nut->LateUpdate();

		// Late update all children
		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			LateUpdate(nut->GetNthChild(c));
		}
	}

	void Tree::RegisterLights(Nut* nut)
	{
		// If light, register it with the graphics service
		LightNut* lightCast = dynamic_cast<LightNut*>(nut);

		if (lightCast != nullptr) {
			Services::GetGraphics()->RegisterLightForFrame(lightCast);
		}

		// Check all children for lights. 
		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			RegisterLights(nut->GetNthChild(c));
		}
	}

	void Tree::Render(Nut* nut)
	{
		// If mesh, render it using the graphics service. 
		MeshNut* meshCast = dynamic_cast<MeshNut*>(nut);

		if (meshCast != nullptr) {
			Services::GetGraphics()->Render(meshCast);
		}

		// Check all children for meshes. 
		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			Render(nut->GetNthChild(c));
		}
	}

	bool Tree::DestroyQueued(Nut* nut)
	{
		// Loop first, then act on delete from the children up
		//unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < nut->GetChildCount(); ++c) {
			if(DestroyQueued(nut->GetNthChild(c))) --c;
		}

		// If queued to go, gain ownership, and then let it fall out of scope
		if (nut->IsQueuedForDestruction()) {
			UniquePoolPtr<Nut> finalOwner = nut->TakeOwnership();
			// If we are opening this nut with the inspector, close the inspector.
			if (currentInspectorTarget == finalOwner.get()) currentInspectorTarget = nullptr;
			return true;
		}
		return false;
	}
	bool Tree::FreeAllNuts(Nut* nut)
	{
		// Loop first, then act on delete from the children up
		
		for (unsigned int c = 0; c < nut->GetChildCount(); ++c) {
			if(FreeAllNuts(nut->GetNthChild(c))) --c;
		}

		// To delete, gain ownership, and then let it fall out of scope
		// Dont delete root nut
		if(nut != &rootNut)
		{
			UniquePoolPtr<Nut> finalOwner = nut->TakeOwnership();
			return true;
		}
		return false;
	}
	void Tree::ImGuiRenderDebugInfo()
	{
		ImGui::SetNextItemOpen(true);
		ImGuiRenderTreeNut(&rootNut);
	}
	void Tree::ImGuiRenderTreeNut(Nut* nut)
	{
		if (ImGui::TreeNodeEx(nut->name.c_str(), ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::Button("Open In Inspector")) {
				currentInspectorTarget = nut;
			}
			unsigned int childCount = nut->GetChildCount();
			for (unsigned int c = 0; c < childCount; ++c) {
				ImGuiRenderTreeNut(nut->GetNthChild(c));
			}
			ImGui::TreePop();
		}
	}

}