#pragma once
#include "Vect.hpp"
#include "Hitable.hpp"
#include "BoundingBox.hpp"
#include "math.h"
#include "float.h"

class Rotatable: public Hitable{
public:
  Hitable* pointer;
  int axis;
  double angle;
  BoundingBox bbox;
  bool hasBox;

  Rotatable(Hitable* p, double a, int x): pointer(p), axis(x), angle((M_PI/ 180.0) * a){
    hasBox = pointer->bounding_box(0, 1, bbox);
    Vect _min(DBL_MAX, DBL_MAX, DBL_MAX);
    Vect _max(-DBL_MAX,-DBL_MAX, -DBL_MAX);
    for(int i = 0; i< 2; i++){
      for(int j = 0; j < 2; j++){
        for(int k = 0; k < 2; k++){
          double x = i * bbox.max().x() + (1-i)*bbox.min().x();
          double y = j * bbox.max().y() + (1-i)*bbox.min().y();
          double z = k * bbox.max().z() + (1-i)*bbox.min().z();
          Vect tester = rotate(Vect(x,y,z));
          for(int c = 0; c < 3; c++){
            if(tester[c] > _max[c]){
              _max[c] = tester[c];
            }
            if(tester[c] < _min[c]){
              _min[c] = tester[c];
            }
          }
        }
      }
    }
    bbox = BoundingBox(_min, _max);
  }

  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
    auto rotated = rotateRay(r);
    if(pointer->hit(rotated, t_min, t_max, rec)){
      rec.p = rotate(rec.p);
      rec.normal = rotate(rec.normal);
      return true;
    }

    return false;
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    if(!hasBox){
      return false;
    }
    box = bbox;
    return true;
  }

private:
  Ray rotateRay(Ray r){

    Vect origin = rotate(r.getRayOrigin(), true);
    Vect direction = rotate(r.getRayDirection(),true);
    return Ray(origin, direction, r.getTime());
  }

  Vect rotate(Vect v, bool inverse = false){
    double ang = angle;
    if(inverse){ang *=-1;}
    if(axis == 0){
      return v.rotateX(ang);
    }else if(axis == 1){
      return v.rotateY(ang);
    }
    return v.rotateZ(ang);
  }
};

class RotateX: public Rotatable{
public:
  RotateX(Hitable* p, float a): Rotatable(p, a, 0){}
};

class RotateY: public Rotatable{
public:
  RotateY(Hitable* p, float a): Rotatable(p, a, 1){}
};

class RotateZ: public Rotatable{
public:
  RotateZ(Hitable* p, float a): Rotatable(p, a, 2){}
};
