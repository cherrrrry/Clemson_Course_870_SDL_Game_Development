#include "bullets.h"
#include "collisionStrategy.h"
#include "gamedata.h"
#include "ioManager.h"
#include "bullets.h"
Bullets::Bullets(const std::string & n) :
  name(n),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  bulletList(),
  freeList()
{}

bool Bullets::collidedWith(const Drawable* obj) const {
  PerPixelCollisionStrategy cs;
  std::list<BulletSprite>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if(cs.execute(*ptr, *obj)) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

void Bullets::shoot(const Vector2f& pos, const Vector2f& vel,bool direction) {
  if(timeSinceLastFrame > frameInterval) {
    if(freeList.empty()) {
      BulletSprite b(name, pos, vel,direction);
      bulletList.push_back(b);
    }
    else {
      BulletSprite b = freeList.front();
      freeList.pop_front();
      b.reset(pos, vel,direction);
      bulletList.push_back(b);
    }
    timeSinceLastFrame = 0;
  }
}

void Bullets::draw() const {
  //IOManager::getInstance().printMessageValueAt("Bullets: ", bulletCount(), 700, 20);
  //IOManager::getInstance().printMessageValueAt("FreeList: ", freeCount(), 700, 40);
  std::list<BulletSprite>::const_iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()) {
    ptr->draw();
    ++ptr;
  }
}
//need to change
void Bullets::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  std::list<BulletSprite>::iterator ptr = bulletList.begin();
  while(ptr != bulletList.end()) {
    ptr->update(ticks);
    if(ptr->X()>=ptr->getWorldWidth()-ptr->getFrame()->getWidth() || ptr->X()<= 0){
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }
    else ++ptr;
  }
}
void Bullets::resetBullets(){
  bulletList.clear();
  freeList.clear();
}
