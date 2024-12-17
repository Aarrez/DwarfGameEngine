#include "DwarfEntityManager.h"

namespace Dwarf{

    vector<Entity*> DwarfEntityManager::entities;
    DwarfEntityManager* DwarfEntityManager::Instance;

    void DwarfEntityManager::Allocate() {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new DwarfEntityManager();
    }

    Entity * DwarfEntityManager::CreateEntity(const string &name) {
        string nameWithIndex = name + std::to_string(entities.size());
        Entity *entity = new Entity();
        entity->id = entities.size();
        entity->name = nameWithIndex;
        entity->transform = glm::mat4(1.0f);
        entity->selected = false;
        entities.push_back(entity);
        return entity;
    }

    vector<Entity*>* DwarfEntityManager::GetEntityList() {
        return &entities;
    }

    void DwarfEntityManager::SetEntityMesh(Entity* entity, const string& model) {
        entity->model = model;
    }

    void DwarfEntityManager::RemoveEntity(Entity *entity) {
        if (entities.empty()) return;
        auto ToRemove = stable_partition(entities.begin() , entities.end(),
            [entity](Entity *e) {
                if (e == entity) {
                    delete e;
                    e = nullptr;
                }
                return e == entity;
            });
        entities.erase(ToRemove);

    }

    void DwarfEntityManager::RemoveEntityByName(string &name) {
        if (entities.empty()) return;
        auto ToRemove = stable_partition(entities.begin() , entities.end(),
            [name](Entity *e) {
                if (name == e->name) {
                    delete e;
                    e = nullptr;
                }
               return name == e->name;
            });
        entities.erase(ToRemove);
    }

    void DwarfEntityManager::RemoveAllEntities() {
        for_each(entities.begin(), entities.end(), [](Entity *e) {
            delete e;
            e = nullptr;
        });
        entities.clear();
    }

    DwarfEntityManager::DwarfEntityManager() {
    }




}


