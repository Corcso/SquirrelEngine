#pragma once
#include "WorldNut.h"
#include "Mesh.h"
#include "Material.h"

namespace SQ {
    class MeshNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        void SetMesh(std::shared_ptr<Mesh> mesh);
        std::shared_ptr<Mesh> GetMesh();

        void SetMaterial(std::shared_ptr<Material> material);
        std::shared_ptr<Material> GetMaterial();

        virtual ~MeshNut() override {};
    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
    };
}
