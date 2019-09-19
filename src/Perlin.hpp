#pragma once
#include "math.h"
#include "Vect.hpp"

double trilinearInterpolation(Vect c[2][2][2], double u, double v, double w) {
  double uu = u*u*(3-2*u);
  double vv = v*v*(3-2*v);
  double ww = w*w*(3-2*w);
  double accum = 0;
  for (int i=0; i < 2; i++)
    for (int j=0; j < 2; j++)
      for (int k=0; k < 2; k++) {
        Vect weight_v(u-i, v-j, w-k);
        accum += (i*uu + (1-i)*(1-uu))*
          (j*vv + (1-j)*(1-vv))*
          (k*ww + (1-k)*(1-ww))*(c[i][j][k]% weight_v);
      }
  return accum;
}

class Perlin{
public:
  double noise(Vect p){
    double u = p.x() - floor(p.x());
    double v = p.y() - floor(p.y());
    double w = p.z() - floor(p.z());
    int i = floor(p.x());
    int j = floor(p.y());
    int k = floor(p.z());
    Vect c[2][2][2];
    for (int di=0; di < 2; di++)
      for (int dj=0; dj < 2; dj++)
        for (int dk=0; dk < 2; dk++)
          c[di][dj][dk] = randomVects[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
    return trilinearInterpolation(c, u, v, w);
  }

  double turb(Vect p, int depth = 9){
    double accum = 0;
    Vect temp = p;
    double weight = 1.0;
    for(int i = 0; i< depth; i++){
      accum += weight * noise(temp);
      weight *= 0.5;
      temp  = temp * 2;
    }
    return fabs(accum);
  }

  static Vect *randomVects;
  static int *perm_x;
  static int *perm_y;
  static int *perm_z;
};

static Vect* generatePerlin(){
  Vect*p = new Vect[256];
  for(int i = 0; i< 256; i++) {
    p[i] = Vect(-1 + 2 * drand48(), -1 + 2*drand48() , -1 + 2 * drand48()).normalize();
  }
  return p;
}

void permute(int*p, int n){
  for(int i = n-1; i>0; i--){
    int target = int(drand48()* (i+1));
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}

static int* generatePerlinPerm(){
  int* p = new int[256];
  for(int i = 0; i< 256; i++){
    p[i] = i;
  }
  permute(p, 256);
  return p;
}

Vect *Perlin::randomVects = generatePerlin();

int *Perlin::perm_x = generatePerlinPerm();
int *Perlin::perm_y = generatePerlinPerm();
int *Perlin::perm_z = generatePerlinPerm();
