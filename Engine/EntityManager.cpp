#include "EntityManager.h"

using namespace std;

namespace Engine{

    vector<Entity*> EntityManager::entities;
    EntityManager* EntityManager::Instance;

    void EntityManager::Allocate() {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new EntityManager();
    }

    Entity * EntityManager::CreateEntity(const SerializedFile& file, const Texture &texture, const string &name) {
        string nameWithIndex = name + std::to_string(entities.size());
        auto *entity = new Entity();
        entity->id = entities.size();
        entity->name = nameWithIndex;
        entity->transform = glm::mat4(1.0f);
        entity->selected = false;
        entity->meshName = file.fileName;
        entity->texture = texture;
        entities.push_back(entity);
        return entity;
    }

    vector<Entity*>* EntityManager::GetEntityList() {
        return &entities;
    }

    void EntityManager::RemoveEntity(Entity *entity) {
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

    void EntityManager::RemoveEntityByName(string &name) {
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

    void EntityManager::RemoveAllEntities() {
        for_each(entities.begin(), entities.end(), [](Entity *e) {
            delete e;
            e = nullptr;
        });
        entities.clear();
    }

    EntityManager::EntityManager() {
    }




}


