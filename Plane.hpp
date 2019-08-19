#ifndef _PLANE_H
#define _PLANE_H

#include "Color.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Vect.hpp"
#include <cmath>

class Plane : public Object {

  Vect normal;
  double distance;
  Color color;

public:
  Plane();

  Plane(Vect, double, Color);

  Plane(Vect, double, Material*);
  // method function

  Vect getPlaneNormal() { return normal; }

  double getPlaneDistance() { return distance; }

  Vect getNormalAt(Vect point) { return normal; }

  UV getUV(Vect point){
    return UV(fmod(point.getVectX(), 1), fmod(point.getVectZ(),1));
  }

  double findIntersection(Ray ray) {
    Vect ray_dir = ray.getRayDirection();
    double a = ray_dir % normal;
    if (a == 0) {
      // ray is parallel to the plane
      return -1;
    }
    double b = normal % (ray.getRayOrigin() + !(normal * distance));
    return -1 * b / a;
  }
};

Plane::Plane()
    : Object(Color(0.5, 0.5, 0.5, 0.0)), normal(Vect(1, 0, 0)), distance(0) {}

Plane::Plane(Vect n, double d, Color c) : Object(c), normal(n), distance(d) {}

Plane::Plane(Vect n, double d, Material *mat)
    : Object(mat), normal(n), distance(d) {}

#endif
