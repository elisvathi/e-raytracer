#pragma once
#include "Hitable.hpp"
#include "Vect.hpp"
#include "BoundingBox.hpp"

class Translate: public Hitable{
public:
   Hitable* pointer;
   Vect offset;
   Translate(Hitable * ptr, Vect disp): pointer(ptr), offset(disp){}

  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
      Ray moved(r.getRayOrigin() - offset, r.getRayDirection(), r.getTime());
      if(pointer->hit(moved, t_min,t_max, rec)){
          rec.p = rec.p + offset;
          return true;
      }
      return false;
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
      if(pointer->bounding_box(t0, t1, box)){
          box = BoundingBox(box.min() + offset, box.max() + offset );
          return true;
      }
      return false;
  }

};
