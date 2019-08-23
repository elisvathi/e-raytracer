#pragma once
#include "Vect.hpp"
#include "Object.hpp"
#include "Material.hpp"
#include "Utils.hpp"
#include "math.h"

class Sphere : public Object
{
  Vect center;
  double radius;

public:
  Sphere();
  Sphere(Vect, double, Vect);
  Sphere(Vect, double, Material*);

  // method function
  Vect getSphereCenter() { return center; }
  double getSphereRadius() { return radius; }

  Vect getNormalAt(Vect point)
  {
    return (point - center).normalize();
  }

  UV getUV(Vect point){
    Vect rad = point - center;
    double yangle = rad.angleBetween(Vect(0, 1, 0));
    yangle = fmod(yangle, M_PI);
    double xangle = rad.angleBetween(Vect(1, 0, 0));
    xangle = fmod(xangle, 2 * M_PI);
    yangle = mapValue(yangle, 0, M_PI, 0, 1);
    xangle = mapValue(xangle, 0, 2 * M_PI, 0, 1);
    return UV( xangle * 16, yangle * 16 );
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

    double sphere_center_x = center.getVectX();
    double sphere_center_y = center.getVectY();
    double sphere_center_z = center.getVectZ();

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
      // ray intersects the sphere
      // first root
      double root_1 = ((-1 * b) - sqrt(discriminant)) / (2 * a) - tolerance;
      if (root_1 > 0)
      {
        // the first root is the smallest positive root;
        return root_1;
      }
      // return the second root
      return ((-1 * b) + sqrt(discriminant)) / (2 * a) - tolerance;
    }
    else
    {
      // ray doesn't intersect the sphere
      return -1;
    }
  }
};

Sphere::Sphere() : Object(Vect(0.5, 0.5, 0.5)), center(Vect()), radius(1) {}

Sphere::Sphere(Vect c, double r, Vect col)
    : Object(col), center(c), radius(r) {}

Sphere::Sphere(Vect c, double r, Material *mat)
    : Object(mat), center(c), radius(r) {}

