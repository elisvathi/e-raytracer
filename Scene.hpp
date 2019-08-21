#pragma once
#include "Light.hpp"
#include "CompoundLight.hpp"
#include "Object.hpp"
#include "Vect.hpp"
#include <vector>

using namespace std;

class Scene {
public:
  vector<CompoundLight *> lights;
  vector<Object *> objects;
  void addObject(Object *object) { objects.push_back(object); }
  void addLight(CompoundLight *light) { lights.push_back(light); }
};
