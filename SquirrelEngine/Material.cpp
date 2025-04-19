#include "PCH.h"
#include "Material.h"
#ifdef DX11
#include "MaterialDX11.h"
#endif // DX11
namespace SQ {
    Material* Material::Load(std::string path)
    {
#ifdef DX11
        return MaterialDX11::Load(path);
#endif // DX11
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
