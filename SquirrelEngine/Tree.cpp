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
		std::unique_ptr<Nut> sceneReady = myLittleScene->Instantiate();
		sceneReady->SetParent(&rootNut);
		sceneReady.release();
	}

	void Tree::RunLoop()
	{
		while (!toQuit) {
			SQ::Services::GetInput()->ProcessInput();

			Update(&rootNut);

			Services::GetGraphics()->BeginRender();

			CameraNut* activeCamera = GetActiveCamera();
			bool usingTempCamera = false;
			if (activeCamera == nullptr) { activeCamera = new CameraNut; usingTempCamera = true; activeCamera->SetFov(70); }
			Services::GetGraphics()->UpdateProjectionMatrix(activeCamera);
			Services::GetGraphics()->SetupCameraForFrame(activeCamera);
			if (usingTempCamera) delete activeCamera;

			RegisterLights(&rootNut);
			Render(&rootNut, Scale(V3(1, 1, 1)));

			Services::GetGraphics()->EndRender();
			Services::GetGraphics()->ClearFrameLights();

			SQ::Services::GetInput()->Update();
		}
	}

	CameraNut* Tree::GetActiveCamera(Nut* nutToLookFrom)
	{
		if (nutToLookFrom == nullptr) nutToLookFrom = &rootNut;

		CameraNut* cameraCast = dynamic_cast<CameraNut*>(nutToLookFrom);

		if (cameraCast != nullptr) {
			if (cameraCast->IsActiveCamera()) return cameraCast;
		}

		unsigned int childCount = nutToLookFrom->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			CameraNut* camFromChild = GetActiveCamera(nutToLookFrom->GetNthChild(c));
			if (camFromChild != nullptr) return camFromChild;
		}

		// We have found no active camera below us, return nullptr
		return nullptr;
	}

	void Tree::Quit()
	{
		toQuit = true;
	}

	void Tree::Update(Nut* nut)
	{
		nut->Update();
		unsigned int childCount = nut->GetChildCount();

		for (unsigned int c = 0; c < childCount; ++c) {
			Update(nut->GetNthChild(c));
		}
	}

	void Tree::RegisterLights(Nut* nut)
	{
		LightNut* lightCast = dynamic_cast<LightNut*>(nut);

		if (lightCast != nullptr) {
			Services::GetGraphics()->RegisterLightForFrame(lightCast);
		}

		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			RegisterLights(nut->GetNthChild(c));
		}
	}

	void Tree::Render(Nut* nut, Mat4 worldMatrixSoFar)
	{
		MeshNut* meshCast = dynamic_cast<MeshNut*>(nut);
		Mat4 worldMatrix = worldMatrixSoFar;

		if (meshCast != nullptr) {
			worldMatrix = worldMatrix * meshCast->GetSRTWorldMatrix();
			Services::GetGraphics()->Render(meshCast, worldMatrix);
		}

		unsigned int childCount = nut->GetChildCount();
		for (unsigned int c = 0; c < childCount; ++c) {
			Render(nut->GetNthChild(c), worldMatrix);
		}
	}
}