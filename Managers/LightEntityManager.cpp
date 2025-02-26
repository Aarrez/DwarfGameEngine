#include "LightEntityManager.h"

namespace Engine {
    LightEntityManager* LightEntityManager::instance = nullptr;

    void LightEntityManager::Allocate() {
        assert(instance == nullptr);
        if (instance) return;
        instance = new LightEntityManager();
    }

    LightEntityManager & LightEntityManager::Get() {
        return *instance;
    }

    LightEntity* LightEntityManager::CreateLight(const LightTypes type, const char* name) {
        auto* entity = new LightEntity();
        entity->type = type;
        entity->id = LightEntities.size();
        if (name && !name[0]) {
            entity->name = ToString(type) + entity->id;
        }else {
            entity->name = name;
        }
        LightEntities.push_back(entity);
        return entity;
    }

    void LightEntityManager::DestroyLightByName(const std::string& name) {
        auto it = std::find_if(LightEntities.begin(),
            LightEntities.end(), [&name](LightEntity* entity) {
                return entity->name == name;
            });
        if (it != LightEntities.end()) {
            auto pos = std::distance(LightEntities.begin(), it);
            delete (*it);
            LightEntities.at(pos) = nullptr;
            LightEntities.erase(it);
        }
    }
    void LightEntityManager::DestroyLightByType(LightTypes type) {
        for (int i = 0; i < LightEntities.size(); i++) {
            if (LightEntities.at(i)->type == type) {
                delete (LightEntities.at(i));
                LightEntities.at(i) = nullptr;
                LightEntities.erase(LightEntities.begin() + i);
            }
        }
    }
    void LightEntityManager::DestroyAllLights() {
        for (auto& entity : LightEntities) {
            delete entity;
            entity = nullptr;
        }
        LightEntities.clear();
    }

    std::vector<LightEntity *>& LightEntityManager::GetAllLights() {
        return LightEntities;
    }
}
