
#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>


class Mesh {
public:
    std::vector<glm::vec3 > vertices;
    std::vector<glm::ivec3> faces;
    std::vector<glm::vec2 > texCoords;
    std::vector<glm::vec3 > normals;
    std::vector<glm::vec3 > tangents;    

    std::string name;
    glm::mat4 matrix;
    unsigned materialID;

public:
    Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::ivec3>& faces, const std::vector<glm::vec2>& texCoords,
         const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& tangents, unsigned materialId, const std::string& name = std::string()) :
         vertices(vertices), faces(faces), texCoords(texCoords), normals(normals), tangents(tangents), materialID(materialId), name(name)
    {}

//    Mesh() {}
    
    // Nasobeni matici vynasobi vsechny vertexy meshe
    Mesh operator* (const glm::mat4& matrix) const {    
    }

    void FreeMemory() {
        std::vector<glm::vec3 >().swap(vertices );
        std::vector<glm::ivec3>().swap(faces    );
        std::vector<glm::vec2 >().swap(texCoords);
        std::vector<glm::vec3 >().swap(normals  );
        std::vector<glm::vec3 >().swap(tangents );    
    }

    void MakeTestMesh();
};