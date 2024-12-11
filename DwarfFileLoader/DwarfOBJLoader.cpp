#include "DwarfOBJLoader.h"

#include "../DwarfMisc/DwarfPath.h"

namespace Dwarf {

    vector<SerializedFile> DwarfOBJLoader::FilesSerialized;
    /* DwarfModels/BinaryFiles/ */
    string DwarfOBJLoader::defaultBinPath = "DwarfModels/BinaryFiles/";

    void DwarfOBJLoader::GetBinaryFiles() {
        auto binFiles = DwarfPathChange::GetNameFilesInDirectory(defaultBinPath);
        for (auto& binFile : binFiles) {
            size_t i = binFile.find('.');
            if (i == binFile.npos) continue;
            MeshDataSize data_size{};
            SerializedFile serialized_file;
            string fileType = binFile.substr(i + 1);
            bool exists = false;
            for (auto& files_serialized : FilesSerialized) {
                if (files_serialized.fileName == binFile.erase(i)) {
                    serialized_file = files_serialized;
                    exists = true;
                }
            }
            if (fileType == "json") {
                string filePath = defaultBinPath + binFile + ".json";
                ifstream f;
                f.open(filePath);
                json data = json::parse(f);
                f.close();
                data_size.VertexCount = data["vCount"].get<size_t>();
                data_size.VertexIndexCount = data["viCount"].get<size_t>();
                data_size.TexCordCount = data["texCordCount"].get<size_t>();
                data_size.VertexNormalCount = data["nvCount"].get<size_t>();
                data_size.UvsCount = data["uvsCount"].get<size_t>();
                data_size.NormalIndexCount = data["inCount"].get<size_t>();

                serialized_file.meshDataSize = data_size;
                serialized_file.jsonPath = filePath;
                serialized_file.fileName = binFile.erase(i);
            }
            if (fileType == "bin") {
                serialized_file.binPath = defaultBinPath + binFile;
                serialized_file.fileName = binFile.erase(i);
            }
            if (exists) continue;
            FilesSerialized.push_back(serialized_file);
        }

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
            return MeshData();
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
        auto meshData = MeshData();
        meshData.vertexes = vertices;
        meshData.texCords = tex_cords;
        meshData.vertex_normals = normals;
        meshData.vertex_indexes = vertices_faces;
        meshData.uvs_indexes = uv_faces;
        meshData.vertex_normals_indexes = normal_faces;

        return meshData;
    }

    vector<Vertex> DwarfOBJLoader::GetVerticesFromData(MeshData& data) {
        vector<Vertex> ordered_vertices;
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

    MeshData<> DwarfOBJLoader::OBJDataDeserializer(const string& filename) {
        MeshDataSize data_size;
        string fname;

        bool foundNam = false;
        for (SerializedFile data : FilesSerialized) {
            if (data.fileName == filename) {
                data_size = data.meshDataSize;
                fname = data.binPath;
                foundNam = true;
                break;
            }
        }
        if (!foundNam) {
            cerr << "File name not found: " << filename << endl;
            return {};
        }
        MeshData<> meshData;
        std::ifstream file;
        file.open(fname, std::ios_base::binary | std::ios_base::in);
        if (!file.is_open()) {
            cerr << "File not found: " << filename << endl;
            return {};
        }

        for (int i {0}; i < data_size.VertexCount; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            meshData.vertexes.push_back(temp);
        }
        for (int i {0}; i < data_size.TexCordCount; i++) {
            TexCord temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(TexCord));
            meshData.texCords.push_back(temp);
        }
        for (int i {0}; i < data_size.VertexNormalCount; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            meshData.vertex_normals.push_back(temp);
        }
        for (int i {0}; i < data_size.VertexIndexCount; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            meshData.vertex_indexes.push_back(temp);
        }
        for (int i {0}; i < data_size.UvsCount; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            meshData.uvs_indexes.push_back(temp);
        }
        for (int i {0}; i < data_size.NormalIndexCount; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            meshData.vertex_normals_indexes.push_back(temp);
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return {};
        }
        return meshData;
    }

    void DwarfOBJLoader::OBJDataSerializer(string& filePath, MeshData &meshData, const string& binPath) {
        SerializedFile serialized_file;

        //"filePath" looks something like this
        //DwarfModels/BinaryFiles/{Filename}.obj

        string pathToFile = binPath;
        string filename = filePath;
        filename = filename.substr(filename.find_last_of('/') + 1);
        //{Filename}.obj
        filename = filename.erase(filename.find_last_of('.'));
        //{Filename}

        pathToFile.append(filename);

        serialized_file.binPath = pathToFile;
        serialized_file.fileName = filename;

        MeshDataSize mds = MeshDataSize(
            meshData.vertexes.size(),
            meshData.vertex_indexes.size(),
            meshData.texCords.size(),
            meshData.vertex_normals.size(),
            meshData.uvs_indexes.size(),
            meshData.vertex_normals_indexes.size());

        serialized_file.meshDataSize = mds;
        //Creating a json object that holds the data of mds
        json s
        {
            {"vCount", mds.VertexCount},
            {"viCount", mds.VertexIndexCount},
            {"texCordCount", mds.TexCordCount},
            {"nvCount", mds.VertexNormalCount},
            {"uvsCount", mds.UvsCount},
            {"inCount", mds.NormalIndexCount}
        };

        //Creating a json file and serializing the data in the json object "s"
        string jsonFile = defaultBinPath + filename + ".json";
        ofstream i(jsonFile);
        i << s << '\n';
        //i << std::setw(2) << s << '\n';
        i.close();
        serialized_file.jsonPath = jsonFile;
        FilesSerialized.push_back(serialized_file);

        //TODO Check path has the correct name, replace meshData.{structname}.size()
        std::ofstream file;
        file.open(pathToFile, std::ios_base::binary | std::ios_base::out);
        if (!file.is_open()) {
            cerr << filePath << " \n is not a valid path" << endl;
            return;
        }
        for (int i {0}; i < mds.VertexCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertexes[i]), sizeof(Vertex));
        }
        for (int i {0}; i < mds.TexCordCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < mds.VertexNormalCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertex_normals[i]), sizeof(Vertex));
        }
        for (int i {0}; i < mds.VertexIndexCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertex_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < mds.UvsCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.uvs_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < mds.NormalIndexCount; i++) {
            file.write(reinterpret_cast<char*>(&meshData.vertex_normals_indexes[i]), sizeof(Face));
        }
        file.close();
        if (!file.good()) {
            cerr << "Error while writing to binary file" << endl;
            return;
        }
    }

}

