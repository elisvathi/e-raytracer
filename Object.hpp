#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.hpp"
#include "Color.hpp"

class Object{
public:
  Object();
  // method function
  virtual Color getColor() { return Color(0.0, 0.0, 0.0, 0.0); }
  virtual Color getColorAt(Vect point) { return Color(0, 0, 0, 0); }
  virtual double findIntersection(Ray ray) { return 0; };

};

Object::Object(){}

#endif
