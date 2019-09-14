#pragma once
#include "Vect.hpp"
#include <vector>
#include <cmath>
#include "Perlin.hpp"
using namespace std;

class Texture {
public:
  virtual Vect value(double u, double v, Vect& p) = 0;
};

class ConstantTexture: public Texture {
public:
  Vect color;
  ConstantTexture(){}
  ConstantTexture(Vect c):color(c){}
  virtual Vect value(double u, double v, Vect& p) {
    return color;
  };
};

class CheckerTexture: public Texture{
public:
  CheckerTexture(){}
  CheckerTexture(Texture* t0, Texture* t1): even(t0), odd(t1){}
  virtual Vect value(double u, double v, Vect& p) {
    double sines = sin(step*p.x()) * sin(step*p.y()) * sin(step*p.z());
    if(sines < 0){
      return odd->value(u, v, p);
    }
    return even->value(u, v, p);
  };
  Texture *even;
  Texture *odd;
  double step = 10;
};


class NoiseTexture : public Texture{
public:
  NoiseTexture(){}
  NoiseTexture(double s):scale(s){}
  virtual Vect value(double u, double v, Vect& p) {
    // return Vect(1,1,1)*noise.noise( p* scale);
    return Vect(1,1,1)*0.5*(1-sin(scale * p.z() + 10* noise.turb(p)));
  };
  double scale = 1.0;
  Perlin noise;
};

class ImageTexture: public Texture{
public:
  ImageTexture(){}
  ImageTexture(unsigned char *pixels, int A, int B): data(pixels), nx(A), ny(B){}
  unsigned char* data;
  int nx, ny;
  virtual Vect value(double u, double v, Vect& p) {
    int i = (u )* nx;
    int j = (1-v)*ny - 0.001;
    if(i < 0)i = 0;
    if(j < 0)j = 0;
    if(i > nx-1)i = nx-1;
    if(j > ny -1)j = ny-1;
    int baseIndex =  3 * i + 3 * nx * j;
    double r = int(data[baseIndex]) / 255.0;
    double g = int(data[baseIndex + 1]) / 255.0;
    double b = int(data[baseIndex + 2]) / 255.0;
    return Vect(r, g, b);
  }
};
