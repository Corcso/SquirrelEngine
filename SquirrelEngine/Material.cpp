#include "PCH.h"
#include "Material.h"
#ifdef DX11
#include "MaterialDX11.h"
#endif // DX11
#ifdef VULKAN
#include "MaterialVulkan.h"
#endif // VULKAN
namespace SQ {
    StandardMaterial* StandardMaterial::Load(std::string path)
    {
#ifdef DX11
        return MaterialDX11::Load(path);
#endif // DX11
#ifdef VULKAN
        return MaterialVulkan::Load(path);
#endif // VULKAN
        return nullptr;
    }
    void StandardMaterial::ImGuiRenderMyInspector()
    {
        Resource::ImGuiRenderMyInspector();
        if (ImGui::TreeNodeEx("StandardMaterial", ImGuiTreeNodeFlags_DefaultOpen)) {

            ImGui::Text("StandardMaterial is only a interface, all data is stored in the platform specific resources.");

            ImGui::TreePop();
        }
    }
}
