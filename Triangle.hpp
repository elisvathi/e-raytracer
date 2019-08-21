#pragma once
#include "Object.hpp"
#include "Color.hpp"
class Triangle: public Object{
  Vect point1, point2, point3;
public:
  Vect getPoint1(){return point1;}
  Vect getPoint2(){return point2;}
  Vect getPoint3(){return point3;}
  void setPoint1(Vect point){point1 = point;}
  void setPoint2(Vect point){point2 = point;}
  void setPoint3(Vect point){point3 = point;}
  Triangle(Vect,Vect,Vect);
  Triangle(Vect,Vect,Vect, Material*);

  UV getUV(Vect point){
    return UV(0,0);
  }

  double findIntersection(Ray ray){
    Vect ray_dir = ray.getRayDirection();
    double a = ray_dir % getNormal();
    if (a == 0) {
      // ray is parallel to the plane
      return -1;
    }
    double distance = getNormal() % point1;
    double b = getNormal() % (ray.getRayOrigin() + !(getNormal() * distance));
    double dHit = -1 * b / a;
    Vect pHit = ray.getPointAtLength(dHit);
    return 0;
  }

  Vect getNormalAt(Vect point){
    return getNormal();
  }

  void flipNormal(){
    Vect temp = point2;
    point2 = point3;
    point3 = temp;
  }

private:

  Vect getNormal(){
    return ((point2 - point1)^(point3-point1)).normalize();
  }

};

Triangle::Triangle(Vect a, Vect b, Vect c)
    : Object(Color(1.0, 1.0, 1.0, 0)), point1(a), point2(b), point3(c) {}

Triangle::Triangle(Vect a, Vect b, Vect c, Material *m)
  : Object(m), point1(a), point2(b), point3(c) {}
