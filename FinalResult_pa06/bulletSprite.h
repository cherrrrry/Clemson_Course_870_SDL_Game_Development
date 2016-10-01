#ifndef BULLETSPRITE__H
#define BULLETSPRITE__H

#include "multisprite.h"
class BulletSprite : public MultiSprite {
public:
  BulletSprite(const std::string&,bool);
  BulletSprite(const std::string&, const Vector2f &,const Vector2f &,bool);
  virtual ~BulletSprite() {};
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
      return frames[currentFrame];
  }
  void reset(const Vector2f &, const Vector2f &, bool);
private:
	
    bool directionRight ;
	Vector2f v;
};

#endif
