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
      rec.material = get_material();
      Vect point = r.getPointAtLength(distance);
      getUV(point, rec.u, rec.v);
      rec.normal = getNormalAt(point);
      rec.p = point;
      return true;
    }
    else
    {
      return false;
    }
  }

  virtual double findIntersection(Ray ray) = 0;
  virtual Vect getNormalAt(Vect point) = 0 ;
  virtual void getUV(Vect point, double& u, double& v) = 0;
};

Object::Object() : Hitable(), material(new Lambertian(new ConstantTexture(Vect(0.5,0.5,0.5)))) {}
Object::Object(Vect c) :Hitable(), material(new Lambertian(new ConstantTexture(c))){}
Object::Object(Material *m) :Hitable(),  material(m) {}
