#include "explosion.h"
#include "gamedata.h"
#include "frameFactory.h"
#include <cmath>
void Explosion::advanceFrame(Uint32 ticks){
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval){
    currentFrame = (currentFrame + 1) % (numberOfFrames);
    if (currentFrame == numberOfFrames - 1) complete = true;
    timeSinceLastFrame = 0;
	}
}

Explosion::Explosion(const std::string& name):
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  frames(FrameFactory::getInstance().getFrames(name)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),

  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  complete(false)
{ }
Explosion::Explosion( const std::string& name, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name,pos,vel),
  frames(FrameFactory::getInstance().getFrames(name)),//
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  complete(false)
 { }

Explosion::Explosion(const Explosion& s):
  Drawable(s),
  frames(s.frames),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight),
  currentFrame(s.currentFrame),
  numberOfFrames(s.numberOfFrames),
  frameInterval(s.frameInterval),
  timeSinceLastFrame(s.timeSinceLastFrame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  complete(false)
{ }

void Explosion::draw() const{
  if (!complete){
  	Uint32 x = static_cast<Uint32>(X());
  	Uint32 y = static_cast<Uint32>(Y());
  	frames[currentFrame]->draw(x,y);
  }
}

void Explosion::update(Uint32 ticks){
  advanceFrame(ticks);
}








