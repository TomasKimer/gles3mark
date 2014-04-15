
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

public:
    Mesh(std::vector<glm::vec3> vertices, std::vector<glm::ivec3> faces, std::vector<glm::vec2> texCoords,
        std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::string name = std::string()) :
    vertices(vertices), faces(faces), texCoords(texCoords), normals(normals), tangents(tangents), name(name)    
    {}

//    Mesh() {}
    
    // Nasobeni matici vynasobi vsechny vertexy meshe
    Mesh operator* (const glm::mat4& matrix) const {    
    }


    void MakeTestMesh();
};