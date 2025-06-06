#ifndef LIGHTENTITYMANAGER_H
#define LIGHTENTITYMANAGER_H
#include <iostream>
#include <vector>
#include <string>
#include "../Misc/LightVariables.h"

namespace Engine {
    class LightEntityManager {
    public:
        static void Allocate();
        static LightEntityManager& Get();

        LightEntity* CreateLight(LightTypes& type, const char* name = "");
        void DestroyLightByName(const std::string& name);
        void DestroyLightByType(LightTypes type);
        void DestroyAllLights();
        std::vector<LightEntity*>& GetAllLights();
        [[nodiscard]] int GetNumberOfLightsOfType(const LightTypes& type) const;
        void SetLightsUniforms(Shader& shader);

    private:
        LightEntityManager() = default;
        static LightEntityManager* instance;
        int PointLights;
        int SpotLights;
        int DirectionalLights;
        std::vector<LightEntity*> LightEntities;

    };
}

#endif //LIGHTENTITYMANAGER_H
