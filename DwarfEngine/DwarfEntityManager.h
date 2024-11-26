#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../DwarfGL/Mesh/DwarfMesh2D.h"

using namespace std;

namespace Dwarf {
    struct Entity {
        string name;
        const char* model;
        const char* texture;
        glm::mat4 transform;
    };

    class DwarfEntityManager {
    private:
        public:
        DwarfEntityManager();

        Entity CreateDwarfEntity(const string &name = "Entity");
        vector<Entity>* GetEntityList();
        /*void SetEntityModel(shared_ptr<Entity> entity, const char* model);
        void SetEntityTexture(shared_ptr<Entity> entity, const char* texture);*/
        void RemoveEntity(string name);
        void RemoveAllEntitys();


        /*~DwarfEntityManager();*/
    private:

        vector<Entity> entities {};
    };
}
