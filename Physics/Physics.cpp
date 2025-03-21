#include "Physics.h"

namespace Engine {

  Physics* Physics::instance = nullptr;
  void Physics::Allocate() {
    assert(instance == nullptr);
    if (instance) return;
    instance = new Physics();

  }

  Physics * Physics::Get() {
    return instance;
  }

  void Physics::Simulate(const float& deltaTime) {
    this->deltaTime = deltaTime;


    CheckCollision();

    for (auto& e : EntityManager::Get().GetEntityList()) {
      HandleGravity(e);
    }

  }

  void Physics::AddEntityToSimulate(Entity *entity) {
    entities_to_simulate.push_back(entity);
  }

  void Physics::RemoveEntityToSimulate(Entity *entity) {
    auto it = std::find_if(
      entities_to_simulate.begin(), entities_to_simulate.end(),
      [entity](Entity* e) {
        return entity->name == e->name;
    });
    if (it != entities_to_simulate.end()) {
      entities_to_simulate.erase(it);
    }
  }


  void Physics::CheckCollision() {
    if (entities_to_simulate.size() <= 1) return;

    for (int i = 0; i < entities_to_simulate.size(); i++) {
      for (int j = 0; j < entities_to_simulate.size(); j++) {
        if (i == j) continue;

        SphereIntersection(entities_to_simulate[i], entities_to_simulate[j]);
      }
    }

  }

  void Physics::HandleGravity(Entity* entity) {
    if (!entity->collider.hasGravity) return;
    entity->Translate(glm::vec3(0,gravity + gravityModifier,0) * this->deltaTime);
  }

  void Physics::SphereIntersection(Entity* ent1, Entity* ent2) {
    float dist = glm::distance(ent1->GetPosition(), ent2->GetPosition());
    float ToClose = ent1->collider.radius + ent2->collider.radius;
    if (dist <= ToClose) {
      ent1->collider.hasGravity = false;
      ent2->collider.hasGravity = false;
    }
  }

  void Physics::BoxIntersection(Entity *ent1, Entity *ent2) {
  }
}
