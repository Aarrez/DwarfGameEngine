#include "DwarfOBJLoader.h"

namespace Dwarf {

    vector<SerializedFile> DwarfOBJLoader::FilesSerialized;
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
        MeshDataSize data_size;
        string fname;

        bool foundNam = false;
        for (SerializedFile data : FilesSerialized) {
            if (data.fileName == filename) {
                data_size = data.meshDataSize;
                fname = data.path;
                foundNam = true;
                break;
            }
        }
        if (!foundNam) {
            cerr << "File name not found: " << filename << endl;
            return MeshData();
        }
        MeshData meshData;
        std::ifstream file;
        file.open(fname, std::ios_base::binary | std::ios_base::in);
        if (!file.is_open()) {
            cerr << "File not found: " << filename << endl;
            return MeshData();
        }
        meshData.vertices.resize(data_size.verticesCount);
        meshData.texCords.resize(data_size.texCordCount);
        meshData.normals_vertices.resize(data_size.vNormalCount);
        meshData.vertices_faces.resize(data_size.vFacesCount);
        meshData.uvs_faces.resize(data_size.uvsCount);
        meshData.normals_faces.resize(data_size.iNormalCount);
        for (int i {0}; i < data_size.verticesCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < data_size.texCordCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < data_size.vNormalCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.normals_vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < data_size.vFacesCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.vertices_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < data_size.uvsCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.uvs_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < data_size.iNormalCount; i++) {
            file.read(reinterpret_cast<char*>(&meshData.normals_faces[i]), sizeof(Face));
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return MeshData();
        }
        return meshData;
    }

    void DwarfOBJLoader::OBJDataSerializer(string& filePath, MeshData &meshData, const string& binPath) {
        SerializedFile serialized_file;

        string pathToFile = binPath;
        string filename = filePath;
        filename = filename.substr(filename.find_last_of('/') + 1);
        filename = filename.erase(filename.find_last_of('.'));

        filename.append(".bin");
        pathToFile.append(filename);

        serialized_file.path = pathToFile;
        serialized_file.fileName = filename;

        MeshDataSize mds = MeshDataSize(
            meshData.vertices.size(),
            meshData.vertices_faces.size(),
            meshData.texCords.size(),
            meshData.normals_vertices.size(),
            meshData.uvs_faces.size(),
            meshData.normals_faces.size());
        serialized_file.meshDataSize = mds;
        FilesSerialized.push_back(serialized_file);

        //TODO Check path has the correct name, replace meshData.{structname}.size()
        std::ofstream file;
        file.open(pathToFile, std::ios_base::binary | std::ios_base::out);
        if (!file.is_open()) {
            cerr << filePath << " \n is not a valid path" << endl;
            return;
        }
        for (int i {0}; i < mds.verticesCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < mds.texCordCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < mds.vNormalCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.normals_vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < mds.vFacesCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertices_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < mds.uvsCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.uvs_faces[i]), sizeof(Face));
        }
        for (int i {0}; i < mds.iNormalCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.normals_faces[i]), sizeof(Face));
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return;
        }
    }


}

