
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

void Mesh::MakeCube() {
    /* cube vertices
     *
     *               5---------4
     *              /|        /|
     *             / |       / |
     *            1---------0  |
     *            |  |      |  |
     *            |  6------|--7
     *            | /       | /
     *            |/        |/
     *            2---------3
     *
     *   front: { 0, 2, 1 }, { 0, 3, 2 }
     *   right: { 4, 3, 0 }, { 4, 7, 3 }
     *     top: { 4, 1, 5 }, { 4, 0, 1 }
     *    left: { 1, 6, 5 }, { 1, 2, 6 }
     *  bottom: { 3, 6, 2 }, { 3, 7, 6 }
     *    back: { 5, 7, 4 }, { 5, 6, 7 }
     */

    vertices = {
        glm::vec3( 1.0f,  1.0f, -1.0f), // 0
        glm::vec3(-1.0f,  1.0f, -1.0f), // 1
        glm::vec3(-1.0f, -1.0f, -1.0f), // 2
        glm::vec3( 1.0f, -1.0f, -1.0f), // 3
        glm::vec3( 1.0f,  1.0f,  1.0f), // 4
        glm::vec3(-1.0f,  1.0f,  1.0f), // 5
        glm::vec3(-1.0f, -1.0f,  1.0f), // 6
        glm::vec3( 1.0f, -1.0f,  1.0f), // 7    
    };

    faces = {
        glm::ivec3(0, 2, 1),  glm::ivec3(0, 3, 2),  // front
        glm::ivec3(4, 3, 0),  glm::ivec3(4, 7, 3),  // right
        glm::ivec3(4, 1, 5),  glm::ivec3(4, 0, 1),  // top
        glm::ivec3(1, 6, 5),  glm::ivec3(1, 2, 6),  // left
        glm::ivec3(3, 6, 2),  glm::ivec3(3, 7, 6),  // bottom
        glm::ivec3(5, 7, 4),  glm::ivec3(5, 6, 7),  // back    
    };

    normals.resize(vertices.size()); // TODO
    texCoords.resize(vertices.size());
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

    normals.resize(vertices.size());
}