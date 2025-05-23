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

    LightEntity* LightEntityManager::CreateLight(LightTypes& type, const char* name) {
        if (DirectionalLights > 0)
            return nullptr;

        auto* entity = new LightEntity();
        entity->ChangeLightType(type);
        switch (type) {
            case LightTypes::PointLight:
                entity->id = PointLights;
                PointLights++;
                break;
            case LightTypes::SpotLight:
                entity->id = SpotLights;
                SpotLights++;
                break;
            case LightTypes::DirectionalLight:
                entity->id = DirectionalLights;
                DirectionalLights++;
                break;
        }
        if (name && !name[0]) {
            entity->name = ToCString(type) + std::to_string(entity->id);
        }else {
            entity->name = name;
        }
        entity->SetScale(glm::vec3(0.25f));
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
            if (LightEntities.at(i)->GetLightType() == type) {
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

    int LightEntityManager::GetNumberOfLightsOfType(const LightTypes& type) const {
        int count = 0;
        for (auto& entity : LightEntities) {
            if (entity->GetLightType() == type) {
                count++;
            }
        }
        return count;
    }

    void LightEntityManager::SetLightsUniforms(Shader &shader) {
        for (auto& entity : LightEntities) {
            entity->SetSetUnifromValues(shader);
        }
    }
}
