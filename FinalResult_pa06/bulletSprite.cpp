#include <cmath>
#include "gamedata.h"
#include "frameFactory.h"
#include "bulletSprite.h"
BulletSprite::BulletSprite(const std::string& name,bool direction) : 
  MultiSprite(name),
  directionRight(direction),
  v(Vector2f(
    abs(Gamedata::getInstance().getXmlInt(name+"/speedX")),
    -abs(Gamedata::getInstance().getXmlInt(name+"/speedY")))
   )
{ }

BulletSprite::BulletSprite(const std::string& name, const Vector2f& pos,const Vector2f& vel,bool direction) :
  MultiSprite(name,pos,vel),
  directionRight(direction),
  v(Vector2f(
    abs(Gamedata::getInstance().getXmlInt(name+"/speedX")),
    -abs(Gamedata::getInstance().getXmlInt(name+"/speedY")))
  )
{
  reset(pos, vel,direction);
}
void BulletSprite::draw() const{
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x,y); 
}

void BulletSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  if(directionRight){
  	setPosition(getPosition() + incr);
  }else{
    setPosition(getPosition() - incr);
  }
}



void BulletSprite::reset(const Vector2f & pos, const Vector2f & vel,bool direction)
{
  setPosition(pos);
  setVelocity(vel);
  setVelocity(v);
  directionRight = direction;
}

