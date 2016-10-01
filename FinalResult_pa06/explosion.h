#ifndef EXPLOSION__H
#define EXPLOSION__H
#include <string>
#include <vector>
#include "drawable.h"

class Explosion : public Drawable{
public:
  Explosion(const std::string&);
  Explosion(const Explosion&);
  Explosion(const std::string&,const Vector2f& , const Vector2f& );
  virtual ~Explosion(){}

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void advanceFrame(Uint32 ticks);
  std::string getType() const { return "Explosion";}
  virtual const Frame* getFrame() const{
    return frames[currentFrame];
  }
  bool getComplete() const{
    return complete;
  }
  void setComplete(bool state){
	complete = state;
  }
protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;
  bool complete;

};
#endif
