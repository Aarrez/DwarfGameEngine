#include "DwarfOBJLoader.h"

namespace Dwarf {
    DwarfOBJLoader::~DwarfOBJLoader() {
    }

    MeshData DwarfOBJLoader::OBJFileParser(const string& filename) {
        vector<Vertex> vertices;
        vector<TexCord> tex_cords;
        std::vector<Vertex> normals;
        vector<Face> vertices_faces;
        vector<Face> uv_faces;
        vector<Face> normal_faces;
        std::ifstream file(filename);
        std::string line;
        if (!file.is_open()) {
            cerr << "File not found: " << filename << endl;
            return MeshData(vertices,
                tex_cords,
                normals,
                vertices_faces,
                uv_faces,
                normal_faces);
        }

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            string prefix;
            iss >> prefix;
            if (prefix == "v") {
                Vertex vert;
                iss >> vert.x >> vert.y >> vert.z;
                vertices.push_back(vert);
            }
            if (prefix == "vn") {
                Vertex vert;
                iss >> vert.x >> vert.y >> vert.z;
                normals.push_back(vert);
            }
            if (prefix == "vt") {
                TexCord temp;
                iss >> temp.x >> temp.y;
                tex_cords.push_back(temp);
            }
            if (prefix == "f") {
                Face vertexIndex {0, 0, 0};
                Face uvIndex {0, 0, 0};
                Face normalIndex {0, 0, 0};
                int j = 0;
                char* token = std::strtok(line.data(), " /f");
                vector<unsigned int> faceList;
                while (token != nullptr) {
                    unsigned int i = std::stoi(token);
                    faceList.push_back(i);
                    token = std::strtok(nullptr, " /f");
                    j++;
                }
                if (j == 9) {
                    vertexIndex.x = faceList[0];
                    vertexIndex.y = faceList[3];
                    vertexIndex.z = faceList[6];
                    uvIndex.x = faceList[1];
                    uvIndex.y = faceList[4];
                    uvIndex.z = faceList[7];
                    normalIndex.x = faceList[2];
                    normalIndex.y = faceList[5];
                    normalIndex.z = faceList[8];
                }
                else if (j == 3) {
                    vertexIndex.x = faceList[0];
                    vertexIndex.y = faceList[1];
                    vertexIndex.z = faceList[2];
                    uvIndex = Face(0,0,0);
                    normalIndex = Face(0,0, 0);
                }
                vertexIndex -= 1;
                uvIndex -= 1;
                normalIndex -= 1;
                vertices_faces.push_back(vertexIndex);
                uv_faces.push_back(uvIndex);

                normal_faces.push_back(normalIndex);
            }
        }
        file.close();
        return MeshData(vertices,
               tex_cords,
               normals,
               vertices_faces,
               uv_faces,
               normal_faces);
    }

    vector<Vertex> DwarfOBJLoader::GetVerticesFromData(MeshData& data) {
        vector<Vertex> ordered_vertices;
        for (unsigned int i {0}; i < data.vertices_faces.size(); i++) {

            Vertex temp1 =
                data.vertices[data.vertices_faces[i].x];
            Vertex temp2 =
                data.vertices[data.vertices_faces[i].y];
            Vertex temp3 =
                data.vertices[data.vertices_faces[i].z];

            ordered_vertices.push_back(temp1);
            ordered_vertices.push_back(temp2);
            ordered_vertices.push_back(temp3);
        }
        return ordered_vertices;
    }

    MeshData DwarfOBJLoader::OBJDataDeserializer(const char* filename) {
        std::ifstream file;
        file.open(filename, std::ios_base::binary);
        if (!file.is_open()) {
            cerr << "File not found: " << filename << endl;
            return MeshData();
        }
        MeshData meshData;

        for (int i {0}; i < meshData.vertices.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < meshData.texCords.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < meshData.normals_vertices.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.normals_vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < meshData.vertices_faces.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.vertices_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < meshData.uvs_faces.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.uvs_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < meshData.normals_faces.size(); i++) {
            file.read(reinterpret_cast<char*>(&meshData.normals_faces[i]), sizeof(Face));
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return MeshData();
        }
        return meshData;
    }

    void DwarfOBJLoader::OBJDataSerializer(string& path, MeshData &meshData) {
        string thepath = path;

        path.erase(0, path.rfind(path, '/'));
        path.erase(path.find('.'));
        MeshDataSize mds = MeshDataSize(
            meshData.vertices.size(),
            meshData.vertices_faces.size(),
            meshData.texCords.size(),
            meshData.normals_vertices.size(),
            meshData.uvs_faces.size());
        SerializedFile serialized_file;
        serialized_file = SerializedFile(path, thepath, mds);
        FilesSerialized.push_back(serialized_file);

        //TODO Check path has the correct name, replace meshData.{structname}.size()
        std::ofstream file;
        file.open(path, std::ios_base::binary);
        if (!file.is_open()) {
            cerr << path << " \n is not a valid path" << endl;
            return;
        }

        for (int i {0}; i < meshData.vertices.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < meshData.texCords.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < meshData.normals_vertices.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.normals_vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < meshData.vertices_faces.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.vertices_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < meshData.uvs_faces.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.uvs_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < meshData.normals_faces.size(); i++) {
            file.write(reinterpret_cast<const char*>(&meshData.normals_faces[i]), sizeof(Face));
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return;
        }
    }


}

