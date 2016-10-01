#include <list>
#include <vector>
#include <SDL/SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "Hud.h"
#include "player.h"
#include "scaledMultiSprite.h"
#include "explosion.h"
#include "health.h"
#include "sound.h"
class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDLSound sound;
  World world02;
  World world01;
  Viewport& viewport;
  Hud hud;
  Health health;
  Player * player;
  bool playerDead;
  std::list<Drawable*> sprites;
  std::list<Drawable*>::iterator currentSprite;
  std::vector<ScaledMultiSprite*> butterflySprites;
  std::vector<ScaledMultiSprite*> angelSprites;
  Explosion *explosion;
  Explosion *playerExplosion;
  bool explode;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;
  int tickos;
  int interval;
  int angelNumber;	
  int score;
  bool god;

  void draw() ;
  void checkForCollisions();
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
