#include "PCH.h"
#include "Mesh.h"

#include <fstream>
#include "Utility.h"

#ifdef DX11
#include "MeshDX11.h"
#endif // DX11

// DELETE THIS
#include <iostream>

namespace SQ {
    Mesh* Mesh::Load(std::string path)
    {
        // Load File into data, since all meshes will use the same data regardless of graphics API we can do it here
        std::ifstream file(path, std::istream::in);
        
        if (!file.is_open()) return nullptr;
        // For each line
        std::string currentLine;

        // Indexes for knowing which texcoord or normal we are on
        unsigned int textureCoordinateIndex = 0; 
        unsigned int normalIndex = 0;

        // Vectors for each component, of a vertex, sometimes they can be indexed differently. 
        std::vector<Vec3> positions;
        std::vector<Vec2> textureCoordinates;
        std::vector<Vec3> normals;
        // Store the mesh in order first then compare vertexes to turn into a vertex index array. 
        std::vector<Vertex> inOrderMesh;

        // Create new mesh resource to input into 
#ifdef DX11
        // DX11 mesh if we are DX11
        Mesh* toReturn = static_cast<Mesh*>(new MeshDX11);
#endif // DX11
        while (std::getline(file, currentLine)) {
            // Split the line by spaces
            std::vector<std::string> lineElements = SplitString(currentLine, ' ');

            // If statement decides action based on line start,
            // https://en.wikipedia.org/wiki/Wavefront_.obj_file#File_format
            // Supported, v, vt, vn, f
            if (lineElements[0] == "v") {
                // Assume V is always first this will create the vertex with just position for now
                /*Vertex newVertex;
                newVertex.position = V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3]));
                toReturn->vertices.push_back(newVertex);*/

                positions.push_back(V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3])));
            }
            else if (lineElements[0] == "vt") {
                // vn and vt can be in either order, make sure to match with right vertex using the index. 
                /*toReturn->vertices[textureCoordinateIndex].textureCoordinate = V2(std::stof(lineElements[1]), std::stof(lineElements[2]));
                ++textureCoordinateIndex;*/

                textureCoordinates.push_back(V2(std::stof(lineElements[1]), std::stof(lineElements[2])));
            }
            else if (lineElements[0] == "vn") {
                // vn and vt can be in either order, make sure to match with right vertex using the index. 
                /*toReturn->vertices[normalIndex].normal = V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3]));
                ++normalIndex;*/

                normals.push_back(V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3])));
            }
            else if (lineElements[0] == "f") {
                // For each point in the triangle
                for (int v = 0; v < 3; ++v) {
                    std::vector<std::string> facePointElements = SplitString(lineElements[v + 1], '/');
                    // If no slashes, then the index is the index of all 3 elements
                    if (facePointElements.size() == 1) {
                        int indexOfData = std::stoi(facePointElements[0]) - 1;
                        Vertex toPushBack;

                        if (indexOfData < positions.size()) toPushBack.position = positions[indexOfData];
                        if (indexOfData < textureCoordinates.size()) toPushBack.textureCoordinate = textureCoordinates[indexOfData];
                        if (indexOfData < normals.size()) toPushBack.normal = normals[indexOfData];

                        inOrderMesh.push_back(toPushBack);
                    }
                    // If there is one slash, then its only positions and texture coordinates
                    else if (facePointElements.size() == 2) {
                        int indexOfPosition = std::stoi(facePointElements[0]) - 1;
                        int indexOfTextureCoord = std::stoi(facePointElements[1]) - 1;
                        Vertex toPushBack;

                        if (indexOfPosition < positions.size()) toPushBack.position = positions[indexOfPosition];
                        if (indexOfTextureCoord < textureCoordinates.size()) toPushBack.textureCoordinate = textureCoordinates[indexOfTextureCoord];

                        inOrderMesh.push_back(toPushBack);
                    }
                    // If there is two slashes, then its either positions and normals only or positions normals and texture coordinates. 
                    else if (facePointElements.size() == 3) {
                        int indexOfPosition = std::stoi(facePointElements[0]) - 1;
                        int indexOfTextureCoord = -1;
                        // There is a chance there is no texture coordinates, if so these will be defaulted to 0 so no need for index.
                        if(facePointElements[1] != "") indexOfTextureCoord = std::stoi(facePointElements[1]) - 1;
                        int indexOfNormal = std::stoi(facePointElements[2]) - 1;
                        Vertex toPushBack;

                        if (indexOfPosition < positions.size()) toPushBack.position = positions[indexOfPosition];
                        if (indexOfTextureCoord < textureCoordinates.size() && indexOfTextureCoord > -1) toPushBack.textureCoordinate = textureCoordinates[indexOfTextureCoord];
                        if (indexOfNormal < normals.size()) toPushBack.normal = normals[indexOfNormal];

                        inOrderMesh.push_back(toPushBack);
                    }
                }

            }
        }
        
        // This part seems to be the slowest, double as slow as above. 
        // Now convert the in order vertices to a unique vertex array and a index array
        for (int v = 0; v < inOrderMesh.size(); ++v) {
            // Search for this vertex in our unique vertex vector
            bool matchFound = false;
            for (int uv = 0; uv < toReturn->vertices.size(); uv++) {
                // If its there add this vertex
                if (toReturn->vertices[uv] == inOrderMesh[v]) {
                    toReturn->indicies.push_back(uv);
                    matchFound = true;
                }
            }
            // If that vertex does not exist, then add it to the unique vertices vector (and add its index too)
            if (!matchFound) {
                toReturn->indicies.push_back(toReturn->vertices.size());
                toReturn->vertices.push_back(inOrderMesh[v]);
            }
            if(v % 500 == 0) std::cout << "Processing Unique: " << (float)v / inOrderMesh.size() * 100.0f << "%\n";
        }


#ifdef DX11
        // Load DX11 buffers if we are in DX11 mode
        static_cast<MeshDX11*>(toReturn)->LoadBuffers();
#endif // DX11


        return toReturn;
    }

    unsigned int Mesh::GetVertexCount()
    {
        return vertices.size();
    }

    unsigned int Mesh::GetIndexCount()
    {
        return indicies.size();
    }
}
