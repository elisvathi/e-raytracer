#pragma once
#include "Utils.hpp"
#include "Ray.hpp"

class Hitable {
public:
 int a;
  Hitable(): a(1){}
  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {return false;}
};

