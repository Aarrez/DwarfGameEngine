#ifndef SHADOWMANAGER_H
#define SHADOWMANAGER_H

#include "../GL/Shader.h"
#include "../Misc/PublicStructs.h"
#include <functional>
#include <assert.h>
#include "../Engine/GameEngine.h"

namespace Engine {
  class ShadowManager {
    ShadowManager() = default;
    ~ShadowManager() = delete;

    static ShadowManager* instance;

    /*ShadowMap shadow_map;*/

  public:

    static void Allocate();
    static ShadowManager* Get();

    /*ShadowMap& GetShadowMap();*/

    void GenerateShadowMap();

    void DrawShadowMap(const ShadowMap& shadow_map);
  };
}




#endif //SHADOWMANAGER_H
