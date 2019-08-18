#ifndef _PLANE_H
#define _PLANE_H

#include "Vect.hpp"
#include "Object.hpp"
#include "Color.hpp"
#include "Ray.hpp"

class Plane: public Object {

  Vect normal;
  double distance;
  Color color;

public:
  Plane();
  Plane(Vect, double, Color);
  // method function
  Vect getPlaneNormal(){return normal;}
  double getPlaneDistance(){return distance;}
  Color getPlaneColor(){return color;}
  Color getColorAt(Vect point) {
    return color;
    // int cellSize = 1;
    // bool c1 = false;
    // bool c2 = false;
    // if((int)(point.getVectX() / cellSize) % 2 == 0){
    //   c1 = true;
    // }
    // if((int)(point.getVectZ() / cellSize) % 2 == 0){
    //   c2 = true;
    // }
    // if(c1 != c2){
    //   return color;
    // }
    // return Color(0.5,0.5,0.5,0);
  }
  Color getColor() { return getPlaneColor(); }

  Vect getNormalAt(Vect point){
    return normal;
  }


  double findIntersection(Ray ray){

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

Plane::Plane() : normal(Vect(1, 0, 0)), distance(0), color(Color(0.5, 0.5, 0.5, 0.0)) {}
Plane::Plane(Vect n, double d, Color c): normal(n), distance(d), color(c){}

#endif
