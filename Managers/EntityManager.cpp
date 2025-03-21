#include "EntityManager.h"

#include "TextureManager.h"

namespace Engine{

    std::vector<Entity*> EntityManager::entities;
    EntityManager* EntityManager::Instance;

    void EntityManager::Allocate() {
        assert(Instance == nullptr);
        if (Instance) return;
        Instance = new EntityManager();
    }

    EntityManager& EntityManager::Get() {
        return *Instance;
    }

    void EntityManager::ProcessMessages(const EntityMessage& message) {
        /*messageQueue.push(message);*/
        std::string msg = message.mMessage;
        Entity* ent;
        switch (message.mType) {
            case MessageType::CreateEntity:
                ent = CreateEntity(message.file, msg);
                if (!message.texture.fileName.empty()) {
                    ent->texture = message.texture;
                }
                if (!message.spec_texture.fileName.empty()) {
                    ent->spec_texture = message.spec_texture;
                }
                break;
            case MessageType::RemoveEntityByName:
                RemoveEntityByName(msg);
                break;
            case MessageType::RemoveAllEntities:
                RemoveAllEntities();
                break;
            default:
                std::cout << "Entity manager can not process message of type" <<
                    ToString(message.mType) << std::endl;
                break;
        }
    }

    Entity* EntityManager::CreateEntity(const SerializedFile& file, const std::string &name) {
        std::string nameWithIndex = name + std::to_string(entities.size());
        auto *entity = new Entity();
        entity->id = entities.size();
        entity->name = nameWithIndex;
        entity->transform = glm::mat4(1.0f);
        entity->collision_type = CollisionTypes::NoCollision;
        entity->collider.velocity = glm::vec3(0);
        entity->collider.simulate = false;
        entity->collider.hasGravity = false;
        entity->meshName = file.fileName;
        entity->shininess = 2.f;
        entities.push_back(entity);
        return entity;
    }

    std::vector<Entity*>& EntityManager::GetEntityList() {
        return entities;
    }

    void EntityManager::RemoveEntityByName(const std::string &name) {
        if (entities.empty()) return;
        for (int i = 0; i < entities.size(); i++) {
            if (entities[i]->name == name) {
                delete entities[i];
                entities[i] = nullptr;
                entities.erase(entities.begin() + i);
            }
        }
    }

    void EntityManager::RemoveAllEntities() {
        std::ranges::for_each(entities.begin(), entities.end(), [](Entity *e) {
            delete e;
            e = nullptr;
        });
        entities.clear();
    }
}


