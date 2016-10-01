#include "frameFactory.h"
#include <sstream>
#include "extractSurface.h"
#include "ioManager.h"
#include "vector2f.h"
#include "SDL/SDL_rotozoom.h"
FrameFactory::~FrameFactory() {
  //std::cout << "The FrameFactory has leaks" << std::endl;
  
  std::map<std::string,Frame*>::const_iterator ptr1 = frames.begin();
  while (ptr1 != frames.end()){
    delete (ptr1->second);
	ptr1 ++;
  }
 // frames.clear();
  std::map<std::string,std::vector<Frame*> >::const_iterator ptr2 = multiFrames.begin();
  while(ptr2 != multiFrames.end()){
    std::vector<Frame*>::const_iterator ptr3 = ptr2->second.begin();
	while(ptr3 !=(ptr2->second).end()){
		delete (*ptr3);
		ptr3++;
	}
   // ptr2->second.clear();
	ptr2++;
  }
 // multiFrames.clear();
  std::map<std::string,std::vector<Frame*> >::const_iterator ptr9 = scaledMultiFrames.begin();
  while(ptr9 != scaledMultiFrames.end()){
    std::vector<Frame*>::const_iterator ptr10 = ptr9->second.begin();
	while(ptr10 !=(ptr9->second).end()){
		delete (*ptr10);
		ptr10++;
	}
	ptr9++;
  }

  std::map<std::string,SDL_Surface*>::const_iterator ptr4 = surfaces.begin();
  while(ptr4 != surfaces.end()){
    SDL_FreeSurface(ptr4->second);
	ptr4++;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::const_iterator ptr5 = multiSurfaces.begin();
  while(ptr5 != multiSurfaces.end()){
    std::vector<SDL_Surface*>::const_iterator ptr6 = (ptr5->second).begin();
	while(ptr6 != (ptr5->second).end()){
	  SDL_FreeSurface(*ptr6);
	  ptr6 ++;
	}
	ptr5 ++;
  }
  std::map<std::string, std::vector<SDL_Surface*> >::const_iterator ptr7 = scaledMultiSurfaces.begin();
  while(ptr7 != scaledMultiSurfaces.end()){
    std::vector<SDL_Surface*>::const_iterator ptr8 = (ptr7->second).begin();
	while(ptr8 != (ptr7->second).end()){
	  SDL_FreeSurface(*ptr8);
	  ptr8 ++;
	}
	ptr7 ++;
  }

}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
    std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
    if ( pos == frames.end() ) {
    SDL_Surface * const surface =
      IOManager::getInstance().loadAndSet(
          gdata.getXmlStr(name+"/file"),
          gdata.getXmlBool(name+"/transparency"));
    surfaces[name] = surface;
    Frame * const frame =new Frame(name, surface);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<Frame*> FrameFactory::getFrames(const std::string& name) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
   surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    surfaces.push_back( surf );
    frames.push_back( new Frame(name, surf) );
  }
  SDL_FreeSurface(surface);
  multiSurfaces[name] = surfaces;
  multiFrames[name] = frames;
  return frames;
}

std::vector<Frame*> FrameFactory::getScaledFrames(const std::string& name,float scale) {
  // First search map to see if we've already made it:
  std::stringstream ss;
  ss << name;
  ss << scale;
  std::string newname = ss.str();
  //std::cout << newname << std::endl;
  std::map<std::string, std::vector<Frame*> >::const_iterator 
    pos = scaledMultiFrames.find(name); 
  if ( pos != scaledMultiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"/file"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"/frames");
  std::vector<Frame*> frames;
  std::vector<SDL_Surface*> surfaces;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"/srcX");
  Uint16 srcY = gdata.getXmlInt(name+"/srcY");
  Uint16 width = gdata.getXmlInt(name+"/width");
  Uint16 height = gdata.getXmlInt(name+"/height");

  SDL_Surface* surf;
  SDL_Surface* scaledSurface;
  for (unsigned i = 0; i < numberOfFrames; ++i) {
    unsigned frameX = i * width + srcX;
    surf = ExtractSurface::getInstance().
               get(surface, width, height, frameX, srcY); 
    scaledSurface = rotozoomSurface(surf,0,scale,SMOOTHING_ON);
    surfaces.push_back( scaledSurface );
    frames.push_back( new Frame(scaledSurface,scaledSurface->w, scaledSurface->h,
                                Gamedata::getInstance().getXmlInt(name+"/srcX"),
                                Gamedata::getInstance().getXmlInt(name+"/srcY")));
  }
  SDL_FreeSurface(surface);
  scaledMultiSurfaces[newname] = surfaces;
  scaledMultiFrames[newname] = frames;
  return frames;
}
