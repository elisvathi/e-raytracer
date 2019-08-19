#ifndef _UTILS_H
#define _UTILS_H

#include <fstream>

struct RGBType {
  double r;
  double g;
  double b;
};

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

#endif