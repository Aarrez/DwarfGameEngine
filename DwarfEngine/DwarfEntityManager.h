#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../DwarfGL/Mesh/VirtualObject.h"



namespace Engine {


    class DwarfEntityManager {
        public:

        static void Allocate();

        static Entity* CreateEntity(const SerializedFile& file, const Texture &texture, const string &name = "Entity");
        static std::vector<Entity*>* GetEntityList();
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
