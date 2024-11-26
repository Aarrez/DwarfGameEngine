#pragma once
#include <list>
#include <string>
#include <glm/glm.hpp>

#include "../DwarfGL/Mesh/DwarfMesh2D.h"

using namespace std;

namespace Dwarf::Entity {
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

        shared_ptr<Entity> CreateDwarfEntity(string name);
        /*void SetEntityModel(shared_ptr<Entity> entity, const char* model);
        void SetEntityTexture(shared_ptr<Entity> entity, const char* texture);*/
        /*void DeleteDwarfEntity(string name);*/

        vector<shared_ptr<Entity>> entities;
        /*~DwarfEntityManager();*/
    private:

    };
}
