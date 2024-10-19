#pragma once
#include "PCH.h"
#include "Resource.h"
#include "Math.h"

namespace SQ {

    struct Vertex {
        Vec3 position;
        Vec3 normal;
        Vec2 textureCoordinate;
        Vec3 tangent;
        Vec3 bitangent;

        /// <summary>
        /// Checks if vertecies are identical, used for turning in order vertices into vertex index ones. 
        /// </summary>
        /// <param name="rhs">Value to Compare</param>
        /// <returns>True if identical</returns>
        bool operator==(const Vertex& rhs) {
            return (position == rhs.position && normal == rhs.normal && textureCoordinate == rhs.textureCoordinate && tangent == rhs.tangent && bitangent == rhs.bitangent);
        }
    };

    class Mesh :
        public Resource
    {
    public:
        /// <summary>
        /// Loads a mesh and returns a pointer to the resource
        /// Currently supports:
        /// .obj
        /// </summary>
        /// <param name="path">Path of mesh to load</param>
        /// <returns>Pointer to loaded mesh</returns>
        static Mesh* Load(std::string path);

        /// <summary>
        /// Returns the number of unique vertices this mesh has.
        /// </summary>
        /// <returns>Number of unique vertices of this mesh</returns>
        unsigned int GetVertexCount();
        /// <summary>
        /// Returns the number of vertices this mesh has. 
        /// </summary>
        /// <returns>Number of vertices of this mesh</returns>
        unsigned int GetIndexCount();
    private:
        std::vector<Vertex> vertices;
        std::vector<int> indicies;
    };
}

