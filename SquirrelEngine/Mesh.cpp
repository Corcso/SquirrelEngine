#include "PCH.h"
#include "Mesh.h"

#include <fstream>
#include "Utility.h"
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

        // Create new mesh resource to input into 
        Mesh* toReturn = new Mesh();
        while (std::getline(file, currentLine)) {
            // Split the line by spaces
            std::vector<std::string> lineElements = SplitString(currentLine, ' ');

            // If statement decides action based on line start,
            // https://en.wikipedia.org/wiki/Wavefront_.obj_file#File_format
            // Supported, v, vt, vn, f
            if (lineElements[0] == "v") {
                // Assume V is always first this will create the vertex with just position for now
                Vertex newVertex;
                newVertex.position = V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3]));
                toReturn->vertices.push_back(newVertex);
            }
            else if (lineElements[0] == "vt") {
                // vn and vt can be in either order, make sure to match with right vertex using the index. 
                toReturn->vertices[textureCoordinateIndex].textureCoordinate = V2(std::stof(lineElements[1]), std::stof(lineElements[2]));
                ++textureCoordinateIndex;
            }
            else if (lineElements[0] == "vn") {
                // vn and vt can be in either order, make sure to match with right vertex using the index. 
                toReturn->vertices[normalIndex].normal = V3(std::stof(lineElements[1]), std::stof(lineElements[2]), std::stof(lineElements[3]));
                ++normalIndex;
            }
            else if (lineElements[0] == "f") {
                // Currently only supports vertex face decleration  (Vertex indices section wikipedia)
                toReturn->indicies.push_back(std::stoi(lineElements[1]) - 1);
            }
        }



        return nullptr;
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
