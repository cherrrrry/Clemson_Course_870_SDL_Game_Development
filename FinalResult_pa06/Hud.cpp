#include "Hud.h"
#include "aaline.h"

Hud::Hud(const std::string &name):
  io(IOManager::getInstance()),
  clock(Clock::getInstance()),
  screen(io.getScreen()),
  position(Vector2f(Gamedata::getInstance().getXmlInt(name+"/x"),Gamedata::getInstance().getXmlInt(name+"/y"))),
  length(Gamedata::getInstance().getXmlInt(name+"/length")),
  width(Gamedata::getInstance().getXmlInt(name+"/width")),
  thick(Gamedata::getInstance().getXmlInt(name+"/thick")),
  deltaTime(0),
  color(SDL_MapRGB(screen->format,0x00,0x00,0xff)),
  bShow(true){
}

void Hud::drawBox()const{
  //two horizontal lines
  Draw_AALine(screen,position[0],position[1],position[0]+length,position[1],thick,color);
  Draw_AALine(screen,position[0],position[1]+width,position[0]+length,position[1]+width,thick,color);
  //two vertical lines
  Draw_AALine(screen,position[0],position[1],position[0],position[1]+width,thick,color);
  Draw_AALine(screen,position[0]+length,position[1],position[0]+length,position[1]+width,thick,color);
  //padding
  Draw_AALine(screen,position[0],position[1]+width/2,position[0]+length,position[1]+width/2,width,0xb6,0xfc,0xd5,0xc8);
}

void Hud::drawHud(unsigned int bulletcount,unsigned int freecount){
  if(bShow){
    drawBox();
    io.printMessageValueAt("Seconds:",Clock::getInstance().getSeconds(),30,20);
    io.printMessageValueAt("fps:",Clock::getInstance().getFps(),30,40);
    io.printMessageAt("Shoot all the angel", 30, 60);
    io.printMessageAt("w : move up", 30, 80);
    io.printMessageAt("s : move down", 30, 100);
    io.printMessageAt("d : move right", 30, 120);
    io.printMessageAt("a : move left", 30, 140);
    io.printMessageAt("space : shoot", 30, 160);
    io.printMessageValueAt("BulletList : ",bulletcount, 30, 180);
    io.printMessageValueAt("BulletList : ",freecount, 30, 200);
  }else{
    io.printMessageAt("F1 help", 30, 20);
  }
  io.printMessageAt("Rui Chang", 30,450);
}

void Hud::updateHud(UInt ticks){
  deltaTime += ticks;
  if(bShow && deltaTime > 4000){
    toggle();
  }
}

void Hud::toggle(){
  bShow = !bShow;
  deltaTime = 0;
}
void Hud::resetHud(){
  bShow = true;
  deltaTime = 0;
}
