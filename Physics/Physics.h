#ifndef PHYSICS_H
#define PHYSICS_H
#include "../Managers/EntityManager.h"
#include "glm/gtx/norm.hpp"


namespace Engine {
  class Physics {

    Physics() = default;
    ~Physics() = default;

    static Physics* instance;

    const double gravity = -9.82;

    float deltaTime = 0;

    std::vector<Entity*> entities_to_simulate;
  public:
    static void Allocate();

    static Physics* Get();
    float gravityModifier = 0.0;
    void Simulate(const float& delta);

    void AddEntityToSimulate(Entity* entity);
    void RemoveEntityToSimulate(Entity* entity);

    void CheckCollision();
    void HandleGravity(Entity* entity);


    bool IntersectionSelect(Entity* ent1, Entity* ent2);
    bool SphereIntersection(Entity* ent1, Entity* ent2);
    bool BoxIntersection(Entity* ent1, Entity* ent2);
    bool BoxSphereIntersection(Entity* BoxEnt, Entity* SphereEnt);
    bool RayIntersect(Entity* ent1, Entity* ent2, RayHit* hit);
    bool RayBoxIntersection(Entity* RayEnt, Entity* BoxEnt, RayHit* hit);
    bool RaySphereIntersection(Entity* RayEnt, Entity* SphereEnt, RayHit* hit);
  };
}



#endif //PHYSICS_H
