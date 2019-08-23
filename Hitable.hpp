#pragma once
#include "Utils.hpp"
#include "Ray.hpp"


class Material;

struct HitRecord {
  double t;
  Vect p;
  Vect normal;
  Material* material;
};

class Hitable {
public:
  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) = 0; 
};

