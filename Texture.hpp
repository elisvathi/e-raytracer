#pragma once
#include "Vect.hpp"
#include <vector>
#include <cmath>
using namespace std;

class Texture {
public:
  int width, height;
  vector<Vect> colors;
  Texture(){}
  Vect getColor(int x, int y){
    return getColor(indexFromCoords(x, y));
  }

  virtual Vect getColor(double u, double v){
    u = fmod(u, 1.0);
    v = fmod(v, 1.0);
    return getColor((int)floor(u*width), (int)floor(v*width));
  }

private:

  Vect getColor(int index){
    return colors[index];
  }

  int indexFromCoords(int x, int y){
    return y * width + x;
  }

};

class Checkerboard: public Texture{
  public:
  Checkerboard(){}
  Vect getColor(double u, double v){
    double coef = 0.5;
    bool c1 = (int)floor(u / coef) % 2 == 0;
    bool c2 = (int)floor(v / coef) % 2 == 0;
    // c1 = c1 && u > 0;
    // c2 = c2 && v > 0;
    if(c1 != c2){
      return Vect(0.0, 0.0, 0.0);
    }
    return Vect(1.0, 1.0, 1.0);
  }
};
