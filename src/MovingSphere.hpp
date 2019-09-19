#pragma once
#include "Sphere.hpp"
class MovingSphere: public Sphere{
  Vect speed;
public:
  MovingSphere():Sphere(){}
  MovingSphere(Vect c, double r, Vect col):Sphere(c, r,col){}
  MovingSphere(Vect c, double r, Material *mat):Sphere(c, r, mat){}
  virtual Vect getCenterAtTime(double time){
    return center + (speed * time);
  }
  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    auto box1 = BoundingBox(getCenterAtTime(t0) - Vect(radius, radius, radius), getCenterAtTime(t0) + Vect(radius, radius, radius));
    auto box2 = BoundingBox(getCenterAtTime(t1) - Vect(radius, radius, radius), getCenterAtTime(t1) + Vect(radius, radius, radius));
    box = surroundingBox(box1, box2);
    return true;
  };
  Vect getSpeed(){return speed;}
  void setSpeed(Vect value){speed = value;}
};
