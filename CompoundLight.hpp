#pragma once
#include "Light.hpp"
#include "Ray.hpp"
#include "Vect.hpp"
#include <vector>
using namespace std;

class CompoundLight {

public:
  virtual int getLightsCount() { return 0; }

  Ray getShadowRay(int index, Vect position) {
    return Ray(position, getVector(index, position));
  }

  virtual double getIntensity(int index, Vect position) { return 0; }

  virtual Vect getVector(int index, Vect point) { return Vect(); }

  virtual double getTotalIntensity() { return 0; }
private:
};

class PointLight : public CompoundLight {
  Light *source = nullptr;

public:
  int getLightsCount() { return 1; }

  double getIntensity(int index, Vect position) {
    return source->getLightIntensity();
  }
  double getTotalIntensity() {
    if (source != nullptr) {
      return source->getLightIntensity();
    }
    return 0.0;
  }

  PointLight(Light *light) { source = light; }

  PointLight() : source(new Light()) {}

  PointLight(Vect pos, Color col) : source(new Light(pos, col)) {}

  Vect getVector(int index, Vect point) {
    return (source->getLightPosition()) - point;
  }

  ~PointLight() {
    if (source != nullptr) {
      delete source;
    }
    source = nullptr;
  }

private:
};

class PlaneLight: public CompoundLight{
  Vect position = Vect(0, 3, 0);
  Vect normal = Vect(0,1,0);
  Vect right = Vect(1,0,0);
  double width = 5.0;
  double height = 5.0;
  int xSubdivs = 2;
  int ySubdivs = 2;
  double intensity = 1.0;
public:
  PlaneLight(){};
  int getLightsCount() { return (xSubdivs + 1) * (ySubdivs + 1); }

  Vect getVector(int i, Vect point){
    return getSourcePoint(i) - point;
  }

  double getIntensity(int index, Vect position) {
    return intensity;
  }

  double getTotalIntensity() {
    return getLightsCount() * intensity;
  }

private:
  Vect getSourcePoint(int i){
    double halfWidth = width / 2;
    double halfHeight = height / 2;
    double stepX = width / xSubdivs;
    double stepY = height / ySubdivs;
    int x= i % xSubdivs;
    int y = (i - x)/width;
    Vect starting = position + (!right * halfWidth) + (!getDown() * halfHeight);
    return starting + (getRight().normalize() * (x* stepX)) + (getDown().normalize() * (y*stepY));
  }
  Vect getDown(){
    return (right ^ normal).normalize();
  }

  Vect getRight(){
    return right;
  }

};

class ParalellLight : public CompoundLight {
  Vect direction;
  Light *source = nullptr;

  int getLightsCount() { return 1; }

  ParalellLight():source(new Light()) {}

  ParalellLight(Light *light):source(light) {}

  ParalellLight(Vect pos, Color c):source(new Light(pos, c)) {}

  double getTotalIntensity() {
    if (source != nullptr) {
      return source->getLightIntensity();
    }
    return 0.0;
  }
  double getIntensity(int index, Vect position){
    return source->getLightIntensity();
  }

  Vect getVector(int index, Vect point) { return direction; }

  ~ParalellLight() {
    if (nullptr != nullptr) {
      delete source;
    }
    source = nullptr;
  }

private:

};
