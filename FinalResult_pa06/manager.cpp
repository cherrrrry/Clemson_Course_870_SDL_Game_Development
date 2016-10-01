#include <iostream>
#include <string>
#include <iomanip>
#include "vector2f.h"
#include "multisprite.h"
#include "twoWayMultisprite.h"
#include "Sprite.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
#include "player.h"
#include "scaledMultiSprite.h"
#include <algorithm>

//#include "sound.h"
Manager::~Manager() { 
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }
  sprites.clear();
  std::vector<ScaledMultiSprite*>::const_iterator ptrB = butterflySprites.begin();
  while ( ptrB !=butterflySprites.end() ) {
    delete (*ptrB);
    ++ptrB;
  }
  butterflySprites.clear();
  std::vector<ScaledMultiSprite*>::const_iterator ptrA = angelSprites.begin();
  while ( ptrA !=angelSprites.end() ) {
    delete (*ptrA);
    ++ptrA;
  }
  butterflySprites.clear();
 
  if(player!=NULL){
    delete player;
  }
  if(explosion != NULL){
    delete explosion;
  }
  if(playerExplosion!=NULL){
    delete playerExplosion;
  }
}
class ScaledMultiSpriteCompare{
public:
  bool operator()(const ScaledMultiSprite* lhs,const ScaledMultiSprite * rhs){
         return lhs->getScale() < rhs->getScale();
	}
};

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  sound(),
  world02("back02", Gamedata::getInstance().getXmlInt("back02/factor") ),
  world01("back01", Gamedata::getInstance().getXmlInt("back01/factor") ),
  viewport( Viewport::getInstance() ),
  hud("hud"),
  health("health"),
  player(new Player("sonicSprite")),
  playerDead(false),
  sprites(),
  currentSprite(),
  butterflySprites(),
  angelSprites(),
  explosion(),
  playerExplosion(),
  explode(false),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  tickos(0),
  interval(700),
  angelNumber( Gamedata::getInstance().getXmlInt("angel/number")),
  score(0),
  god(false)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back(new Sprite("cloud0"));//
  sprites.push_back(new Sprite("cloud1"));//
  sprites.push_back(new Sprite("cloud2"));//
  sprites.push_back(new MultiSprite("Mushroom1"));//
  int numb = Gamedata::getInstance().getXmlInt("butterfly/number");
  int numa = Gamedata::getInstance().getXmlInt("angel/number");
  
  for(int i = 0; i < numb; i++){
    ScaledMultiSprite* temp = new ScaledMultiSprite("butterfly");
	butterflySprites.push_back(temp);
  }
  sort(butterflySprites.begin(),butterflySprites.end(),ScaledMultiSpriteCompare());
  viewport.setObjectToTrack(player);
  
  for(int i = 0; i < numa; i++){
    SmartSprite* temp1 = new SmartSprite("angel",*player);
	angelSprites.push_back(temp1);
  }
  sort(angelSprites.begin(),angelSprites.end(),ScaledMultiSpriteCompare());
  viewport.setObjectToTrack(player);
//  SDL_WM_setCaption("*** SDL Wave Sound Demo ***",NULL);
}

void Manager::checkForCollisions() {
  
  std::vector<ScaledMultiSprite*>::iterator ptr = butterflySprites.begin();
  std::vector<ScaledMultiSprite*>::iterator ptra = angelSprites.begin();
  while(ptr != butterflySprites.end()&& playerDead == false && (angelNumber != 0 || score==0)){
    if(player->hit(* ptr)){
      score ++;
      sound[1];
      if (explosion){
        delete explosion;
      }
      explosion = new Explosion("explosion",(*ptr)->getPosition(),(*ptr)->getVelocity());
      delete *ptr;
      ptr = butterflySprites.erase(ptr);
        explode = true;
     }else if( god == false && player->collidedWith(*ptr)){
        if (explosion){
          delete explosion;
        }
        explosion = new Explosion("explosion",(*ptr)->getPosition(),(*ptr)->getVelocity());
        sound[1];
		explode = true;
		delete *ptr;
        ptr = butterflySprites.erase(ptr);
          health.updateValue(-50);
          if(health.getCurrentLength()==0){
             playerDead = true;
             std::cout<<"playerDead"<<std::endl;
             if(playerExplosion){
                delete playerExplosion;
             }
             playerExplosion = new Explosion("playerExplosion",player->getPosition(),player->getVelocity());
             sound[3];
             viewport.setObjectToTrack(playerExplosion);
        }
     }else ++ptr;
  }
  while(ptra != angelSprites.end()&& playerDead == false && angelNumber != 0){
    if(player->hit(* ptra)){
      score ++;
	  angelNumber --;
      sound[2];
      if (explosion){
        delete explosion;
      }
      explosion = new Explosion("explosion",(*ptra)->getPosition(),(*ptra)->getVelocity());
      delete *ptra;
      ptra = angelSprites.erase(ptra);
        explode = true;
     }else if(god == false && player->collidedWith(*ptra)){
        angelNumber--;
 		if (explosion){
          delete explosion;
        }
        explosion = new Explosion("explosion",(*ptra)->getPosition(),(*ptra)->getVelocity());
        sound[2];
        explode = true;
		delete *ptra;
        ptra = angelSprites.erase(ptra);
          health.updateValue(-50);
          if(health.getCurrentLength()==0){
             playerDead = true;
             std::cout<<"playerDead"<<std::endl;
             if(playerExplosion){
		       delete playerExplosion;
             }
             playerExplosion = new Explosion("playerExplosion",player->getPosition(),player->getVelocity());
             sound[3];
			 viewport.setObjectToTrack(playerExplosion);
        }
     }else ++ptra;
  }
 
}

void Manager::draw()  {
  world02.draw();
  world01.draw();
  clock.draw();
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }
  std::vector<ScaledMultiSprite*>::const_iterator ptrm = butterflySprites.begin();
  while ( ptrm != butterflySprites.end() ) {
    (*ptrm) -> draw();
    ++ ptrm;
  }
  std::vector<ScaledMultiSprite*>::const_iterator ptra = angelSprites.begin();
  while ( ptra != angelSprites.end() ) {
    (*ptra) -> draw();
    ++ ptra;
  }

  //world01.draw();
  if(explode){
  	explosion->draw();
  }
  hud.drawHud(player->getBulletCount(),player->getFreeCount());
  health.draw();
  io.printMessageValueAt("ANGEL NUMBER : ",angelNumber,660,50);
  io.printMessageValueAt("YOUR SCORE : ",score,660,70);
  if(god == true){
  	io.printMessageAt("God Mode On",660,90);
  }else{
    io.printMessageAt("God Mode Off",660,90);
  }
  if(playerDead==false){
   // io.printGameOverCenteredAt("YOU WIN",200); 
  	player->draw();
  }else{
    playerExplosion->draw();
    io.printGameOverCenteredAt("GAME OVER",200); 
    io.printMessageAt("Press R to Restart the Game",300,150); 
    io.printMessageValueAt("YOUR SCORE : ",score,350,300);
  }
  if(angelNumber == 0 && playerDead == false && score != 0){
    io.printMessageAt("Press R to Restart the Game",300,150); 
    io.printGameOverCenteredAt("YOU WIN !!!",200); 
    io.printMessageValueAt("YOUR SCORE : ",score,350,300);
  }
   viewport.draw();
  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}


void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  std::vector<ScaledMultiSprite*>::const_iterator ptrm = butterflySprites.begin();
  while ( ptrm != butterflySprites.end() ) {
    (*ptrm)->update(ticks);
    ++ptrm;
  }
  std::vector<ScaledMultiSprite*>::const_iterator ptra = angelSprites.begin();
  while ( ptra != angelSprites.end() ) {
    (*ptra)->update(ticks);
    ++ptra;
  }

  if (explode){
  	explosion->update(ticks);
  }
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  hud.updateHud(ticks);
  health.update(ticks);
  if(playerDead == false){
  	player->update(ticks);
  }else{
    playerExplosion->update(ticks);
  }
  checkForCollisions();
  world02.update();
  world01.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_s]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_F1]){
          hud.toggle();
        
        }
      }
     
      if(keystate[SDLK_g]&&(SDL_KEYDOWN)){
        if(god == false){
		  god = true;
	    }else if (god == true){
          god = false;
        }
      }
	 
      if(keystate[SDLK_r]&&(SDL_KEYDOWN)){
          sound.startMusic(); 
          playerDead = false;
          angelNumber = Gamedata::getInstance().getXmlInt("angel/number");
          score = 0;
		  player->resetPlayer("sonicSprite");
          viewport.setObjectToTrack(player);  
          health.reset();
          std::vector<ScaledMultiSprite*>::iterator ptrB = butterflySprites.begin();
          while ( ptrB !=butterflySprites.end() ) {
            delete (*ptrB);
            ++ptrB;
          }
          butterflySprites.clear();

		  std::vector<ScaledMultiSprite*>::iterator ptrA = angelSprites.begin();
          while ( ptrA !=angelSprites.end() ) {
            delete (*ptrA);
            ++ptrA;
          }
          angelSprites.clear();

          int numb = Gamedata::getInstance().getXmlInt("butterfly/number");
          int numa = Gamedata::getInstance().getXmlInt("angel/number");
          for(int i = 0; i < numb; i++){
          ScaledMultiSprite* temp = new ScaledMultiSprite("butterfly");
            butterflySprites.push_back(temp);
          }
          sort(butterflySprites.begin(),butterflySprites.end(),ScaledMultiSpriteCompare());
          hud.resetHud();
     
		   
  		  for(int i = 0; i < numa; i++){
    	 	SmartSprite* temp1 = new SmartSprite("angel",*player);
            angelSprites.push_back(temp1);
 		  }
          sort(butterflySprites.begin(),butterflySprites.end(),ScaledMultiSpriteCompare());
          viewport.setObjectToTrack(player);
 	  }
	   if(!playerDead){
        if(keystate[SDLK_a]&&SDL_KEYDOWN){
          player->moveLeft();		
	    }
        if(keystate[SDLK_d]&&SDL_KEYDOWN){
          player->moveRight();		
        }
        if(keystate[SDLK_w]&&SDL_KEYDOWN){
          player->moveUp();		
	    }
        if(keystate[SDLK_s]&&(SDL_KEYDOWN)){
          player->moveDown();		
	    }
        if(keystate[SDLK_SPACE]&&(SDL_KEYDOWN)){
	      player -> shoot();
          sound[0];
        }
        
        if(event.type == SDL_KEYUP) {
          if (keystate[SDLK_s] == false && keystate[SDLK_w] == false) {
            player->setVelocity(Vector2f(player->velocityX(),0)); 
          }  
          else if (keystate[SDLK_s])
             player->moveDown();
          else if(keystate[SDLK_w])
            player->moveUp();
          if (keystate[SDLK_a] == false && keystate[SDLK_d] == false) {
            player->setVelocity(Vector2f(0,player->velocityY())); 
          }
           else if(keystate[SDLK_a])
          {
            player->moveLeft();
          }
          else if(keystate[SDLK_d])
            player->moveRight();
        }
      }
    }
    draw();
    update();
  }
}
