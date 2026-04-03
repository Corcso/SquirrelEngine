#include "PCH.h"
#include "EditorFileBrowser.h"

namespace SQ {
	EditorFileBrowser::EditorFileBrowser()
	{
		currentDirectory = ".\\Resources";
	}

	void EditorFileBrowser::Render()
	{
		toOpenNext = "";
		ImGui::Begin("Open Folder");
		if (ImGui::Button("Up") && currentDirectory != ".\\Resources")
		{
			currentDirectory = currentDirectory.parent_path();
		}
		int tally = 0;
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentDirectory))
		{
			if (tally % 8 != 0) ImGui::SameLine();
			if (entry.is_directory()) 
			{
				if (ImGui::Button((ICON_LC_FOLDER + entry.path().filename().string()).c_str(), ImVec2(48, 48)))
				{
					currentDirectory = entry.path();
				}
				tally++;
			}
		}
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(currentDirectory))
		{
			if (tally % 8 != 0) ImGui::SameLine();
			if (!entry.is_directory())
			{
				if (ImGui::Button((std::string(GetIconForExtension(entry.path().extension().string())) + "\n" + entry.path().filename().string()).c_str(), ImVec2(48, 48)))
				{
					toOpenNext = entry.path().string();
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
		return toOpenNext;
	}
	const char* EditorFileBrowser::GetIconForExtension(std::string extension)
	{
		if (extension == ".nut") return ICON_LC_LAND_PLOT;
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