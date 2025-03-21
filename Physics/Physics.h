#ifndef PHYSICS_H
#define PHYSICS_H
#include "../Managers/EntityManager.h"


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
    void SphereIntersection(Entity* ent1, Entity* ent2);
    void BoxIntersection(Entity* ent1, Entity* ent2);
  };
}



#endif //PHYSICS_H
