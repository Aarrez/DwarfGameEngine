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

  public:

    static void Allocate();
    static ShadowManager* Get();

    void GenerateDepthMap(ShadowMap* shadow_map);

    void DrawShadowMap(const ShadowMap& shadow_map);
  };
}




#endif //SHADOWMANAGER_H
