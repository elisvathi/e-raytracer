#pragma once
#include "Object.hpp"
#include "Scene.hpp"
#include "Utils.hpp"
#include "Vect.hpp"
#include <vector>
#include "float.h"

using namespace std;
class Raytracer {
  int bounces = 5;
  Scene *scene = nullptr;

public:
  Raytracer(Scene *sc) : scene(sc) {}

  Vect color(Ray ray, Hitable *world){
    HitRecord rec;
    if(world->hit(ray, 0.0,DBL_MAX, rec)){
      Vect target = rec.p + rec.normal + randomUnit();
      return color(Ray(rec.p, target-rec.p), world) * 0.5;
    }else{
      Vect unit = ray.getRayDirection().normalize();
      double t = 0.5 * (unit.getVectY() + 1.0);
      return (Vect(1.0, 1.0, 1.0) * (1.0 -t)) + (Vect(0.5, 0.7, 1.0) * t);
    }
  }

  Vect color(Ray ray ){
    return color(ray, scene);
  }

  Vect getColor(Ray ray, bool reflections = true, bool refractions = true,
                bool ambient = true) {
    // HitData data = getHitData(ray);
    // if (data.index >= 0) {
    //   Object *object = scene->objects[data.index];
    //   Vect intersectionPoint = ray.getPointAtLength(data.distance);
    //   Vect result = getIlluminationColor(object, ray, intersectionPoint,
    //                                      reflections, refractions, ambient);
    //   if (ray.getBounces() >= bounces) {
    //     return result;
    //   }
    //   if (reflections) {
    //     Vect reflectedColor = getReflectedColor(object, intersectionPoint, ray,
    //                                             refractions, ambient);
    //     result = result + reflectedColor;
    //   }
    //   if (refractions) {
    //     Vect refractedColor = getRefractedColor(object, intersectionPoint, ray,
    //                                             reflections, ambient);
    //     result = result + refractedColor;
    //   }
    //   return result;
    // }
    // Vect unit = ray.getRayDirection().normalize();
    // double t = 0.5 * (unit.getVectY() + 1.0);
    // return (Vect(1.0, 1.0, 1.0) * (1.0 -t)) + (Vect(0.5, 0.7, 1.0) * t);
    return Vect(0, 0, 0);
  }

private:
  Vect getIlluminationColor(Object *object, Ray ray, Vect point,
                            bool reflections = true, bool refractions = true,
                            bool ambient = true) {
    Vect target = point + object->getNormalAt(point) + randomUnit();
    Ray newRay = Ray(point, target - point);
    newRay.setBounces(ray.getBounces() + 1);
    return getColor(newRay, reflections, refractions, ambient) * 0.5;
    // return getColor(Ray(point, ))
    // Vect color = object->getColorAt(point);
    // double lightIntensity = getTotalSceneIntensity(object, point);
    // if (!ambient) {
    //   return color * lightIntensity;
    // } else {
    //   int samples = 1;
    //   vector<Vect> colors;
    //   for (int i = 0; i < samples; i++) {
    //     if (ray.getBounces() >= bounces) {
    //       return color * lightIntensity;
    //     }
    //     Vect refCol =
    //         getColor(ray.getRandomObjectRay(point, object->getNormalAt(point)),
    //                  reflections, refractions, ambient);
    //     Vect col = color * lightIntensity * 0.5 + refCol * 0.3;
    //     colors.push_back(col);
    //   }
    //   return averageColors(colors);
    // }
  }

  Vect getReflectedColor(Object *object, Vect point, Ray ray,
                         bool refractions = true, bool ambient = true) {
    if (object->get_material()->get_reflectionCoeficient() == 0) {
      return Vect();
    }
    double glossiness = 1 - object->get_material()->getRefGlossiness();
    if (glossiness == 1) {
      return getColor(ray.getReflectedRay(point, object->getNormalAt(point), 0),
                      true, refractions, ambient) *
             object->get_material()->get_reflectionCoeficient();
    } else {
      int glossinessSubdivisions =
          object->get_material()->glossinessSubdivisions;
      vector<Vect> colors;
      for (int i = 0; i < glossinessSubdivisions; i++) {
        colors.push_back(getColor(
            ray.getReflectedRay(point, object->getNormalAt(point), glossiness),
            true, refractions, ambient));
      }
      return averageColors(colors);
    }
  }

  Vect getRefractedColor(Object *object, Vect point, Ray ray,
                         bool reflections = true, bool ambient = true) {
    if (object->get_material()->get_refractionCoeficient() == 0) {
      return Vect();
    }
    return getColor(
               ray.getRefractedRay(point, object->getNormalAt(point),
                                   object->get_material()->get_refractionIOR()),
               reflections, true, ambient) *
           object->get_material()->get_refractionCoeficient();
  }

  vector<double> findIntersections(Ray ray) {
    vector<double> inters;
    // for (unsigned long i = 0; i < scene->objects.size(); i++) {
    //   inters.push_back(scene->objects[i]->findIntersection(ray));
    // }
    return inters;
  }

  HitData getHitData(Ray ray) {
    vector<double> intersections = findIntersections(ray);
    int index = minPositiveIndex(intersections);
    return HitData(index, intersections[index]);
  }

  double getMaximumIntensity() {
    double value = 0;
    for (unsigned long i = 0; i < scene->lights.size(); i++) {
      value += scene->lights[i]->getTotalIntensity();
    }
    return value;
  }

  double getTotalIntensity(Object *object, Vect hitPoint) {
    double value = 0;
    for (unsigned long i = 0; i < scene->lights.size(); i++) {
      value += intensity(object, scene->lights[i], hitPoint);
    }
    return value;
  }

  double getTotalSceneIntensity(Object *object, Vect hitPoint) {
    double maximum = getMaximumIntensity();
    if (maximum == 0) {
      return 1;
    }
    return getTotalIntensity(object, hitPoint) / maximum;
  }

  double intensity(Object *object, CompoundLight *light, Vect hitPoint) {
    int lightCount = light->getLightsCount();
    double value = 0.0;
    for (int i = 0; i < lightCount; i++) {
      Vect shadowVect = light->getVector(i, hitPoint);
      Ray newRay(hitPoint, shadowVect);
      double mag = shadowVect.magnitude();
      Vect objNormal = object->getNormalAt(hitPoint);
      // for (unsigned long i = 0; i < scene->objects.size(); i++) {
      //   double hit = scene->objects[i]->findIntersection(newRay);
      //   if (hit > 0 && hit < mag) {
      //     return 0;
      //   }
      // }
      double angle = objNormal.angleBetween(shadowVect);
      angle = abs(angle);
      if (angle > M_PI / 2) {
        return 0;
      }
      double intens =
          mapValue(angle, 0, M_PI / 2, light->getIntensity(i, hitPoint), 0);
      value += intens;
      // value += light->getIntensity(i, hitPoint);
    }
    return value;
  }
};
