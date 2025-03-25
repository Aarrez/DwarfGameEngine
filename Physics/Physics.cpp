#include "Physics.h"
#include <algorithm>

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

    for (auto& e : entities_to_simulate) {
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
        if (IntersectionSelect(entities_to_simulate[i], entities_to_simulate[j])) {
          entities_to_simulate[i]->collider.hasGravity = false;
          entities_to_simulate[j]->collider.hasGravity = false;
        }

      }
    }
  }

  void Physics::HandleGravity(Entity* entity) {
    if (!entity->collider.hasGravity) return;
    entity->Translate(glm::vec3(0,gravity + gravityModifier,0) * this->deltaTime);
  }




  bool Physics::IntersectionSelect(Entity* ent1, Entity* ent2) {
    if (ent1->collision_type == CollisionTypes::NoCollision ||
      ent2->collision_type == CollisionTypes::NoCollision) return false;

    RayHit hit;
    bool BoxColl = ent1->collision_type == CollisionTypes::BoxCollision ||
      ent2->collision_type == CollisionTypes::BoxCollision;
    bool SphereColl = ent1->collision_type == CollisionTypes::SphereCollision ||
      ent2->collision_type == CollisionTypes::SphereCollision;
    bool RayColl = ent1->collision_type == CollisionTypes::RayCollision ||
      ent2->collision_type == CollisionTypes::RayCollision;

    if (RayColl && BoxColl) {
      if (ent1->collision_type == CollisionTypes::RayCollision){
        return RayBoxIntersection(ent1, ent2, &hit);
      }
      return RayBoxIntersection(ent2, ent1, &hit);
    }
    if (RayColl && SphereColl) {
      if (ent1->collision_type == CollisionTypes::RayCollision) {
        return RaySphereIntersection(ent1, ent2, &hit);
      }
      return RaySphereIntersection(ent2, ent1, &hit);
    }
    if (SphereColl && !BoxColl) {
      return SphereIntersection(ent1, ent2);
      }
    if (!SphereColl && BoxColl) {
      return BoxIntersection(ent1, ent2);

      }
    if (SphereColl && BoxColl) {
      if (ent1->collision_type == CollisionTypes::BoxCollision) {
        return BoxSphereIntersection(ent1, ent2);
      }
      return BoxSphereIntersection(ent2, ent1);
    }



    if (RayColl) {
      return RayIntersect(ent1, ent2, &hit);
    }
    return false;
  }

  bool Physics::SphereIntersection(Entity* ent1, Entity* ent2) {
    float dist = glm::distance(ent1->GetPosition(), ent2->GetPosition());
    float ToClose = ent1->collider.radius + ent2->collider.radius;
    if (dist <= ToClose) {
      return true;
    }
    return false;
  }

  bool Physics::BoxIntersection(Entity *ent1, Entity *ent2) {
    glm::mat3 rot1 = glm::mat3(ent1->transform);
    glm::mat3 rot2 = glm::mat3(ent2->transform);
    glm::vec3 translation = glm::vec3(ent2->GetPosition() - ent1->GetPosition());

    glm::mat3 rotation = rot1 * glm::transpose(rot2);
    glm::mat3 absRot;
    for (int i {0}; i < 3; i++) {
      absRot[i] = glm::abs(rotation[i] + 0.000001f);
    }

    for (int i {0}; i < 3; i++) {
      float ra = ent1->collider.extent[i];
      float rb = glm::dot(absRot[i], ent1->collider.extent);
      float pen = (ra + rb) - glm::abs(glm::dot(translation, rot1[i]));

      if (pen < 0.0f) return false;
    }

    for (int i {0}; i < 3; i++) {
      float ra = glm::dot(absRot[i], ent1->collider.extent);
      float rb = ent2->collider.extent[i];
      float pen = (ra + rb) - glm::abs(glm::dot(translation, rot2[i]));

      if (pen < 0.0f) return false;
    }

    for (int a {0}; a < 3; a++) {
      for (int b {0}; b < 3; b++) {
        glm::vec3 axis = glm::cross(rot1[a], rot2[b]);

        if (glm::length(axis) < 0.001f) continue;
        axis = glm::normalize(axis);

        float ra = glm::dot(ent1->collider.extent, glm::abs(rot1 * axis));
        float rb = glm::dot(ent2->collider.extent, glm::abs(rot2 * axis));
        float pen = (ra + rb) - glm::abs(glm::dot(glm::normalize(translation), axis))
        * glm::length(translation);

        if (pen< 0.0f) return false;
      }
    }
    return true;
  }

  bool Physics::BoxSphereIntersection(Entity *BoxEnt, Entity *SphereEnt) {

    glm::vec3 localSphereCenter = glm::inverse(BoxEnt->transform) *
      glm::vec4(SphereEnt->GetPosition(), 1.0f);

    glm::vec3 closePoint = glm::clamp(localSphereCenter,
      -BoxEnt->collider.extent,
      BoxEnt->collider.extent);

    float dist2 = glm::length2(localSphereCenter - closePoint);

    if (dist2 < SphereEnt->collider.radius * SphereEnt->collider.radius) {
      return true;
    }
    return false;
  }

  bool Physics::RayIntersect(Entity *ent1, Entity *ent2, RayHit *hit) {
    return false;
  }

  bool Physics::RayBoxIntersection(Entity *RayEnt, Entity *BoxEnt, RayHit *hit) {
    glm::vec3 min = glm::vec3(BoxEnt->GetPosition()) - BoxEnt->collider.extent;
    glm::vec3 max = glm::vec3(BoxEnt->GetPosition()) + BoxEnt->collider.extent;

    glm::vec3 invDir = 1.0f / RayEnt->collider.direction;

    float t1 = (min.x - RayEnt->GetPosition().x) * invDir.x;
    float t2 = (max.x - RayEnt->GetPosition().x) * invDir.x;
    float t3 = (min.y - RayEnt->GetPosition().y) * invDir.y;
    float t4 = (max.y - RayEnt->GetPosition().y) * invDir.y;
    float t5 = (min.z - RayEnt->GetPosition().z) * invDir.z;
    float t6 = (max.z - RayEnt->GetPosition().z) * invDir.z;

    float tmin = std::max<float>(
      std::max<float>(std::min<float>(t1, t2),
        std::min<float>(t3, t4)),
        std::min<float>(t5, t6));
    float tmax = std::min<float>(
     std::min<float>(std::max<float>(t1, t2),
       std::max<float>(t3, t4)),
       std::max<float>(t5, t6));

    return tmax >= std::max<float>(0.0f, tmin);
  }

  bool Physics::RaySphereIntersection(Entity *RayEnt, Entity *SphereEnt, RayHit* hit) {
    glm::vec3 diff = SphereEnt->GetPosition() - RayEnt->GetPosition();

    float t0 = glm::dot(diff, RayEnt->collider.direction);

    float dSquared = glm::dot(diff, diff) - t0 * t0;

    float rSquared = SphereEnt->collider.radius * SphereEnt->collider.radius;

    if (dSquared > rSquared)
      return false;

    float t1 = glm::sqrt(rSquared - dSquared);

    float ep =  0.000001f;
    float outID = (t0 > t1 + ep) ? t0 - t1 : t0 + t1;
    if (outID > ep) {
      hit->distance = outID;
      return true;
    }
    return outID > ep;
  }
}
