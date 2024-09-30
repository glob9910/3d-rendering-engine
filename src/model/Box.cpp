#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.cpp"
#include "Shader.cpp"
#include <vector>
#include "Model.cpp"

class Box : public Model {
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    Box() {
        std::vector<Vertex> vertices;
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f));
        vertices.push_back(createVertex(0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f));
        vertices.push_back(createVertex(0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f));
        vertices.push_back(createVertex(-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f));
        std::vector<unsigned int> indices;
        this->meshes.push_back(Mesh(vertices, indices));
    }

    Vertex createVertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty) {
        glm::vec3 position(x, y, z);
        glm::vec3 normal(nx, ny, nz);
        glm::vec2 texCoord(tx, ty);
        Vertex v {
            .position = position,
            .normal = normal,
            .texCoords = texCoord
        };

        return v;
    }
};

