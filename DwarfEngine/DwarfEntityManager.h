#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../DwarfGL/Mesh/VirtualObject.h"

using namespace std;

namespace Dwarf {

    class DwarfEntityManager {
    private:
        public:
        static void Allocate();

        static Entity* CreateEntity(const string &name = "Entity");
        static vector<Entity*>* GetEntityList();
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
