#include "OBJLoader.h"

#include "../Misc/Path.h"

namespace Engine {

    std::vector<SerializedFile> OBJLoader::FilesSerialized;
    /* Models/BinaryFiles/ */
    std::string OBJLoader::defaultBinPath = "Models/BinaryFiles/";

    void OBJLoader::GetBinaryFiles() {
        auto binFiles = DwarfPathChange::GetNameFilesInDirectory(defaultBinPath);
        for (auto& file : binFiles) {
            size_t i = file.find(".bin");
            if (i == file.npos) { continue; }
            SerializedFile serializedFile;
            serializedFile.fileName = file;
            serializedFile.binPath = defaultBinPath + file;
            FilesSerialized.push_back(serializedFile);
        }
    }

    std::optional<MeshData> OBJLoader::OBJFileParser(const std::string& filename) {
        std::vector<Vertex> vertices;
        std::vector<TexCord> tex_cords;
        std::vector<Vertex> normals;
        std::vector<Face> vertices_faces;
        std::vector<Face> uv_faces;
        std::vector<Face> normal_faces;

        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "File not found: " << filename << std::endl;
            return std::nullopt;
        }

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
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
                std::vector<unsigned int> faceList;
                while (token != nullptr) {
                    unsigned int i = std::stoi(token) - 1;
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
                vertices_faces.push_back(vertexIndex);
                uv_faces.push_back(uvIndex);
                normal_faces.push_back(normalIndex);
            }
        }
        file.close();
        auto meshData = MeshData();
        meshData.vertexes = vertices;
        meshData.texCords = tex_cords;
        meshData.vertex_normals = normals;
        meshData.vertex_indexes = vertices_faces;
        meshData.uvs_indexes = uv_faces;
        meshData.vertex_normals_indexes = normal_faces;

        return meshData;
    }

    Mesh OBJLoader::OrderMeshData(MeshData& data) {
        Mesh mesh;
        mesh.vertices = GetVerticesFromData(data);
        mesh.normals = GetNormalsFromData(data);
        mesh.uvs = GetTexCoordFromData(data);
        return mesh;
    }

    std::vector<Vertex> OBJLoader::GetVerticesFromData(MeshData& data) {
        std::vector<Vertex> ordered_vertices;
        for (unsigned int i {0}; i < data.vertex_indexes.size(); i++) {

            Vertex temp1 =
                data.vertexes[data.vertex_indexes[i].x];
            Vertex temp2 =
                data.vertexes[data.vertex_indexes[i].y];
            Vertex temp3 =
                data.vertexes[data.vertex_indexes[i].z];

            ordered_vertices.push_back(temp1);
            ordered_vertices.push_back(temp2);
            ordered_vertices.push_back(temp3);
        }
        return ordered_vertices;
    }

    std::vector<Vertex> OBJLoader::GetNormalsFromData(MeshData &data) {
        std::vector<Vertex> ordered_normals;
        if (data.vertex_normals.empty()) {
            return ordered_normals;
        }
        for (unsigned int i {0}; i < data.vertex_normals_indexes.size(); i++) {
            Vertex temp1 =
                data.vertex_normals[data.vertex_normals_indexes[i].x];
            Vertex temp2 =
                data.vertex_normals[data.vertex_normals_indexes[i].y];
            Vertex temp3 =
                data.vertex_normals[data.vertex_normals_indexes[i].z];

            ordered_normals.push_back(temp1);
            ordered_normals.push_back(temp2);
            ordered_normals.push_back(temp3);
        }
        return ordered_normals;
    }

    std::vector<TexCord> OBJLoader::GetTexCoordFromData(MeshData &data) {
        std::vector<TexCord> ordered_texcord;
        if (data.texCords.empty()) {
            return ordered_texcord;
        }
        for (unsigned int i {0}; i < data.uvs_indexes.size(); i++) {

            TexCord temp1 =
                data.texCords[data.uvs_indexes[i].x];
            TexCord temp2 =
                data.texCords[data.uvs_indexes[i].y];
            TexCord temp3 =
                data.texCords[data.uvs_indexes[i].z];

            ordered_texcord.push_back(temp1);
            ordered_texcord.push_back(temp2);
            ordered_texcord.push_back(temp3);
        }
        return ordered_texcord;
    }

    Mesh OBJLoader::OBJDataDeserializer(const std::string& filename) {
        std::string fname;

        bool foundNam = false;
        for (const SerializedFile& data : FilesSerialized) {
            if (data.fileName == filename) {
                fname = data.binPath;
                foundNam = true;
                break;
            }
        }
        if (!foundNam) {
            std::cerr << "File name not found: " << filename << std::endl;
            return {};
        }

        Mesh mesh;
        mesh.name = fname;
        std::ifstream file;
        file.open(fname, std::ios_base::binary | std::ios_base::in);
        if (!file.is_open()) {
            std::cerr << "File not found: " << filename << std::endl;
            return {};
        }
        if (!mesh.ReadFrom(file)) {
            std::cerr << "Failed to read binary file: " << filename << std::endl;
        }

        file.close();
        if (!file.good()) {
            std::cerr << "Error while writing to binary file" << std::endl;
            return {};
        }
        return mesh;
    }

    SerializedFile OBJLoader::OBJDataSerializer(
    Mesh &mesh, const std::string& filePath, const std::string& binPath) {
        SerializedFile serialized_file;

        //"filePath" looks something like this
        //Models/BinaryFiles/{Filename}.obj

        std::string pathToFile = binPath;
        std::string filename = filePath;
        auto i = filename.find_last_of('/') + 1;
        if (i != filename.npos)
            filename = filename.substr(i);
        //{Filename}.obj
        auto a = filename.find_last_of('.');
        if (a != filename.npos) {
            filename = filename.erase(a);
        }
        //{Filename}
        filename += ".bin";


        //Default: Models/BinaryFiles/{newFilename}.bin
        pathToFile += filename;

        serialized_file.binPath = pathToFile;
        serialized_file.fileName = filename;
        bool simFileFound = false;
        for (auto& file : FilesSerialized) {
            if (file.fileName == filename) {
                file = serialized_file;
                simFileFound = true;
            }
        }
        if (!simFileFound) {
            FilesSerialized.push_back(serialized_file);
        }


        std::ofstream file;
        file.open(pathToFile, std::ios_base::binary | std::ios_base::out);
        if (!file.is_open()) {
            std::cerr << pathToFile << " \n is not a valid path" << std::endl;
        }

        if (!mesh.WriteTo(file)) {
            std::cerr << "Error while writing to binary file" << std::endl;
            return serialized_file;
        }

        file.close();
        if (!file.good()) {
            std::cerr << "Error while writing to binary file" << std::endl;
        }
        return serialized_file;
    }

}

