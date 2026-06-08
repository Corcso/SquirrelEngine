#include "PCH.h"
#include "EditorFileBrowser.h"
#include "SQUtility.h"

namespace SQ {
	EditorFileBrowser::EditorFileBrowser()
	{
		currentDirectory = ".\\Resources";
	}

	void EditorFileBrowser::Render()
	{
		const int ICON_SIZE = 64;
		const int TEXT_WRAP_X_MAX = 48;
		const int ICON_SIZE_WITH_PADDING = 80; // Just an estimate but works

		toOpenNext = "";
		ImGui::Begin("Open Folder");
		if (ImGui::Button("Up") && currentDirectory != ".\\Resources")
		{
			currentDirectory = currentDirectory.parent_path();
		}
		// Max icons per row. 
		int maxPerRow = ImGui::GetWindowSize().x / ICON_SIZE_WITH_PADDING;
		if (maxPerRow <= 0) maxPerRow = 8; // Sometimes the window size can be 0, just make it 8 per row if so.
		int tally = 0;// Used for new line of icons
		// Folder buttons first
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentDirectory))
		{
			if (tally % maxPerRow != 0) ImGui::SameLine();
			if (entry.is_directory()) 
			{
				std::string buttonNameWrapped = ImGuiTextWrap(ICON_LC_FOLDER "\n" + entry.path().filename().string(), TEXT_WRAP_X_MAX);
				if (ImGui::Button(buttonNameWrapped.c_str(), ImVec2(ICON_SIZE, ICON_SIZE)))
				{
					currentDirectory = entry.path();
				}
				tally++;
			}
		}
		// Then file buttons
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentDirectory))
		{
			if (tally % maxPerRow != 0) ImGui::SameLine();
			if (!entry.is_directory())
			{
				std::string buttonNameWrapped = ImGuiTextWrap(std::string(GetIconForExtension(entry.path().extension().string())) + "\n" + entry.path().filename().string(), TEXT_WRAP_X_MAX);
				if (ImGui::Button(buttonNameWrapped.c_str(), ImVec2(ICON_SIZE, ICON_SIZE)))
				{
					toOpenNext = entry.path().string();
					toOpenNextIsScene = entry.path().extension().string() == ".nut";
				}
				tally++;
			}
		}
		
		ImGui::End();
		ImGui::Begin("File Tree");
		RenderItemInTree(std::filesystem::directory_entry(".\\Resources"));
		ImGui::End();
	}
	std::string EditorFileBrowser::GetObjectToOpenNext()
	{
		std::replace(toOpenNext.begin(), toOpenNext.end(), '\\', '/');
		return toOpenNext;
	}
	bool EditorFileBrowser::ObjectToOpenIsScene()
	{
		return toOpenNextIsScene;
	}
	const char* EditorFileBrowser::GetIconForExtension(std::string extension)
	{
		if (extension == ".nut") return ICON_LC_PACKAGE;
		if (extension == ".obj") return ICON_LC_BOXES;
		if (extension == ".mat") return ICON_LC_ECLIPSE;
		return ICON_LC_SQUARE;
	}
	void EditorFileBrowser::RenderItemInTree(const std::filesystem::directory_entry& item)
	{
		if (item.is_directory())
		{
			bool nodeOpen = ImGui::TreeNodeEx(item.path().filename().string().c_str(), DEFAULT_TREE_FLAGS | (currentDirectory == item.path() ? ImGuiTreeNodeFlags_Selected : 0));
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				currentDirectory = item.path();
			}
			if (nodeOpen)
			{
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(item.path()))
				{
					if (entry.is_directory()) RenderItemInTree(entry);
				}
				for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(item.path()))
				{
					if (!entry.is_directory()) RenderItemInTree(entry);
				}
				ImGui::TreePop();
			}
		}
		else {
			ImGui::Text(item.path().filename().string().c_str());
		}
	}
}