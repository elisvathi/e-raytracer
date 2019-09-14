#pragma once
#include "Hitable.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include <random>
#include "float.h"
#include "math.h"
#include <iostream>

class ConstantVolume : public Hitable{
public:
  Hitable* boundary;
  double density;
  Material *mat;
  ConstantVolume(Hitable *b, float d, Texture *a): boundary(b), density(d){
    mat = new Isotropic(a);
  }
  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec){
    HitRecord rec1, rec2;
    if(boundary->hit(r, -DBL_MAX, DBL_MAX, rec1)){
      if(boundary->hit(r, rec1.t + 0.01, DBL_MAX, rec2)){
        if(rec1.t < t_min)
          rec1.t = t_min;
        if(rec2.t  > t_max)
          rec2.t = t_max;
        if(rec1.t >= rec2.t)
          return false;
        if(rec1.t < 0)
          rec1.t = 0;
        double distanceInBounds = (r.getRayDirection()*(rec2.t - rec1.t)).magnitude();
        double hitDistance = -(1/ density)* log(drand48());
        if(hitDistance < distanceInBounds){
          rec.t = rec1.t + hitDistance / r.getRayDirection().magnitude();
          rec.p = r.getPointAtLength(rec.t);
          rec.normal = Vect(1,0,0); // TODO Find a way to represent this
          rec.material = mat;
          return true;
        }
      }
    }
    return false;
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    return boundary->bounding_box(t0, t1, box);
  }

};
