#pragma once
#include "Utils.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"


class Material;

struct HitRecord {
  double t;
  Vect p;
  Vect normal;
  Material* material;
  double u;
  double v;
};

class Hitable {
public:
  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) = 0; 
  virtual bool bounding_box(double t0, double t1, BoundingBox& box) = 0;
};

