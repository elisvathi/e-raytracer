#pragma once
#include "Ray.hpp"
#include "Vect.hpp"
#include "Material.hpp"
#include "Hitable.hpp"

class UV
{
public:
  double u = 0;
  double v = 0;
  UV() : u(0), v(0) {}
  UV(double _u, double _v) : u(_u), v(_v) {}
};

class Object : public Hitable
{
  Material *material;

public:
  Object();
  Object(Vect);
  Object(Material *);
  // method function
  Material *get_material()
  {
    return material;
  }

  void set_material(Material *mat)
  {
    material = mat;
  }

  virtual bool hit(Ray &r, double t_min, double t_max, HitRecord &rec)
  {
    double distance = findIntersection(r);
    if (distance > t_min && distance < t_max)
    {
      rec.t = distance;
      Vect point = r.getPointAtLength(distance);
      rec.normal = getNormalAt(point);
      rec.p = point;
      return true;
    }
    else
    {
      return false;
    }
  }

  virtual Vect getColor() { return material->get_color(); }

  virtual Vect getColorAt(Vect point)
  {
    UV uv = getUV(point);
    return material->get_color(uv.u, uv.v);
  }

  virtual double findIntersection(Ray ray) { return 0; };
  virtual Vect getNormalAt(Vect point) { return Vect(); }
  virtual UV getUV(Vect point) { return UV(); };
};

Object::Object() : Hitable(), material(new Material()) {}
Object::Object(Vect c) :Hitable(), material(new Material(c)) {}
Object::Object(Material *m) :Hitable(),  material(m) {}
