#include "PCH.h"
#include "EditorService.h"
#include "Services.h"
#include <fstream>
#include "ShelledNut.h"

#include "SerializationTypeDictionary.h"

namespace SQ {
	int EditorService::Init(std::string initialNutPath) {
		currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		openGizmoWorldNut = nullptr;
        currentInspectorTarget = nullptr;

        // Set initial path and name of the scene loaded
        openScenePath = initialNutPath;
        
        std::ifstream file(initialNutPath, std::istream::in);
        if (!file.is_open()) throw - 1;

        openSceneName = nlohmann::json::parse(file)["name"];

        file.close();

		return 0;
	}

	void EditorService::Shutdown() {}

    void EditorService::RenderTopMenuBar()
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu(ICON_LC_SQUIRREL))
        {
            ImGui::Text("Thanks for trying out Squirrel Engine");
            ImGui::Text("- Cormac");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem(ICON_LC_PACKAGE_PLUS " New")) {
                imguielement_newSceneName[0] = '\0'; // Clear String
                imguielement_openCreateNewScenePopup = true;
            }

            if (ImGui::MenuItem(ICON_LC_SAVE " Save")) {
                if (openScenePath == "") {
                    imguielement_newScenePath[0] = '\0'; // Clear String
                    imguielement_openSaveNewScenePopup = true;
                }
                else SaveCurrentScene();
            }
            if (ImGui::MenuItem(ICON_LC_SAVE " Save As")) {
                imguielement_newScenePath[0] = '\0'; // Clear String
                imguielement_openSaveNewScenePopup = true;
            }
            if (ImGui::MenuItem(ICON_LC_PACKAGE_OPEN " Load")) {
                LoadNewSceneFromFile(GetInput()->OpenSystemFileDialogue());
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Resource"))
        {
            if (ImGui::MenuItem(ICON_LC_SQUARE_PLUS " New")) {
                imguielement_newResourceName[0] = '\0'; // Clear String
                imguielement_openCreateNewResourcePopup = true;
            }

            if (ImGui::MenuItem(ICON_LC_SAVE " Save")) {
                if (openResourcePath == "") {
                    imguielement_newResourcePath[0] = '\0'; // Clear String
                    imguielement_openSaveNewResourcePopup = true;
                }
                else SaveCurrentResource();
            }
            if (ImGui::MenuItem(ICON_LC_SAVE " Save As")) {
                imguielement_newResourcePath[0] = '\0'; // Clear String
                imguielement_openSaveNewResourcePopup = true;
            }
            if (ImGui::MenuItem(ICON_LC_SQUARE_ARROW_UP " Load")) {
                LoadResourceFromFile(GetInput()->OpenSystemFileDialogue());
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        DisplayScenePopups();
        DisplayResourcePopups();
    }

    void EditorService::RenderFullEditorGUI()
	{
		ImGui::Begin("Info");
		Services::GetPoolAllocationService()->ImGuiPoolUsageRender();
		Services::GetTime()->ImGuiRenderDebugInfo();
		Services::GetResourceManager()->ImGuiRenderDebugInfo();
		ImGui::End();
		ImGui::Begin("Scene");
		Services::GetTree()->ImGuiRenderDebugInfo(GetTree()->GetRootNut()->GetNut(openSceneName));
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

        fileBrowser.Render();
        if (fileBrowser.GetObjectToOpenNext() != "") {
            if (fileBrowser.ObjectToOpenIsScene()) LoadNewSceneFromFile(fileBrowser.GetObjectToOpenNext());
            else LoadResourceFromFile(fileBrowser.GetObjectToOpenNext());
        }
        ImGui::Begin("Resource");
		if (openResource.get() != nullptr) {
            openResource->ImGuiRenderMyInspector();
		}
		ImGui::End();
	}

	void EditorService::RenderViewportGUI(Mat4 LHViewMatrix, Mat4 LHProjMatrix, ImTextureID viewportImage)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);
        float squareImageLength = MIN(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        // Set operation for Guizmo, we do here as the buttons which change it 
        if (ImGui::Button(ICON_LC_MOVE_3D)) {
            SetCurrentMovementOperation(ImGuizmo::OPERATION::TRANSLATE);
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_LC_ROTATE_3D)) {
            SetCurrentMovementOperation(ImGuizmo::OPERATION::ROTATE);
        }
        ImGui::SameLine();
        if (ImGui::Button(ICON_LC_SCALE_3D)) {
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
    void EditorService::LoadNewSceneFromFile(std::string scenePath)
    {
        // Mark Old Scene for deletion
        GetTree()->GetRootNut()->GetNut(openSceneName)->QueueDestroy();

        // Stop all inspections
        openGizmoWorldNut = nullptr;
        currentInspectorTarget = nullptr;

        openScenePath = scenePath;

        std::ifstream file(scenePath, std::istream::in);
        if (!file.is_open()) {
            openScenePath = "";
            openSceneName = "";
            return;
        }

        openSceneName = nlohmann::json::parse(file)["name"];

        file.close();

        std::shared_ptr<ShelledNut> newScene = Services::GetResourceManager()->Retrieve<ShelledNut>(scenePath);
        UniquePoolPtr<Nut> sceneReady = newScene->Instantiate();
        // Set as a child of root, need to get observer to call function as ownership is lost. 
        Nut* temp = sceneReady.get();
        temp->SetParent(GetTree()->GetRootNut(), std::move(sceneReady));
    }
    void EditorService::CreateNewScene()
    {
        // Mark Old Scene for deletion
        GetTree()->GetRootNut()->GetNut(openSceneName)->QueueDestroy();

        // Stop all inspections
        openGizmoWorldNut = nullptr;
        currentInspectorTarget = nullptr;

        // Create a new 3D Node with the name chosen
        openScenePath = "";
        openSceneName = imguielement_newSceneName;

        UniquePoolPtr<WorldNut> sceneReady = GetPoolAllocationService()->MakeUniquePoolPtr<WorldNut>();
        // Set as a child of root, need to get observer to call function as ownership is lost. 
        sceneReady->name = openSceneName;
        WorldNut* temp = sceneReady.get();
        temp->SetParent(GetTree()->GetRootNut(), std::move(sceneReady.DynamicUniquePoolPtrCast<Nut>()));
    }
    void EditorService::SaveCurrentScene()
    {
        nlohmann::json data = GetTree()->GetRootNut()->GetNut(openSceneName)->Serialize();
        std::ofstream o(openScenePath);
        o << std::setw(4) << data << std::endl;
        o.close();
    }

    void EditorService::LoadResourceFromFile(std::string resourcePath)
    {
        std::string openResourceExtension = '.' + SplitString(resourcePath, '.')[SplitString(resourcePath, '.').size() - 1];
        openResource = std::shared_ptr<Resource>(ResourceTypeDictionary[openResourceExtension].load(resourcePath));
        openResourcePath = resourcePath;
    }

    void EditorService::CreateNewResource()
    {
        openResourceExtension = imguielement_resourceTypesExt[imguielement_resourceTypeSelected];
        openResource = std::shared_ptr<Resource>(ResourceTypeDictionary[openResourceExtension].create());
        openResourcePath = "";
    }

    void EditorService::SaveCurrentResource()
    {
        ;
    }

    void EditorService::DisplayScenePopups()
    {
        // == CREATE NEW SCENE POPUP ==
        if (imguielement_openCreateNewScenePopup) {
            ImGui::OpenPopup("EditorCreateNewScene");
            imguielement_openCreateNewScenePopup = false;
        }
        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("EditorCreateNewScene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Please enter a name for the new scene.");
            ImGui::Separator();

            ImGui::InputText("Scene Name", imguielement_newSceneName, 64);

            if (ImGui::Button("Create", ImVec2(120, 0))) {
                CreateNewScene();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        // == SAVE NEW SCENE POPUP ==
        if (imguielement_openSaveNewScenePopup) {
            ImGui::OpenPopup("EditorSaveNewScene");
            imguielement_openSaveNewScenePopup = false;
        }
        // Always center this window when appearing
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("EditorSaveNewScene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter a scene path, relative to ./Resources/ (.nut not needed)");
            ImGui::Text("Please note, if that scene already exists, it will be overwritten.");
            ImGui::Separator();

            ImGui::InputText("Scene Path", imguielement_newScenePath, 64);

            if (ImGui::Button("Save", ImVec2(120, 0)) && imguielement_newScenePath[0] != '\0') {
                openScenePath = "./Resources/" + std::string(imguielement_newScenePath) + ".nut";
                SaveCurrentScene();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }
    void EditorService::DisplayResourcePopups()
    {
        // == CREATE NEW RESOURCE POPUP ==
        if (imguielement_openCreateNewResourcePopup) {
            ImGui::OpenPopup("EditorCreateNewResource");
            imguielement_openCreateNewResourcePopup = false;
        }
        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("EditorCreateNewResource", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Please enter a name for the new resource.");
            ImGui::Separator();

            ImGui::InputText("Name", imguielement_newSceneName, 64);
            ImGui::Combo("Type", &imguielement_resourceTypeSelected, imguielement_resourceTypesText, 2);

            if (ImGui::Button("Create", ImVec2(120, 0))) {
                CreateNewResource();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        // == SAVE NEW SCENE POPUP ==
        if (imguielement_openSaveNewResourcePopup) {
            ImGui::OpenPopup("EditorSaveNewResource");
            imguielement_openSaveNewResourcePopup = false;
        }
        // Always center this window when appearing
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("EditorSaveNewResource", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter a resource path, relative to ./Resources/ (extension not needed)");
            ImGui::Text("Please note, if that resource already exists, it will be overwritten.");
            ImGui::Separator();

            ImGui::InputText("Path", imguielement_newResourcePath, 64);

            if (ImGui::Button("Save", ImVec2(120, 0)) && imguielement_newResourcePath[0] != '\0') {
                //openResourcePath = "./Resources/" + std::string(imguielement_newResourcePath);
                //SaveCurrentScene();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }
}