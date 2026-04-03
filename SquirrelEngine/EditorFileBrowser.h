#pragma once
#include "PCH.h"
#include <filesystem>

namespace SQ {
	class EditorFileBrowser
	{
	public:
		EditorFileBrowser();

		void Render();

		std::string GetObjectToOpenNext();
		bool ObjectToOpenIsScene();

		static const char* GetIconForExtension(std::string extension);
	private:
		std::filesystem::path currentDirectory;

		std::string toOpenNext;
		bool toOpenNextIsScene; // If false, its a resource

		void RenderItemInTree(const std::filesystem::directory_entry& item);

		const int DEFAULT_TREE_FLAGS = ImGuiTreeNodeFlags_DrawLinesFull | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	};
}