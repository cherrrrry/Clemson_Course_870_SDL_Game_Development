#ifndef SCALEDMULTISPRITE__H
#define SCALEDMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class ScaledMultiSprite : public Drawable {
public:
  ScaledMultiSprite(const std::string&, const std::vector<Frame*>&);
  ScaledMultiSprite(const std::string&);//
  ScaledMultiSprite(const std::string&, const Vector2f&, Vector2f);
  virtual ~ScaledMultiSprite() { } 

  ScaledMultiSprite& operator=(const ScaledMultiSprite&);
  bool operator<(const ScaledMultiSprite& rhs){
    return scale < rhs.scale;
  } 
  double getScale() const{return scale;}
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

protected:
  double scale;
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
 // ScaledMultiSprite(const ScaledMultiSprite&);
 // ScaledMultiSprite& operator=(const ScaledMultiSprite&);
};
#endif
