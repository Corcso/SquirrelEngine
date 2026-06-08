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
                //imguielement_newSceneName[0] = '\0'; // Clear String
                //imguielement_openCreateNewScenePopup = true; Moved to after the confirmation

                imguielement_openSaveSceneConfirmation = true;
                imguielement_createAfterSaveSceneConfirmation = true;
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
                //LoadNewSceneFromFile(ConvertFullPathToRelative(GetInput()->OpenSystemFileDialogue())); Moved to after the confirmation

                imguielement_openSaveSceneConfirmation = true;
                imguielement_loadAfterSaveSceneConfirmation = true;
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
                LoadResourceFromFile(ConvertFullPathToRelative(GetInput()->OpenSystemFileDialogue()));
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
            if (fileBrowser.ObjectToOpenIsScene()) {
                //LoadNewSceneFromFile(fileBrowser.GetObjectToOpenNext()); Moved to after save confirmation

                imguielement_openSaveSceneConfirmation = true;
                imguielement_loadAfterSaveSceneConfirmation = true;
                if (fileBrowser.GetObjectToOpenNext().length() > 64) throw("Cannot open scene, file name too long.");
                memcpy(&imguielement_saveSceneConfirmationPathToLoad, fileBrowser.GetObjectToOpenNext().data(), fileBrowser.GetObjectToOpenNext().length());
            }
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
        if (openResourcePath != "") {
            // Reload the current resource from file
            std::string openResourceExtension = '.' + SplitString(openResourcePath, '.')[SplitString(openResourcePath, '.').size() - 1];
            ResourceTypeDictionary[openResourceExtension].reload(openResourcePath);
        }

        openResourceExtension = '.' + SplitString(resourcePath, '.')[SplitString(resourcePath, '.').size() - 1];
        openResource = ResourceTypeDictionary[openResourceExtension].load(resourcePath);
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
       ResourceTypeDictionary[openResourceExtension].save(*openResource.get(), openResourcePath);
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

                // Incase we came from a save confirmation, we have this here
                PerformSaveConfirmationSubsequentAction();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
        // == SAVE CONFIRMATION POPUP
        if (imguielement_openSaveSceneConfirmation)
        {
            ImGui::OpenPopup("SaveSceneConfirmation");
            imguielement_openSaveSceneConfirmation = false;
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); // Center the popup to be
        }
        if (ImGui::BeginPopupModal("SaveSceneConfirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Would you like to save your current scene?");
            ImGui::Separator();
            if (ImGui::Button("Save", ImVec2(120, 0))) {
                // Run the same logic in the menu to check wether we are saving or save-as-ing
                if (openScenePath == "") { // Save as
                    imguielement_newScenePath[0] = '\0'; // Clear String
                    imguielement_openSaveNewScenePopup = true;
                }
                else {
                    SaveCurrentScene(); // Save
                    PerformSaveConfirmationSubsequentAction();
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Save As New", ImVec2(120, 0))) {
                imguielement_newScenePath[0] = '\0'; // Clear String
                imguielement_openSaveNewScenePopup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button("Don't Save", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                PerformSaveConfirmationSubsequentAction();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                // Reset follow up action parameters, we do not what to do the thing we were going to do.
                imguielement_saveSceneConfirmationPathToLoad[0] = '\0';
                imguielement_createAfterSaveSceneConfirmation = false;
                imguielement_loadAfterSaveSceneConfirmation = false;
            }
            ImGui::EndPopup();
        }
    }

    void EditorService::PerformSaveConfirmationSubsequentAction()
    {
        // If both the below are false nothing happens, but the path is reset!
        if (imguielement_createAfterSaveSceneConfirmation) // If a new scene needs creating, create it
        {
            imguielement_newSceneName[0] = '\0'; // Clear String
            imguielement_openCreateNewScenePopup = true;
        }
        else if (imguielement_loadAfterSaveSceneConfirmation) // If a scene needs loading, load it
        {
            if (imguielement_saveSceneConfirmationPathToLoad[0] == '\0') // If no scene to load set, load 
            {
                LoadNewSceneFromFile(ConvertFullPathToRelative(GetInput()->OpenSystemFileDialogue()));
            }
            else
            {
                LoadNewSceneFromFile(imguielement_saveSceneConfirmationPathToLoad);
            }
        }
        // Reset
        imguielement_saveSceneConfirmationPathToLoad[0] = '\0';
        imguielement_createAfterSaveSceneConfirmation = false;
        imguielement_loadAfterSaveSceneConfirmation = false;
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
                openResourcePath = "./Resources/" + std::string(imguielement_newResourcePath) + openResourceExtension;
                SaveCurrentResource();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }
}