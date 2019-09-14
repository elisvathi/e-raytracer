#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "Raytracer.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "MovingSphere.hpp"
#include "Plane.hpp"
#include "Hitable.hpp"
#include "Volume.hpp"
#include "Rotate.hpp"
#include "Box.hpp"
#include "HitableList.hpp"
#include "FlipNormals.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "XYRect.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

bool antialiasing = true;
bool multiTrread = false;
int randomSamples = 1;
bool reflections = false;
bool refractions = false;
bool checkerboard = true;
bool enableLights = false;
bool enableAmbient = true;

Hitable* getGlobeSphere(Vect center, double radius){
  int nx, ny, nn;
  unsigned char *tex_data  = stbi_load("earth.jpg", &nx, &ny, &nn, 0);
  Material *mat = new Lambertian(new ImageTexture(tex_data, nx, ny));
  return new Sphere(center, radius, mat);
}

Camera *getCamera(double tmin, double tmax) {
  Vect lookAt(0, 0.5, -1);
  Vect campos(3, 0.0, -2.5);
  Camera* cam =  new Camera(campos, lookAt);
  cam->setTime1(tmin);
  cam->setTime2(tmax);
  return cam;
}
Camera *getCamera2() {
  Vect lookAt(0,0,0);
  Vect campos(13, 2, 3);
  Camera* cam =  new Camera(campos, lookAt);
  return cam;
}

// CORNELL BOX

Raytracer *cornell_box_scene(){
  Hitable **list = new Hitable*[10];
  int i = 0;
  Material *red = new Lambertian(new ConstantTexture(Vect(0.65, 0.05, 0.05)));
  Material *white = new Lambertian(new ConstantTexture(Vect(0.73, 0.73, 0.73)));
  // Material *white_reflective = new Metal(Vect(1.0, 1.0 , 1.0), 0.2);
  Material *green = new Lambertian(new ConstantTexture(Vect(0.12, 0.45, 0.15)));
  Material *light = new DiffuseLight(new ConstantTexture(Vect(15, 15, 15)));
  list[i++] = new YZRect(0, 555, 0, 555, 555, green);
  list[i++] = new YZRect(0, 555, 0, 555, 0, red);
  // list[i++] = new XZRect(213, 343, 227, 332, 554, light);
  list[i++] = new XZRect(0, 555, 0, 555, 555, white);
  list[i++] = new XZRect(0, 555, 0, 555, 0, white);
  list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, red));
  Hitable* b1 = new RotateY(new Box(Vect(130, 0, 65), Vect(295, 165, 230), white), -18);
  list[i++] = new ConstantVolume(b1,0.0004, new ConstantTexture(Vect(1.0, 1.0, 1.0)));
  list[i++] = new RotateY(new Box(Vect(265, 0, 295), Vect(430, 330, 460), white), 15);
  list[i++] = getGlobeSphere(Vect(250, 50, 250), 50);
  list[i++] = getGlobeSphere(Vect(350, 50, 250), 50);
  Scene *scene = new Scene();
  scene->addObjects(list, i);
  scene->buildTree(0,1);
  return new Raytracer(scene, false);
}

Camera *cornell_box_camera(){
  Vect lookAt(278,278,0);
  Vect campos(278, 278, -850);
  Camera *cam = new Camera(campos, lookAt);
  cam->setFov(36.0);
  return cam;
}

// CORNELL BOX


Raytracer *get_scene(double tmin, double tmax) {
  Vect O(0, 0, 0);
  Vect X(1, 0, 0);
  Vect Y(0, 1, 0);
  Vect Z(0, 0, 1);
  Hitable ** objects = new Hitable*[4];
  Texture* checker = new CheckerTexture(new ConstantTexture(Vect(0.2, 0.3, 0.1)), new ConstantTexture(Vect(0.9, 0.9, 0.9)));
  MovingSphere* mv= new MovingSphere(Vect(0,1.0,-1), 0.5, new Lambertian(new ConstantTexture(Vect(0.1, 0.2, 0.5))));
  objects[0] = mv;
  objects[1] = new Sphere(Vect(0,-100.5,-1), 100, new Lambertian(checker));
  objects[2] = new Sphere(Vect(1,0.0,-1.0), 0.5, new Metal(Vect(0.8, 0.6, 0.2)));
  mv->setSpeed(Vect(0.0, -0.2, 0.0));
  objects[3]= new Sphere(Vect(-1,0.0,-1.0), 0.5, new Dielectric(1.9));
  Scene *scene = new Scene();
  scene->addObjects(objects, 4);
  scene->buildTree(tmin, tmax);
  return new Raytracer(scene);
}

Raytracer *two_perlin_spheres(){
  Texture *perl = new NoiseTexture(1);
  Hitable** objects = new Hitable*[2];
  objects[0] = new Sphere(Vect(0, -1000, 0),1000, new Lambertian(perl));
  objects[1] = new Sphere(Vect(0, 2, 0),2, new Lambertian(perl));
  Scene *scene = new Scene();
  scene->addObjects(objects, 2);
  return new Raytracer(scene);
}

Hitable* getLightRectangle(double x0, double x1, double y0, double y1, double k , Vect color) {
  Material * mat = new DiffuseLight(new ConstantTexture(color));
  return new AARect(x0, x1, y0, y1, k, mat);
}

Hitable* getLightSphere(Vect center, double radius, Vect color) {
  Material * mat = new DiffuseLight(new ConstantTexture(color));
  return new Sphere(center, radius, mat);
}

Raytracer *get_globe(){
  Texture *perl = new NoiseTexture(1);
  Hitable** objects = new Hitable*[2];
  objects[0] = new Sphere(Vect(0, -1000, 0),1000, new Lambertian(perl));
  objects[1] = getGlobeSphere(Vect(0, 2, 0),2);
  Scene *scene = new Scene();
  scene->addObjects(objects, 2);
  return new Raytracer(scene);
}

Raytracer *get_globe_night(){
  Texture *perl = new NoiseTexture(1);
  Hitable** objects = new Hitable*[4];
  objects[0] = new Sphere(Vect(0, -1000, 0),1000, new Lambertian(perl));
  objects[1] = getGlobeSphere(Vect(0, 2, 0),2);
  objects[2] = getLightSphere(Vect(0,7,0), 2, Vect(4,4,4));
  objects[3] = getLightRectangle(3, 5, 1, 3, -2, Vect(4,4,4));
  Scene *scene = new Scene();
  scene->addObjects(objects, 4);
  return new Raytracer(scene);
}

Raytracer *get_random_scene(){
  int n = 400;
  Hitable ** list = new Hitable*[n + 1];
  list[0] = new Sphere(Vect(0, -1000, 0), 1000, new Lambertian(Vect(0.5, 0.5, 0.5)));
  int i = 1;
  for(int a = -11; a < 11; a++){
    for(int b = -11; b < 11; b++){
      float choose_mat = drand48();
      Vect center(a + 0.9 * drand48(), 0.2, b+ 0.9*drand48());
      if((center - Vect(4, 0.2, 0)).magnitude() > 0.9 && i < n - 4 && drand48() > 0.5){
        if(choose_mat < 0.8){
          list[i++] = new Sphere(
              center, 0.2,
              new Lambertian(Vect(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48())));
        }else if(choose_mat < 0.95){
          list[i++] = new Sphere(
              center, 0.2,
              new Metal(Vect(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                             0.5 * (1 + drand48())),
                        0.5 * drand48()));
        }else{
          list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
        }
      }
    }
  }
  list[i++] = new Sphere(Vect(0, 1, 0), 1.0, new Dielectric(1.5));
  list[i++] = new Sphere(Vect(-4, 1, 0), 1.0, new Lambertian(Vect(0.4, 0.2, 0.1)));
  list[i++] = new Sphere(Vect(4, 1, 0), 1.0, new Metal(Vect(0.7, 0.6, 0.5)));
  Scene *scene = new Scene();
  scene->addObjects(list, i);
  return new Raytracer(scene);
}

void savePixel(Raytracer *raytracer, Camera *camera, RGBType *pixels,
               int samples, int width, int height, int x, int y) {
  double aspectRatio = (double)width/(double)height;
  int index = y * width + x;
  if (!antialiasing) {
    samples = 1;
  }
  Vect final(0,0,0);
  for(int i = 0;i < samples; i++){
    double dx = x + drand48();
    double dy = y + drand48();
    if(!antialiasing){
      dx = x+ 0.5;
      dy = y+ 0.5;
    }
    double nx = (double)dx / (double)width;
    double ny = (double)dy / (double)height;
    Ray cam_ray = camera->getOriginRay(aspectRatio, nx, ny);
    final = final + raytracer->color(cam_ray, 0);
  }
  final = final * (1.0 / (double)samples);
  final = Vect(sqrt(final.getVectX()), sqrt(final.getVectY()), sqrt(final.getVectZ()));
  pixels[index].r = final.getVectX();
  pixels[index].g = final.getVectY();
  pixels[index].b = final.getVectZ();
  if((x % width) == 0){
    double value  = (((double)x + double(y) * double(width)) / (double(width) * double(height))) * 100 ;
    cout << "Finished " << value << "%" << endl;}
}

void test_render(int width, int height) {
  cout << "rendering ..." << endl;
  double tmin = 0.0;
  double tmax = 1.0;
  // Camera *camera = getCamera(tmin, tmax);
  // Raytracer *raytracer = get_scene(tmin, tmax);
  Camera *camera = cornell_box_camera();
  Raytracer *raytracer = cornell_box_scene();
  int n = width * height;
  RGBType *pixels = new RGBType[n];
  int numberOfThreads = thread::hardware_concurrency();
  if (!multiTrread) {
    numberOfThreads = 1;
  }
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y += numberOfThreads) {
      if (multiTrread) {
        vector<thread> threads;
        for (int j = 0; j < numberOfThreads; j++) {
          threads.push_back(thread(savePixel, raytracer, camera, pixels,
                                   randomSamples, width, height, x, y + j));
        }
        for (ulong i = 0; i < threads.size(); i++) {
          threads[i].join();
        }
      } else {
        savePixel(raytracer, camera, pixels, randomSamples, width, height, x,
                  y);
      }
    }
  }
  saveBmp("scene.bmp", width, height, 72, pixels);
}
