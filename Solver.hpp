#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "Raytracer.hpp"
#include "Camera.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Hitable.hpp"
#include "HitableList.hpp"
#include "Material.hpp"

using namespace std;

bool antialiasing = true;
bool multiTrread = true;
int randomSamples = 50;
bool reflections = false;
bool refractions = false;
bool checkerboard = true;
bool enableLights = false;
bool enableAmbient = true;


Camera *getCamera(int width, int height) {
  Vect lookAt(0, 0.0, 0);
  Vect campos(2, 2.0, -3);
  return new Camera(campos, lookAt);
}

Raytracer *get_scene() {
  Vect O(0, 0, 0);
  Vect X(1, 0, 0);
  Vect Y(0, 1, 0);
  Vect Z(0, 0, 1);
  Sphere *s1 = new Sphere(Vect(0,0,-1), 0.5, new Lambertian(Vect(0.1, 0.2, 0.5)));
  Sphere *s2 = new Sphere(Vect(0,-100.5,-1), 100, new Lambertian(Vect(0.8, 0.8, 0.0)));
  Sphere *s3 = new Sphere(Vect(1,0.0,-1.0), 0.5, new Metal(Vect(0.8, 0.6, 0.2)));
  Sphere *s4 = new Sphere(Vect(-1,0.0,-1.0), 0.5, new Dielectric(1.9));
  Scene *scene = new Scene();
  scene->addObject(s1);
  scene->addObject(s2);
  scene->addObject(s3);
  scene->addObject(s4);
  return new Raytracer(scene);
}

Raytracer *get_random_scene(){
  int n = 500;
  Hitable ** list = new Hitable*[n + 1];
  list[0] = new Sphere(Vect(0, -1000, 0), 1000, new Lambertian(Vect(0.5, 0.5, 0.5)));
  int i = 1;
  for(int a = -11; a < 11; a++){
    for(int b = -11; b < 11; b++){
      float choose_mat = drand48();
      Vect center(a + 0.9 * drand48(), 0.2, b+ 0.9*drand48());
      if((center - Vect(4, 0.2, 0)).magnitude() > 0.9){
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
}

void test_render(int width, int height) {
  cout << "rendering ..." << endl;
  Camera *camera = getCamera(width, height);
  Raytracer *raytracer = get_random_scene();
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
