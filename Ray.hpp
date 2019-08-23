#pragma once
#include "Vect.hpp"
#include "Utils.hpp"
class Ray {
  Vect origin, direction;
  int bounces = 0;
public:
  Ray();
  Ray(Vect, Vect);
  // method functions
  Vect getRayOrigin(){return origin;}
  Vect getRayDirection(){return direction;}
  Vect getPointAtLength(double length){
    return origin + (direction * length);
  }

  int getBounces(){return bounces;}

  void setBounces(int b){bounces = b;}

  Ray getReflectedRay(Vect orig, Vect ref, double fuzz = 0.0 ){
    Vect dir = direction.reflect(ref);
    if(fuzz > 0){
      dir = dir + (randomUnit() * fuzz) ;
    }
    Ray r (orig,dir);
    r.bounces = bounces + 1;
    return r;
  }


  Ray getRandomObjectRay(Vect origin, Vect normal, double fuzz = 1.0){
    Vect dir = !normal;
    if(fuzz > 0){
      dir = dir + (randomUnit() * fuzz) ;
    }
    Ray r (origin,dir);
    r.bounces = bounces + 1;
    return r;
  }

  Ray getRefractedRay(Vect orig, Vect ref, double ior){
    Ray r (orig, direction.refract(direction, ref,  ior));
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

