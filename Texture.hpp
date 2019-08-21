#pragma once
#include "Color.hpp"
#include <vector>
#include <cmath>
using namespace std;

class Texture {
public:
  int width, height;
  vector<Color> colors;
  Texture(){}
  Color getColor(int x, int y){
    return getColor(indexFromCoords(x, y));
  }

  virtual Color getColor(double u, double v){
    u = fmod(u, 1.0);
    v = fmod(v, 1.0);
    return getColor((int)floor(u*width), (int)floor(v*width));
  }

private:

  Color getColor(int index){
    return colors[index];
  }

  int indexFromCoords(int x, int y){
    return y * width + x;
  }

};

class Checkerboard: public Texture{
  public:
  Checkerboard(){}
  Color getColor(double u, double v){
    double coef = 0.5;
    bool c1 = (int)floor(u / coef) % 2 == 0;
    bool c2 = (int)floor(v / coef) % 2 == 0;
    // c1 = c1 && u > 0;
    // c2 = c2 && v > 0;
    if(c1 != c2){
      return Color(0.0, 0.0, 0.0, 0.0);
    }
    return Color(1.0, 1.0, 1.0, 0.0);
  }
};
