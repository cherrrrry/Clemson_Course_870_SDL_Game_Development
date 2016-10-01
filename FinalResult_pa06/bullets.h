#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "bulletSprite.h"
class Bullets {
public:
  Bullets(const std::string &);
  ~Bullets() {};
  void draw() const;
  void update(Uint32 ticks);

  void shoot(const Vector2f& pos, const Vector2f& vel,bool direction);
  bool collidedWith(const Drawable* obj) const;

  unsigned bulletCount() const { return bulletList.size(); }
  unsigned freeCount() const { return freeList.size(); }
  bool isShooting() const { return bulletList.empty(); }
  void resetBullets();
private:
  std::string name;
  float frameInterval;
  float timeSinceLastFrame;

  mutable std::list<BulletSprite> bulletList;
  mutable std::list<BulletSprite> freeList;
  
};

#endif
