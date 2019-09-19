#pragma once
#include "Utils.hpp"
#include "Ray.hpp"
#include <stdlib.h>

using namespace std;
class BoundingBox{
public:
  BoundingBox(){}
  Vect _min;
  Vect _max;
  BoundingBox(Vect a, Vect b): _min(a), _max(b){}
  Vect getMin(){return _min;}
  void setMin(Vect value){_min = value;}
  Vect getMax(){return _max;}
  Vect min(){return _min;}
  Vect max(){return _max;}
  void setMax(Vect value){_max = value;}
  bool hit(Ray r, float tmin, float tmax){
    for(int a = 0;  a < 3; a++){
      double inverseDirection = 1.0f / r.getRayDirection()[a];
      float t0 = (min()[a] - r.getRayOrigin()[a]) * inverseDirection;
      float t1 = (max()[a] - r.getRayOrigin()[a]) * inverseDirection;
      if(inverseDirection < 0.0){
        swap(t0, t1);
      }
      tmin = t0 > tmin ? t0 : tmin;
      tmax = t1 < tmax ? t1 : tmax;
      if(tmax <= tmin){
        return false;
      }
    }
    return true;
  }
};

BoundingBox surroundingBox(BoundingBox b1, BoundingBox b2){
  Vect small =
      Vect(dmin(b1.min().x(), b2.min().x()), dmin(b1.min().y(), b2.min().y()),
           dmin(b1.min().z(), b2.min().z()));
  Vect big =
    Vect(dmax(b1.max().x(), b2.max().x()), dmax(b1.max().y(), b2.max().y()),
         dmax(b1.max().z(), b2.max().z()));
  return BoundingBox(small, big);
}

