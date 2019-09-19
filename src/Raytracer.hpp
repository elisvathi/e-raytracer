#pragma once
#include "HitRecord.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "Utils.hpp"
#include "Vect.hpp"
#include "float.h"
#include <vector>

using namespace std;
class Raytracer {
  int bounces = 5;
  Scene *scene = nullptr;
  bool night = false;

public:
  Raytracer(Scene *sc, bool n = false) : scene(sc), night(n) {}

  Vect color(Ray ray, Hitable *world, int depth) {
    HitRecord rec;
    if (world->hit(ray, 0.001, DBL_MAX, rec)) {
      Ray scatered;
      Vect attenuation;
      Vect emitted = rec.material->emitted(rec.u, rec.v, rec.p);
      if (depth < 50 &&
          rec.material->scatter(ray, rec, attenuation, scatered)) {
        return emitted + (attenuation * color(scatered, world, depth + 1));
      } else {
        return emitted;
      }
    } else {
      if (night) {
        return Vect(0, 0, 0);
      }
      return getSkyColor(ray);
    }
  }

  Vect color(Ray ray, int depth) { return color(ray, scene, depth); }

private:
  Vect getSkyColor(Ray ray) {
    // RETURN SKY
    Vect unit = ray.getRayDirection().normalize();
    double t = 0.5 * (unit.getVectY() + 1.0);
    return (Vect(1.0, 1.0, 1.0) * (1.0 - t)) + (Vect(0.5, 0.7, 1.0) * t);
  }
};
