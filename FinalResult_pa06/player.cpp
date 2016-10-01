#include <vector>
#include "player.h"
#include "gamedata.h"
#include "ioManager.h"
#include "frameFactory.h"
#include "extractSurface.h"
#include "twoWayMultisprite.h"
#include "collisionStrategy.h"

Player::Player(const std::string& n):
	twoWayMultiSprite(n),
    bullets("bulletSprite")
{
	setVelocity(Vector2f(0,0));
}
void Player::moveLeft(){
	velocityX(-200);
    directionRight = 0;
}
void Player::moveRight(){
	velocityX(200);
    directionRight = 1;
}
void Player::moveUp(){
	velocityY(-100);
}
void Player::moveDown(){
	velocityY(100);
}
void Player::stop(){
    setVelocity(Vector2f(0,0));
}
void Player::shoot(){
  float x = X()+20;
  float y = Y()+20;
  bullets.shoot(Vector2f(x,y),getVelocity(),directionRight);
}
bool Player::hit(const Drawable* obj){
  return bullets.collidedWith(obj);
}
void Player::draw(){
  twoWayMultiSprite::draw();
  bullets.draw();
}
void Player::update(unsigned int ticks){
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks)*0.001;
  setPosition(getPosition() + incr);
  if( Y() < 400){
    velocityY(abs(0));
  }
  if(Y() > worldHeight - frameHeight){
    velocityY(-abs(0));
  }
  if( X() < 0){
    velocityX(abs(0));
    directionRight = 1;
  }
  if(X() > worldWidth - frameWidth){
    velocityX(-abs(0));
    directionRight = 0;
  }
  bullets.update(ticks);
}
bool Player::collidedWith(const Drawable* obj) const{
  PerPixelCollisionStrategy cs;
   if (cs.execute(*this,*obj)){
     return true;
   }
   else{
     return false;
   }
}
unsigned Player::getBulletCount(){
   return bullets.bulletCount();
}
unsigned Player::getFreeCount(){
   return bullets.freeCount();
}

void Player::resetPlayer(const std::string &name){
   setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                      Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
   setVelocity(Vector2f(0,0));
   bullets.resetBullets();
   directionRight = 1;
}

