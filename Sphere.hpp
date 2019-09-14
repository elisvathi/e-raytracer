#pragma once
#include "Vect.hpp"
#include "Object.hpp"
#include "Material.hpp"
#include "Utils.hpp"
#include "math.h"
#include "BoundingBox.hpp"

class Sphere : public Object
{
protected:
  Vect center;
  double radius;
public:
  Sphere();
  Sphere(Vect, double, Vect);
  Sphere(Vect, double, Material*);

  // method function
  Vect getSphereCenter() { return center; }
  double getSphereRadius() { return radius; }

  virtual Vect getCenterAtTime(double time = 0){
    return center;
  }

  virtual bool bounding_box(double t0, double t1, BoundingBox& box) {
    box = BoundingBox(center - Vect(radius, radius, radius), center + Vect(radius, radius, radius));
    return true;
  };

  Vect getNormalAt(Vect point)
  {
    return (point - center).normalize();
  }

  void getUV(Vect point, double& u, double& v){
    Vect p = (point - center) / radius;
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
  }

  double findIntersection(Ray ray)
  {
    Vect ray_origin = ray.getRayOrigin();
    double ray_origin_x = ray_origin.getVectX();
    double ray_origin_y = ray_origin.getVectY();
    double ray_origin_z = ray_origin.getVectZ();

    Vect ray_direction = ray.getRayDirection();
    double ray_direction_x = ray_direction.getVectX();
    double ray_direction_y = ray_direction.getVectY();
    double ray_direction_z = ray_direction.getVectZ();

    Vect centerAtTime = getCenterAtTime(ray.getTime());
    double sphere_center_x = centerAtTime.getVectX();
    double sphere_center_y = centerAtTime.getVectY();
    double sphere_center_z = centerAtTime.getVectZ();

    double a = 1; // normalized

    double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) +
               (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) +
               (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);

    double c = pow(ray_origin_x - sphere_center_x, 2) +
               pow(ray_origin_y - sphere_center_y, 2) +
               pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);
    double discriminant = b * b - 4 * a * c;
    static double tolerance = 0.00001;
    if (discriminant > 0)
    {
      double root_1 = ((-1 * b) - sqrt(discriminant)) / (2 * a) - tolerance;
      if (root_1 > 0)
      {
        return root_1;
      }
      return ((-1 * b) + sqrt(discriminant)) / (2 * a) - tolerance;
    }
    else
    {
      return -1;
    }
  }
};

Sphere::Sphere() : Object(Vect(0.5, 0.5, 0.5)), center(Vect()), radius(1) {}

Sphere::Sphere(Vect c, double r, Vect col)
    : Object(col), center(c), radius(r) {}

Sphere::Sphere(Vect c, double r, Material *mat)
    : Object(mat), center(c), radius(r) {}
