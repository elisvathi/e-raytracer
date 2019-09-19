#pragma once
#include "Vect.hpp"
#include "Ray.hpp"
class Bounds3D {
  Vect center;
  double width, height, depth;
public:
  Bounds3D():center(Vect()), width(1), height(1), depth(1){}
  Bounds3D(Vect c, double w, double h, double d):center(c), width(w), height(h), depth(d){}
  bool intersects(Ray r){
    return false;
  }

  Bounds3D* divideX(double pos){
    return new Bounds3D[2];
  }

  Bounds3D* divideY(double pos){
    return new Bounds3D[2];
  }

  Bounds3D* divideZ(double pos){
    return new Bounds3D[2];
  }

};
