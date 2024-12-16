#pragma once
#include "WorldNut.h"
#include "Mesh.h"
#include "Material.h"

namespace SQ {
    /// <summary>
    /// Mesh nut, used for 3D geometry.
    /// </summary>
    class MeshNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        // Make sure to call base class ready update and late update
        virtual void Ready() override { WorldNut::Ready(); }
        virtual void Update() override { WorldNut::Update(); }
        virtual void LateUpdate() override { WorldNut::LateUpdate(); }

        /// <summary>
        /// Set the mesh this nut uses to render
        /// </summary>
        /// <param name="mesh">Mesh resource reference</param>
        void SetMesh(std::shared_ptr<Mesh> mesh);

        /// <summary>
        /// Get the mesh resource reference this nut uses
        /// </summary>
        /// <returns>Mesh resource reference</returns>
        std::shared_ptr<Mesh> GetMesh();

        /// <summary>
        /// Set the material this mesh should use
        /// </summary>
        /// <param name="material">Material resource reference</param>
        void SetMaterial(std::shared_ptr<Material> material);

        /// <summary>
        /// Get the material this mesh uses
        /// </summary>
        /// <returns>Material resource reference</returns>
        std::shared_ptr<Material> GetMaterial();

        virtual ~MeshNut() override {};
    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
    };
}
