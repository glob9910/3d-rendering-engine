#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>

#include "Mesh.cpp"

class ModelLoader {
public:

    Mesh load(std::string filename) {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>> faces;

        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string type;
            ss >> type;

            if (type == "v") {
                glm::vec3 v;
                ss >> v.x >> v.y >> v.z;
                vertices.push_back(v);
            } else if (type == "vn") {
                glm::vec3 n;
                ss >> n.x >> n.y >> n.z;
                normals.push_back(n);
            } else if (type == "vt") {
                glm::vec2 uv;
                ss >> uv.x >> uv.y;
                texcoords.push_back(uv);
            } else if (type == "f") {
                std::vector<int> face;
                std::vector<int> texcoord_indices;
                std::vector<int> normal_indices;
                std::string vertex;
                while (ss >> vertex) {
                    int vertexIndex = 0;
                    int texCoordIndex = 0;
                    int normalIndex = 0;
                    sscanf(vertex.c_str(), "%d/%d/%d", &vertexIndex, &texCoordIndex, &normalIndex);

                    face.push_back(vertexIndex);
                    texcoord_indices.push_back(texCoordIndex);
                    normal_indices.push_back(normalIndex);
                }
                faces.push_back(std::make_tuple(face, normal_indices, texcoord_indices));
            }
        }

        return generateModel(vertices, normals, texcoords, faces);
    }


private:

    Mesh generateModel(std::vector<glm::vec3> &vertices,
                       std::vector<glm::vec3> &normals,
                       std::vector<glm::vec2> &texcoords,
                       std::vector<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>>& faces) {

        std::vector<Vertex> meshVertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < faces.size(); i++) {
            std::vector<int>& faceVertices = std::get<0>(faces[i]);
            std::vector<int>& faceNormals = std::get<1>(faces[i]);
            std::vector<int>& faceTexcoords = std::get<2>(faces[i]);

            for (int j = 0; j < faceVertices.size(); j++) {
                int vertexIndex = faceVertices[j] - 1;
                int normalIndex = faceNormals[j] - 1;
                int texcoordIndex = faceTexcoords[j] - 1;

                Vertex vertex;

                vertex.position = vertices[vertexIndex];

                if (normalIndex >= 0) {
                    vertex.normal = normals[normalIndex];
                }


                if (texcoordIndex >= 0) {
                    vertex.texCoords = texcoords[texcoordIndex];
                } else {
                    vertex.texCoords = glm::vec2(0.0f, 0.0f); // default texcoords, if not present
                }

                meshVertices.push_back(vertex);
                indices.push_back(meshVertices.size() - 1);
            }
        }

        return Mesh(meshVertices, indices);
    }
};