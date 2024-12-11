#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../DwarfGL/Mesh/DwarfMesh2D.h"

using namespace std;

namespace Dwarf {

    struct Entity {
        string name;
        string model;
        string texture;
        glm::mat4 transform;

        void Translate(glm::vec3 translation) {
            transform = glm::translate(transform, translation);
        }
    };

    class DwarfEntityManager {
    private:
        public:
        static void Allocate();


        static Entity* CreateEntity(const string &name = "Entity");
        static vector<Entity*>* GetEntityList();
        void SetEntityMesh(Entity* entity, const string& model);
        /*void SetEntityTexture(shared_ptr<Entity> entity, const string& texture);*/
        static void RemoveEntity(Entity* entity);
        static void RemoveEntityByName(string& name);
        static void RemoveAllEntities();


        /*~DwarfEntityManager();*/
    private:
        static DwarfEntityManager* Instance;
        DwarfEntityManager();
        static vector<Entity*> entities;
    };
}
