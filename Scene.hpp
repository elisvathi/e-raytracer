#ifndef _SCENE_H
#define _SCENE_H

#include "Light.hpp"
#include "Object.hpp"
#include "Vect.hpp"
#include <vector>

using namespace std;

class Scene {
  vector<Light *> lights;
  vector<Object *> objects;

public:
  void addObject(Object *object) { objects.push_back(object); }

  void addLight(Light *light) { lights.push_back(light); }

  Object* getFirstHit(Ray ray){
    int index = getFirstHitIndex(ray);
    if(index > 0){
      return objects[index];
    }
    return nullptr;
  }

private:

  vector<double> findIntersections(Ray ray) {
    vector<double> inters;
    for (int i = 0; i < objects.size(); i++) {
      inters.push_back(objects[i].findIntersection(ray));
    }
    return inters;
  }

  int getFirstHitIndex(Ray ray) {
    vector<double> intersections = findIntersections(ray);
    return minPositiveIndex(intersections);
  }

  double getFirstHitDistance(Ray ray){
    vector<double> intersections = findIntersections(ray);
    int index = minPositiveIndex(intersections);
    return intersections[index];
  }

  bool IsShadow(Light* light, Ray ray, double dist){
    Vect hitPoint = ray.getPointAtLength(dist);
    Vect lightRay = light->getLightPosition() - hitPoint;
    double mag = lightRay.magnitude();
    Ray newRay(hitPoint, lightRay);
    for(int i = 0; i < objects.size(); i++){
      double hit = objects[i]->findIntersection(newRay);
      if(hit > 0 && hit < mag){
        return true;
      }
    }
    return false;
  }

  int minPositiveIndex(vector<double> values) {
    if(values.size() == 0) {
      return -1;
    }
    if (values.size() == 1) {
      if (values[0] > 0) {
        return 0;
      }
      return -1;
    }
    double selected_value = - 1;
    int selected_index = -1;

    for(int i = 0; i < values.size(); i++) {
      bool cond1 = selected_value > 0;
      bool cond2 = values[i] > 0;
      bool cond3 = values[i] < selected_value;
      if((cond1 && cond2 && cond3) || (!cond1 && cond2)){
        selected_value = values[i];
        selected_index = i;
      }
    }
    return selected_index;
  }

};

#endif
