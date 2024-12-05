#include "DwarfEntityManager.h"

namespace Dwarf{
    DwarfEntityManager::DwarfEntityManager() {
    }

    Entity DwarfEntityManager::CreateEntity(const string &name) {
        Entity entity {name + std::to_string(entities.capacity())};
        entity.transform = glm::mat4(1.0f);
        entities.push_back(entity);
        return entity;
    }

    vector<Entity> DwarfEntityManager::GetEntityList() {
        return entities;
    }

    void DwarfEntityManager::RemoveEntity(string name) {
        erase_if(entities, [&name](Entity &entity){
            return entity.name == name;
        });
    }

    void DwarfEntityManager::RemoveAllEntities() {
        entities.clear();
    }

}


