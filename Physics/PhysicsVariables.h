#ifndef PHYSICSVARIABLES_H
#define PHYSICSVARIABLES_H
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


namespace Engine{

  struct Collider {
    glm::vec3 velocity;
    bool simulate;
    bool hasGravity;
    glm::vec3 extent;
    float radius;
  };

}

#endif //PHYSICSVARIABLES_H
