#pragma once
#include <fstream>
#include "math.h"
#include "Vect.hpp"

using namespace std;

struct RGBType {
  double r;
  double g;
  double b;
};

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

double dmin(double a, double b){
  return !(b<a)?a:b;
}

double max(double a, double b){
  return !(b>a)?a:b;
}

double dmax(double a, double b){
  return !(b>a)?a:b;
}

double degreesToRadians(double degrees){
  return (M_PI * degrees) / 180;
}

double radiansToDegrees(double radians) {
  return (180 * radians) / M_PI;
}

double mapValue(double val, double a1, double b1, double a2, double b2) {
  double rap = (val - a1) / (b1 - a1);
  return a2 + rap * (b2 - a2);
}

void saveBmp(const char *filename, int width, int height, int dpi, RGBType *data) {
  FILE *f;
  int k = width * height;
  int s = 4 * k;
  int fileSize = 54 + s;
  double factor = 39.375;
  int m = static_cast<int>(factor);

  int ppm = dpi * m;
  unsigned char bmpfileheader[14] = {'B', 'M', 0, 0,  0, 0, 0,
                                     0,   0,   0, 54, 0, 0, 0};
  unsigned char bmpInfoHeader[40] = {40, 0, 0, 0, 0, 0, 0,  0,
                                     0,  0, 0, 0, 1, 0, 24, 0};
  bmpfileheader[2] = (unsigned char)(fileSize);
  bmpfileheader[3] = (unsigned char)(fileSize >> 8);
  bmpfileheader[4] = (unsigned char)(fileSize >> 16);
  bmpfileheader[5] = (unsigned char)(fileSize >> 24);

  bmpInfoHeader[4] = (unsigned char)(width);
  bmpInfoHeader[5] = (unsigned char)(width >> 8);
  bmpInfoHeader[6] = (unsigned char)(width >> 16);
  bmpInfoHeader[7] = (unsigned char)(width >> 24);

  bmpInfoHeader[8] = (unsigned char)(height);
  bmpInfoHeader[9] = (unsigned char)(height >> 8);
  bmpInfoHeader[10] = (unsigned char)(height >> 16);
  bmpInfoHeader[11] = (unsigned char)(height >> 24);

  bmpInfoHeader[21] = (unsigned char)(s);
  bmpInfoHeader[22] = (unsigned char)(s >> 8);
  bmpInfoHeader[23] = (unsigned char)(s >> 16);
  bmpInfoHeader[24] = (unsigned char)(s >> 24);

  bmpInfoHeader[25] = (unsigned char)(ppm);
  bmpInfoHeader[26] = (unsigned char)(ppm >> 8);
  bmpInfoHeader[27] = (unsigned char)(ppm >> 16);
  bmpInfoHeader[28] = (unsigned char)(ppm >> 24);

  bmpInfoHeader[29] = (unsigned char)(ppm);
  bmpInfoHeader[30] = (unsigned char)(ppm >> 8);
  bmpInfoHeader[31] = (unsigned char)(ppm >> 16);
  bmpInfoHeader[32] = (unsigned char)(ppm >> 24);

  f = fopen(filename, "wb");
  fwrite(bmpfileheader, 1, 14, f);
  fwrite(bmpInfoHeader, 1, 40, f);

  for (int i = 0; i < k; i++) {
    RGBType rgb = data[i];

    double red = (data[i].r) * 255;
    double green = (data[i].g) * 255;
    double blue = (data[i].b) * 255;
    unsigned char color[3] = {
        (unsigned char)(int)floor(blue),
        (unsigned char)(int)floor(green),
        (unsigned char)(int)floor(red),
    };
    fwrite(color, 1, 3, f);
  }
  fclose(f);
};

double schlick(double cosine, double ref_idx){
  double r0 = (1- ref_idx) / (1+ref_idx);
  r0 = r0 * r0;
  return r0 + (1-r0)* pow((1-cosine), 5);
}
