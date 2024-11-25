#include "DwarfEntityManager.h"

Dwarf::Entity::DwarfEntityManager::DwarfEntityManager() {
    entities.reserve(10);
}

shared_ptr<Dwarf::Entity::Entity> Dwarf::Entity::DwarfEntityManager::CreateDwarfEntity(string name) {
    shared_ptr<Entity>entity = make_shared<Entity>(name);
    entity->transform = glm::mat4(1.0f);
    entities.push_back(entity);
    return entity;
}

/*
 * No idea if this works
 * Used this -->https://www.fluentcpp.com/2018/09/18/how-to-remove-pointers-from-a-vector-in-cpp/
 */
/*void Dwarf::Entity::DwarfEntityManager::DeleteDwarfEntity(string name) {
    entities.erase(remove_if(entities.begin(), entities.end(),[&name](shared_ptr<Entity> &entity){
        return entity->name == name;
    }));
}*/


