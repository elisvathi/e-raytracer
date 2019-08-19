#ifndef _SCENE_H
#define _SCENE_H

#include "Light.hpp"
#include "Object.hpp"
#include "Vect.hpp"
#include <vector>

using namespace std;

class Scene {
public:
  vector<Light *> lights;
  vector<Object *> objects;
  void addObject(Object *object) { objects.push_back(object); }
  void addLight(Light *light) { lights.push_back(light); }
};

#endif
