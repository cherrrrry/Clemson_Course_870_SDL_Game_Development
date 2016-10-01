#ifndef HUD__H
#define HUD__H
#include"vector2f.h"
#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "aaline.h"
#include "bullets.h"
typedef unsigned int UInt;
class Hud{
public:
  Hud(const std::string &name);
  void drawHud(unsigned int ,unsigned int);
  void updateHud(UInt ticks);
  void toggle();
  void resetHud();
private:
  const IOManager &io;
  Clock &clock;
  SDL_Surface *screen;
  Vector2f position;
  int length;
  int width;
  float thick;
  int deltaTime;
  const Uint32 color;
  bool bShow;
  void drawBox() const;
  Hud(const Hud&);
  Hud& operator = (const Hud&);

};
#endif
