#pragma once
#include "WorldNut.h"
#include "Mesh.h"
namespace SQ {
    class MeshNut :
        public WorldNut
    {
    public:

        void SetMesh(std::shared_ptr<Mesh> mesh);
        std::shared_ptr<Mesh> GetMesh();

        virtual ~MeshNut() override {};
    private:
        std::shared_ptr<Mesh> mesh;
    };
}
