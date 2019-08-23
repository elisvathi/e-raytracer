#pragma once
#include "Object.hpp"
#include "Scene.hpp"
#include "Utils.hpp"
#include "Vect.hpp"
#include <vector>
#include "float.h"
#include "HitRecord.hpp"

using namespace std;
class Raytracer {
  int bounces = 5;
  Scene *scene = nullptr;

public:
  Raytracer(Scene *sc) : scene(sc) {}

  Vect color(Ray ray, Hitable *world, int depth){
    HitRecord rec;
    if(world->hit(ray, 0.001,DBL_MAX, rec)){
      Ray scatered;
      Vect attenuation;
      if(depth < 50 && rec.material->scatter(ray, rec, attenuation, scatered)){
        return attenuation * color(scatered, world, depth + 1);
      }else{
        return Vect(0,0,0);
      }
    }else{
      Vect unit = ray.getRayDirection().normalize();
      double t = 0.5 * (unit.getVectY() + 1.0);
      return (Vect(1.0, 1.0, 1.0) * (1.0 -t)) + (Vect(0.5, 0.7, 1.0) * t);
    }
  }

  Vect color(Ray ray,int depth){
    return color(ray, scene, depth);
  }


private:
};
