#ifndef PHYSICSVARIABLES_H
#define PHYSICSVARIABLES_H
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "PublicStructs.h"
#include <iostream>

struct Entity;

namespace Engine{

  struct Collider {
    glm::vec3 velocity;
    bool simulate;
    bool hasGravity;
    glm::vec3 extent;
    float radius;
    glm::vec3 direction;
  };

  struct RayHit {
    RayHit(){}
    RayHit(const glm::vec3& _point, Entity* _entity, const float& _distance) {
      point = _point;
      entity = _entity;
      distance = _distance;
    }

    glm::vec3 point;
    Entity* entity;
    float distance;


  };

}

#endif //PHYSICSVARIABLES_H
