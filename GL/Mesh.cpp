#include "Mesh.h"


namespace Engine{
    bool MeshData::WriteTo(std::ofstream &file) const {
        int v, vt, vn, f, uv, fn;
        v = vertexes.size();
        vt = texCords.size();
        vn = vertex_normals.size();
        f = vertex_indexes.size();
        uv = uvs_indexes.size();
        fn = vertex_normals_indexes.size();
        file.write(reinterpret_cast<const char*>(&v), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vt), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vn), sizeof(int));
        file.write(reinterpret_cast<const char*>(&f), sizeof(int));
        file.write(reinterpret_cast<const char*>(&uv), sizeof(int));
        file.write(reinterpret_cast<const char*>(&fn), sizeof(int));
        for (int i {0}; i < vertexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertexes[i]), sizeof(Vertex));
        }
        for (int i {0}; i < texCords.size(); i++) {
            file.write(reinterpret_cast<const char*>(&texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < vertex_normals.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_normals[i]), sizeof(Vertex));
        }
        for (int i {0}; i < vertex_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < uvs_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&uvs_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < vertex_normals_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_normals_indexes[i]), sizeof(Face));
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }

    bool MeshData::ReadFrom(std::ifstream &file) {
        vertexes.clear();
        texCords.clear();
        vertex_normals.clear();
        vertex_indexes.clear();
        uvs_indexes.clear();
        vertex_normals_indexes.clear();
        int v, vt, vn, f, uv, fn;
        file.read(reinterpret_cast<char*>(&v), sizeof(int));
        file.read(reinterpret_cast<char*>(&vt), sizeof(int));
        file.read(reinterpret_cast<char*>(&vn), sizeof(int));
        file.read(reinterpret_cast<char*>(&f), sizeof(int));
        file.read(reinterpret_cast<char*>(&uv), sizeof(int));
        file.read(reinterpret_cast<char*>(&fn), sizeof(int));
        for (int i {0}; i < v; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            vertexes.push_back(temp);
        }
        for (int i {0}; i < vt; i++) {
            TexCord temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(TexCord));
            texCords.push_back(temp);
        }
        for (int i {0}; i < vn; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            vertex_normals.push_back(temp);
        }
        for (int i {0}; i < f; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            vertex_indexes.push_back(temp);
        }
        for (int i {0}; i < uv; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            uvs_indexes.push_back(temp);
        }
        for (int i {0}; i < fn; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            vertex_normals_indexes.push_back(temp);
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }

    bool Mesh::WriteTo(std::ofstream &file) const {
        int v, vt, vn;
        v = vertices.size();
        vt = normals.size();
        vn = uvs.size();
        file.write(reinterpret_cast<const char*>(&v), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vt), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vn), sizeof(int));
        for (int i {0}; i < vertices.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertices[i]), sizeof(Vertex));
        }
        for (int i {0}; i < normals.size(); i++) {
            file.write(reinterpret_cast<const char*>(&normals[i]), sizeof(Vertex));
        }
        for (int i {0}; i < uvs.size(); i++) {
            file.write(reinterpret_cast<const char*>(&uvs[i]), sizeof(TexCord));
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }

    bool Mesh::ReadFrom(std::ifstream &file) {
        vertices.clear();
        normals.clear();
        uvs.clear();
        int v, vt, vn;
        file.read(reinterpret_cast<char*>(&v), sizeof(int));
        file.read(reinterpret_cast<char*>(&vt), sizeof(int));
        file.read(reinterpret_cast<char*>(&vn), sizeof(int));
        for (int i {0}; i < v; i++) {
            Vertex temp;
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            vertices.push_back(temp);
        }
        for (int i {0}; i < vt; i++) {
            Vertex temp;
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            normals.push_back(temp);
        }
        for (int i {0}; i < vn; i++) {
            TexCord temp;
            file.read(reinterpret_cast<char*>(&temp), sizeof(TexCord));
            uvs.push_back(temp);
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }
}


