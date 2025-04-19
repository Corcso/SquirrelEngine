#include "PCH.h"
#include "ResourceManager.h"
#include "Services.h"

void SQ::ResourceManager::ImGuiRenderDebugInfo()
{
	ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode("Resources"))
	{
		if (ImGui::BeginTable("Resources", 2, ImGuiTableFlags_Borders))
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::Text("Path");

			ImGui::TableSetColumnIndex(1);
			ImGui::Text("Use Count");

			for (auto it : resources) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				if (it.second.use_count() == 0) ImGui::Text(it.first.c_str());
				else if (ImGui::Button(it.first.c_str())) {
					// Cant load resources polymorphicly, maybe consider this for SQ2
					Services::GetTree()->SetResourceInspector(it.second.lock());
				}

				ImGui::TableSetColumnIndex(1);
				ImGui::Text(std::to_string(it.second.use_count()).c_str());
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}
}
