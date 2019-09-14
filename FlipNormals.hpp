#pragma once
#include "Hitable.hpp"

class FlipNormals: public Hitable{
public:
  FlipNormals(Hitable *p):pointer(p){}
  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
    if(pointer->hit(r,t_min, t_max, rec)){
      rec.normal = -rec.normal;
      return true;
    }
    return false;
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    return pointer->bounding_box(t0, t1, box);
  }
  Hitable *pointer;
};
