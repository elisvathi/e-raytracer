#pragma once
#include "Light.hpp"
#include "CompoundLight.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Vect.hpp"
#include <vector>
#include "HitableList.hpp"
#include "Hitable.hpp"

using namespace std;

class Scene : public HitableList {
public:
  Scene():HitableList(){}
  vector<CompoundLight *> lights;
  vector<Hitable *> objects;
  void addObject(Object *object) { objects.push_back(object); }
  void addLight(CompoundLight *light) { lights.push_back(light); }
  void addObjects(Hitable **objs, int count){
    for(int i = 0; i < count; i++){
      objects.push_back(objs[i]);
    }
  }
protected:
  const vector<Hitable*> getHitables(){
    return objects;
  }

};

