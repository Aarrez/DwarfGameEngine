#ifndef PHYSICS_H
#define PHYSICS_H



class Physics {

public:

  void Simulate(float delta);

  void CheckCollision();
  void HandleGravity();



};



#endif //PHYSICS_H
