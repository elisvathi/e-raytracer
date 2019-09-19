#pragma once
#include "Object.hpp"
#include "Ray.hpp"
#include "Material.hpp"
class AARect: public Object{
public:
  double x0, x1, y0, y1, k;
  int direction = 0;
  AARect(){}
  // direction 0 XY plane, direction 1 XZ plane, direction 2 YZ plane
  AARect(double _x0, double _x1, double _y0, double _y1, double _k,
         Material *mat, int dir = 0)
    : Object(mat), x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k) , direction(dir){}

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    double dist = 0.01;
    if (direction == 0) {
      box = BoundingBox(Vect(x0, y0, k - dist), Vect(x1, y1, k + dist));
    } else if (direction == 1) {
      box = BoundingBox(Vect(x0, k - dist, y0), Vect(x1, k + dist, y1));
    }else{
      box = BoundingBox(Vect(k - dist, x0, y0), Vect(k + dist, x1, y1));
    }
    return true;
  };

  virtual double findIntersection(Ray ray) {
    // return 0;
    Vect origin = ray.getRayOrigin();
    Vect raydir = ray.getRayDirection();
    int f0 = 0;
    int f1 = 1;
    int f2 = 2;
    if(direction == 1){
      f1 = 2;
      f2 = 1;
    }else if(direction == 2 ){
      f0 = 1;
      f1 = 2;
      f2 = 0;
    }

    // DISTANCE TO HIT THE PLANE
    double t = (k - origin[f2]) / raydir[f2];
    double x = origin[f0] + t*raydir[f0];
    double y = origin[f1] + t*raydir[f1];
    // CHECK IF IS INSIDE THE RECTANGLE
    if(x < x0 || x > x1 || y < y0 || y > y1){
      return -1;
    }
    return t;
  }

  virtual Vect getNormalAt(Vect point){
    return Vect(0,0,1);
  }

  virtual void getUV(Vect point, double& u, double& v){
    u = (point.x() - x0)/ (x1 - x0);
    v = (point.y() - y0)/ (y1 - y0);
  }

};

class XYRect: public AARect{
public:
  XYRect(double _x0, double _x1, double _y0, double _y1, double _k,
         Material *mat): AARect(_x0, _x1, _y0, _y1, _k, mat, 0){}
};

class XZRect: public AARect{
public:
  XZRect(double _x0, double _x1, double _y0, double _y1, double _k,
         Material *mat): AARect(_x0, _x1, _y0, _y1, _k, mat, 1){}
};

class YZRect: public AARect{
public:
  YZRect(double _x0, double _x1, double _y0, double _y1, double _k,
         Material *mat): AARect(_x0, _x1, _y0, _y1, _k, mat, 2){}
};
