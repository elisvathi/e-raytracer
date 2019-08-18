#ifndef _RAY_H
#define _RAY_H
#include "Vect.hpp"
class Ray{
  Vect origin, direction;
  int bounces;
public:
  Ray();
  Ray(Vect, Vect);
  // method functions
  Vect getRayOrigin(){return origin;}
  Vect getRayDirection(){return direction;}
  Vect getPointAtLength(double length){
    return origin + (direction * length);
  }

  Ray getReflectedRay(Vect origin, Vect ref ){
    Ray r (origin, ref);
    r.bounces = bounces + 1;
    return r;
  }

};


Ray::Ray(){
  origin = Vect();
  direction = Vect(1, 0, 0);
}

Ray::Ray(Vect o, Vect d) {
  origin = o;
  direction = d.normalize();
}
#endif
