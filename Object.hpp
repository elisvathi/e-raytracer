#pragma once

#include "Ray.hpp"
#include "Color.hpp"
#include "Material.hpp"
class UV{
public:
  double u = 0;
  double v = 0;
  UV():u(0), v(0){}
  UV(double _u, double _v):u(_u), v(_v){}
};

class Object {
  Material *material;
public:

  Object();
  Object(Color);
  Object(Material*);
  // method function
  Material *get_material() {
    return material;
  }

  void set_material(Material *mat) {
    material = mat;
  }
  virtual Color getColor() { return material->get_color(); }

  virtual Color getColorAt(Vect point) {
    UV uv = getUV(point);
    return material->get_color(uv.u, uv.v);
  }

  virtual double findIntersection(Ray ray) { return 0; };
  virtual Vect getNormalAt(Vect point){return Vect();}
  virtual UV getUV(Vect point){return UV();};
};

Object::Object() : material(new Material()){}
Object::Object(Color c) : material(new Material(c)){}
Object::Object(Material *m): material(m){}
