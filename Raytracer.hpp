#pragma once
#include "Scene.hpp"
#include "Vect.hpp"
#include "Object.hpp"
#include "Utils.hpp"


int minPositiveIndex(vector<double> values) {
  if (values.size() == 0) {
    return -1;
  }
  if (values.size() == 1) {
    if (values[0] > 0) {
      return 0;
    }
    return -1;
  }
  double selected_value = -1;
  int selected_index = -1;
  for (unsigned long i = 0; i < values.size(); i++) {
    bool cond1 = selected_value > 0;
    bool cond2 = values[i] > 0;
    bool cond3 = values[i] < selected_value;
    if ((cond1 && cond2 && cond3) || (!cond1 && cond2)) {
      selected_value = values[i];
      selected_index = i;
    }
  }
  return selected_index;
}

class HitData {
  public:
  int index;
  double distance;
  HitData(int i, double d) {
    index = i;
    distance = d;
  }
};

class Raytracer{
  int bounces = 5;
  Scene* scene = nullptr;
  public:
  Raytracer(Scene* sc): scene(sc){}

  Color getColor(Ray ray) {
    HitData data = getHitData(ray);
    if (data.index >= 0) {
      Object *object = scene->objects[data.index];
      Vect intersectionPoint = ray.getPointAtLength(data.distance);
      Color result = getIlluminationColor(object, intersectionPoint);
      if (ray.getBounces() >= bounces) {
        return result;
      }
      // Color reflectedColor = getReflectedColor(object, intersectionPoint, ray);
      // Color refractedColor = getRefractedColor(object, intersectionPoint, ray);
      // return result.add(reflectedColor);
      return result;
    }
    return Color(0, 0, 0, 0);
  }
private:

  Color getIlluminationColor(Object *object, Vect point){
    Color color = object->getColorAt(point);
    double lightIntensity = getTotalSceneIntensity(object, point);
    return color.multiply(lightIntensity);
  }

  Color getReflectedColor(Object *object, Vect point, Ray ray) {
    if (object->get_material()->get_reflectionCoeficient() == 0) {
      return Color(0, 0, 0, 0);
    }
    return getColor(ray.getReflectedRay(point, object->getNormalAt(point)))
        .multiply(object->get_material()->get_reflectionCoeficient());
  }

  Color getRefractedColor(Object *object, Vect point, Ray ray){
    if(object->get_material()->get_refractionCoeficient() == 0){
      return Color(0,0,0,0);
    }
    return getColor(ray.getRefractedRay(point, object->getNormalAt(point), object->get_material()->get_refractionIOR())).multiply(object->get_material()->get_refractionCoeficient());

  }

  vector<double> findIntersections(Ray ray) {
    vector<double> inters;
    for (unsigned long i = 0; i < scene->objects.size(); i++) {
      inters.push_back(scene->objects[i]->findIntersection(ray));
    }
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
    return getTotalIntensity(object, hitPoint) / getMaximumIntensity();
  }

  double intensity(Object *object, CompoundLight *light, Vect hitPoint) {
    int lightCount = light->getLightsCount();
    double value = 0.0;
    for(int i = 0; i< lightCount; i++){
      Vect shadowVect = light->getVector(i, hitPoint);
      Ray newRay(hitPoint, shadowVect);
      double mag = shadowVect.magnitude();
      Vect objNormal = object->getNormalAt(hitPoint);
      for (unsigned long i = 0; i < scene->objects.size(); i++) {
        double hit = scene->objects[i]->findIntersection(newRay);
        if (hit > 0 && hit < mag) {
          return 0;
        }
      }
      double angle = objNormal.angleBetween(shadowVect);
      angle = abs(angle);
      if (angle > M_PI / 2) {
        return 0;
      }
      double intens = mapValue(angle, 0, M_PI / 2, light->getIntensity(i, hitPoint), 0);
      value += intens;
    }
    return value;
  }

};
