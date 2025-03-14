#include "ShadowManager.h"

namespace Engine {
  class GameEngine;

  ShadowManager* ShadowManager::instance = nullptr;

  void ShadowManager::Allocate() {
    assert(instance == nullptr);
    if (instance != nullptr)
      return;

    instance = new ShadowManager();
  }

  ShadowManager* ShadowManager::Get() {
    return instance;
  }

  void ShadowManager::GenerateDepthMap(ShadowMap* shadow_map) {
    glGenFramebuffers(1, &shadow_map->depthMapFBO);

    glGenTextures(1, &shadow_map->depthMap);
    glBindTexture(GL_TEXTURE_2D, shadow_map->depthMap);
    glTexImage2D(GL_TEXTURE_2D,  0, GL_DEPTH_COMPONENT,
      shadow_map->ShadowWidth, shadow_map->ShadowHeight, 0,
      GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, shadow_map->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
      GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void ShadowManager::DrawShadowMap(const ShadowMap & shadow_map) {

  }

}
