#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../GL/Mesh/VirtualObject.h"



namespace Engine {


    class EntityManager {
        public:

        static void Allocate();

        static Entity* CreateEntity(const SerializedFile& file, const Texture &texture, const string &name = "Entity");
        static std::vector<Entity*>* GetEntityList();
        static void RemoveEntity(Entity* entity);
        static void RemoveEntityByName(string& name);
        static void RemoveAllEntities();


        /*~DwarfEntityManager();*/
    private:
        static EntityManager* Instance;
        EntityManager();
        static vector<Entity*> entities;

    };
}
