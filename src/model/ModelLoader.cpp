#include "Mesh.cpp"
#include <glm/glm.hpp>
#include <sstream>
#include <fstream>
#include <tuple>
#include <vector>

class ModelLoader {
public:
    Mesh load(std::string filename) {
        // dane
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

                // Ustawienie pozycji wierzchołka
                vertex.Position = vertices[vertexIndex];

                // Ustawienie normalnego wektora (jeśli dostępny)
                if (normalIndex >= 0) {
                    vertex.Normal = normals[normalIndex];
                }

                // Ustawienie współrzędnych tekstury (jeśli dostępne)
                if (texcoordIndex >= 0) {
                    vertex.TexCoords = texcoords[texcoordIndex];
                } else {
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f); // domyślna współrzędna tekstury, jeśli brak
                }

                // Dodajemy wierzchołek do siatki
                meshVertices.push_back(vertex);
                // Indeksy są numerowane od 0, więc możemy po prostu dodawać kolejno
                indices.push_back(meshVertices.size() - 1);
            }
        }

        // Tworzymy siatkę z wygenerowanych wierzchołków i indeksów, na razie brak tekstur
        return Mesh(meshVertices, indices, {});
    }
};


// #include "Mesh.cpp"
// #include "Model.cpp"

// #include <glm/glm.hpp>

// class ModelLoader {
// public:

//     Mesh load(std::string filename) {
//         // data
//         std::vector<glm::vec3> vertices;
//         std::vector<glm::vec3> normals;
//         std::vector<glm::vec2> texcoords;
//         std::vector<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>> faces;

//         std::ifstream file(filename);
//         std::string line;
//         while(std::getline(file, line)) {
//             std::istringstream ss(line);
//             std::string type;
//             ss >> type;

//             if(type == "v") {
//                 glm::vec3 v;
//                 ss >> v.x >> v.y >> v.z;
//                 vertices.push_back(v);
//             }
//             else if(type == "vn") {
//                 glm::vec3 n;
//                 ss >> n.x >> n.y >> n.z;
//                 normals.push_back(n);
//             }
//             else if(type == "vt") {
//                 glm::vec2 uv;
//                 ss >> uv.x >> uv.y;
//                 texcoords.push_back(uv);
//             }
//             else if(type == "f") {
//                 std::vector<int> face;
//                 std::vector<int> texcoord_indices;
//                 std::vector<int> normal_indices;
//                 std::string vertex;
//                 while(ss >> vertex) {
//                     int vertexIndex = 0;
//                     int texCoordIndex = 0;
//                     int normalIndex = 0;
//                     sscanf(vertex.c_str(), "%d/%d/%d", &vertexIndex, &texCoordIndex, &normalIndex);

//                     face.push_back(vertexIndex);
//                     texcoord_indices.push_back(texCoordIndex);
//                     normal_indices.push_back(normalIndex);
//                 }
//                 faces.push_back(std::make_tuple(face, normal_indices, texcoord_indices));
//             }
//         }

//         return generateModel(vertices, normals, texcoords, faces);
//     }

// private:

//     Mesh generateModel(std::vector<glm::vec3> &vertices,
//                        std::vector<glm::vec3> &normals,
//                        std::vector<glm::vec2> &texcoords,
//                        std::vector<std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>>& faces) {
        
//         std::vector<float> finalVertices;

//         for(int i=0; i<faces.size(); i++) {
//             std::vector<int>& faceVertices = std::get<0>(faces[i]);
//             std::vector<int>& faceNormals = std::get<1>(faces[i]);
//             std::vector<int>& faceTexcoords = std::get<2>(faces[i]);

//             for(int j=0; j<faceVertices.size(); j++) {
//                 int vertexIndex = faceVertices[j] - 1;
//                 int normalIndex = faceNormals[j] - 1;
//                 int texcoordIndex = faceTexcoords[j] - 1;

//                 // Dodaj współrzędne wierzchołka
//                 finalVertices.push_back(vertices[vertexIndex].x);
//                 finalVertices.push_back(vertices[vertexIndex].y);
//                 finalVertices.push_back(vertices[vertexIndex].z);

//                 // Dodaj wektory normalne (jeśli są dostępne)
//                 if (normalIndex >= 0) {
//                     finalVertices.push_back(normals[normalIndex].x);
//                     finalVertices.push_back(normals[normalIndex].y);
//                     finalVertices.push_back(normals[normalIndex].z);
//                 }

//                 // Dodaj współrzędne tekstury (jeśli są dostępne)
//                 if (texcoordIndex >= 0) {
//                     finalVertices.push_back(texcoords[texcoordIndex].x);
//                     finalVertices.push_back(texcoords[texcoordIndex].y);
//                 }
//             }
//         }
        
//         return Mesh(finalVertices);
//     }
// };