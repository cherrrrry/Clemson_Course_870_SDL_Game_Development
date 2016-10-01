#ifndef PLAYER__H
#define PLAYER__H
#include <iostream>
#include <vector>
#include "twoWayMultisprite.h"
#include "bullets.h"
class Player : public twoWayMultiSprite {
public:
   ~Player() {}
   Player(const std::string&);
   void moveLeft();
   void moveRight();
   void moveUp();
   void moveDown();
   void stop();
   void shoot();
   bool hit(const Drawable* obj);
   void draw();   
   void update(unsigned int ticks);
   bool collidedWith(const Drawable* obj) const;
   void resetPlayer(const std::string &name);
   unsigned getBulletCount();
   unsigned getFreeCount();
private:
   Bullets bullets;
};
#endif
