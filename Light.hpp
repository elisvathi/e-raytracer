#pragma once
#include "Vect.hpp"

class Light
{
  Vect position;
  Vect color;
  double intensity;

public:
  Light();
  Light(Vect, Vect);
  // method functions
  Vect getLightPosition() { return position; }
  Vect getLightColor() { return color; }
  double getLightIntensity() { return intensity; }
  void setLightIntensity(double value) { intensity = value; }
};

Light::Light() : position(Vect(0, 0, 0)), color(Vect(1, 1, 1)), intensity(1) {}

Light::Light(Vect p, Vect c) : position(p), color(c), intensity(1) {}
