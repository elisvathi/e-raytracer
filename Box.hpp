#pragma once
#include "Hitable.hpp"
#include "HitableList.hpp"
#include "BoundingBox.hpp"
#include "Object.hpp"
#include "Material.hpp"
#include "XYRect.hpp"

class Box: public Hitable{
public:
  Vect pmin, pmax;
  Box(){}
  Hitable *list_ptr;
  Box(Vect p0, Vect p1, Material* mat): pmin(p0), pmax(p1) {
    Hitable** list = new Hitable*[6];
    list[0] = new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat);
    list[1] = new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat);

    list[2] = new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat);
    list[3] = new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat);

    list[4] = new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat);
    list[5] = new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat);
    list_ptr = new SimpleHitableList(list, 6);
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box)
  {
    box = BoundingBox(pmin, pmax);
    return true;
  }

  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
    return list_ptr->hit(r, t_min, t_max, rec);
  }

};
