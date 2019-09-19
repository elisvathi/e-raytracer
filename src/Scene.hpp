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
  // vector<Hitable *> objects;
  Hitable ** hitables;
  int count;
  void addObjects(Hitable **objs, int cnt){
    hitables = objs;
    count = cnt;
    buildTree(0,1);
  }
protected:
  Hitable**  getHitables(int& n){
    n = count;
    return hitables;
  }

};

