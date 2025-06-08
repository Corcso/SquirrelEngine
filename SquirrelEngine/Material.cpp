#include "PCH.h"
#include "Material.h"
#ifdef DX11
#include "MaterialDX11.h"
#endif // DX11
#ifdef VULKAN
#include "MaterialVulkan.h"
#endif // VULKAN
namespace SQ {
    Material* Material::Load(std::string path)
    {
#ifdef DX11
        return MaterialDX11::Load(path);
#endif // DX11
#ifdef VULKAN
        return MaterialVulkan::Load(path);
#endif // VULKAN
        return nullptr;
    }
    void Material::ImGuiRenderMyInspector()
    {
        Resource::ImGuiRenderMyInspector();
        if (ImGui::TreeNodeEx("Material", ImGuiTreeNodeFlags_DefaultOpen)) {

            ImGui::Text("Material is only a interface, all data is stored in the platform specific resources.");

            ImGui::TreePop();
        }
    }
}
