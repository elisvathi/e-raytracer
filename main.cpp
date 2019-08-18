#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "Camera.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "Vect.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct RGBType {
  double r;
  double g;
  double b;
};
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

void saveBmp(const char *filename, int width, int height, int dpi,
             RGBType *data) {
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

int thisone;
int main(int argc, char **argv) {
  cout << "rendering ..." << endl;
  int dpi = 72;
  int width = 640;
  int height = 480;
  int n = width * height;
  RGBType *pixels = new RGBType[n];

  double aspectratio = (double)width / (double)height;

  Vect O(0, 0, 0);
  Vect X(1, 0, 0);
  Vect Y(0, 1, 0);
  Vect Z(0, 0, 1);

  Vect lookAt(0, 0, 0);
  Vect campos(3, 1.5, -5);
  Camera camera(campos, lookAt);

  Color white_light(1.0, 1.0, 1.0, 0.0);
  Color pretty_green(0.5, 1.0, 0.5, 0.3);
  Color maroon(0.5, 0.25, 0.25, 0);
  Color gray(0.5, 0.5, 0.5, 0.0);
  Color black(0.0, 0.0, 0.0, 0.0);

  Vect lighPos(-19, 10, -10);
  Light scene_light = Light(lighPos, white_light);

  // Sphere objects
  Sphere scene_sphere(O, 1, pretty_green);
  Plane scene_plane(Y, -2, maroon);

  vector<Object*> scene_objects;

  scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
  scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));

  vector<Light*> scene_lights;
  scene_lights.push_back(&scene_light);

  int border = 5;
  double xamnt, yamnt;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      thisone = y * width + x;
      // start with no anti-aliasing
      if (width > height) {
        // the image is wider than it is tall
        xamnt = ((x + 0.5) / (double)width) * aspectratio -
                ((double)(width - height) / (double)height) / 2;
        yamnt = ((height - y) + 0.5) / (double)height;
      } else if (height > width) {
        // the image is taller than it is wide
        xamnt = (x + 0.5) / width;
        yamnt = (((height - 2) + 0.5) / height) / aspectratio -
                (((height - width) / (double)width) / 2);
      } else {
        // the image is square
        xamnt = (x + 0.5) / width;
        yamnt = ((height - y) + 0.5) / height;
      }

      Vect cam_ray_origin = camera.getCameraPosition();
      Vect cam_ray_direction = (camera.getCameraDirection() +
                               ((camera.getCameraRight() * (xamnt - 0.5)) +
                                (camera.getCameraDown() * (yamnt - 0.5)))).normalize();

      Ray cam_ray (cam_ray_origin, cam_ray_direction);

      vector<double> intersections;

      for (int index = 0; index < scene_objects.size(); index++) {
        double inters = scene_objects[index]->findIntersection(cam_ray);
        intersections.push_back(inters);
      }

      int index_of_winning_object = minPositiveIndex(intersections);
      if(index_of_winning_object >= 0 && intersections[index_of_winning_object] > 0){
        Object* wining = scene_objects[index_of_winning_object];
        Vect intersectionPoint = cam_ray.getPointAtLength(intersections[index_of_winning_object]);
        // Calculate shadows
        Vect light_vect = scene_light.getLightPosition();
        Vect dif = light_vect - intersectionPoint;
        Ray light_ray (intersectionPoint, dif);
        bool hit = false;
        for (int index = 0; index < scene_objects.size(); index++) {
          double inters = scene_objects[index]->findIntersection(light_ray);
          if(inters> 0 && inters < dif.magnitude()){
            hit = true;
            break;
          }
          // intersections.push_back(inters);
        }
        // Calculate shadows
        Color c = wining->getColorAt(intersectionPoint);
        if(!hit){
          pixels[thisone].r = c.getColorRed();
          pixels[thisone].g = c.getColorGreen();
          pixels[thisone].b = c.getColorBlue();}
      }
    }
  }
  saveBmp("scene.bmp", width, height, dpi, pixels);
  return 0;
}
