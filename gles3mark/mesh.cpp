

#include "mesh.h"


void Mesh::TransformData(const glm::mat4& matrix) {    
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertices[i] = glm::vec3(matrix * glm::vec4(vertices[i], 1.0f));        
    }
    for (size_t i = 0; i < normals.size(); ++i) {
        normals[i] = glm::vec3(matrix * glm::vec4(normals[i], 0.0f));  // vec4 * matrix?
    }
    for (size_t i = 0; i < tangents.size(); ++i) {
        tangents[i] = glm::vec3(matrix * glm::vec4(tangents[i], 0.0f));        
    }
}

void Mesh::MakeTestMesh() {
    vertices = {
        // Walls
        glm::vec3(-5.0, -5.0, -5.0),
        glm::vec3(-5.0, -5.0,  5.0),
        glm::vec3( 5.0, -5.0,  5.0),
        glm::vec3( 5.0, -5.0, -5.0),
    
        glm::vec3(-5.0,  5.0, -5.0),
        glm::vec3(-5.0,  5.0,  5.0),
        glm::vec3( 5.0,  5.0,  5.0),
        glm::vec3( 5.0,  5.0, -5.0),

        glm::vec3(-5.0, -5.0, -5.0),
        glm::vec3(-5.0, -5.0,  5.0),
        glm::vec3(-5.0,  5.0,  5.0),
        glm::vec3(-5.0,  5.0, -5.0),

        glm::vec3( 5.0, -5.0, -5.0),
        glm::vec3( 5.0, -5.0,  5.0),
        glm::vec3( 5.0,  5.0,  5.0),
        glm::vec3( 5.0,  5.0, -5.0),
        // Roof
        glm::vec3(-5.0,  5.0, -5.0),
        glm::vec3( 5.0,  5.0, -5.0),
        glm::vec3( 0.0, 11.0,  0.0),

        glm::vec3( 5.0,  5.0, -5.0),
        glm::vec3( 5.0,  5.0,  5.0),
        glm::vec3( 0.0, 11.0,  0.0),

        glm::vec3( 5.0,  5.0,  5.0),
        glm::vec3(-5.0,  5.0,  5.0),
        glm::vec3( 0.0, 11.0,  0.0),

        glm::vec3(-5.0,  5.0,  5.0),
        glm::vec3(-5.0,  5.0, -5.0),
        glm::vec3( 0.0, 11.0,  0.0),
    };

    texCoords = {
        // Walls
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 0.0, 4.0 ),
        glm::vec2( 4.0, 4.0 ),
        glm::vec2( 4.0, 0.0 ),
                     
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 0.0, 2.0 ),
        glm::vec2( 2.0, 2.0 ),
        glm::vec2( 2.0, 0.0 ),
                     
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 0.0, 1.0 ),
        glm::vec2( 1.0, 1.0 ),
        glm::vec2( 1.0, 0.0 ),
                     
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 0.0, 1.0 ),
        glm::vec2( 1.0, 1.0 ),
        glm::vec2( 1.0, 0.0 ),
        // Roof
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 1.0, 0.0 ),
        glm::vec2( 0.0, 1.0 ),
                     
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 1.0, 0.0 ),
        glm::vec2( 0.0, 1.0 ),
                     
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 0.0, 1.0 ),
        glm::vec2( 1.0, 1.0 ),
                     
        glm::vec2( 0.0, 1.0 ),
        glm::vec2( 0.0, 0.0 ),
        glm::vec2( 1.0, 1.0 ),
    };    

    // House indices
    faces = {
        //Walls
        glm::ivec3( 0,  1,  2),
        glm::ivec3( 0,  2,  3),
        glm::ivec3( 4,  5,  6),
        glm::ivec3( 4,  6,  7),
        glm::ivec3( 8,  9, 10),
        glm::ivec3( 8, 10, 11),
        glm::ivec3(12, 13, 14),
        glm::ivec3(12, 14, 15),
        //Roof
        glm::ivec3(16, 17, 18),
        glm::ivec3(19, 20, 21),
        glm::ivec3(22, 23, 24),
        glm::ivec3(25, 26, 27),
    };
}