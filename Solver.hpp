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
  Vect campos(0, -0.8, -5);
  return new Camera(campos, lookAt);
}

Raytracer *get_scene() {
  Vect O(0, 0, 0);
  Vect X(1, 0, 0);
  Vect Y(0, 1, 0);
  Vect Z(0, 0, 1);
  Vect white_light(1.0, 1.0, 1.0);
  Vect pretty_green(1.0, 0.0, 0.0);
  Vect red_col(1.0, 0.0, 0.0);
  Vect blue_col(0.0, 0.0, 1.0);
  Vect green(0.0, 1.0, 0.0);
  Vect maroon(0.5, 0.25, 0.25);
  Vect gray(0.5, 0.5, 0.5);
  Vect black(0.0, 0.0, 0.0);

  Vect second_sphere_center(0, -1.5, -1.0);
  Vect first_sphere_center(0, -1.0, 0);
  Sphere *second_sphere = new Sphere(second_sphere_center, 0.5, blue_col);
  Sphere *scene_sphere = new Sphere(first_sphere_center, 1, pretty_green);
  Plane *scene_plane = new Plane(Y, -2, white_light);
  Map *cmap = new Map(white_light);
  if (checkerboard) {
    Checkerboard *boardTexture = new Checkerboard();
    cmap->setTexture(boardTexture);
  }
  scene_plane->get_material()->setDiffuse(cmap);
  scene_plane->get_material()->set_reflectionCoeficient(0.8);
  scene_sphere->get_material()->set_reflectionCoeficient(0.8);
  scene_sphere->get_material()->set_refractionIOR(1.2);
  scene_sphere->get_material()->setRefGlossiness(0.8);
  scene_sphere->get_material()->set_refractionCoeficient(1.0);
  Scene *scene = new Scene();
  scene->addObject(scene_plane);
  scene->addObject(scene_sphere);
  scene->addObject(second_sphere);
  if (enableLights) {
    Vect lighPos(-7, 10, -10);
    Light *scene_light = new Light(lighPos, white_light);
    Vect secondLightPos(-10, 10, -10);
    Light *scene_light2 = new Light(secondLightPos, white_light);
    scene->addLight(new PointLight(scene_light));
    scene->addLight(new PointLight(scene_light2));
  }
  return new Raytracer(scene);
}

void savePixel(Raytracer *raytracer, Camera *camera, RGBType *pixels,
               int samples, int width, int height, int x, int y) {
  double aspectRatio = (double)width/(double)height;
  int index = y * width + x;
  vector<Vect> colors;
  if (!antialiasing) {
    samples = 1;
  }
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
    Vect cres = raytracer->color(cam_ray);
    colors.push_back(cres);
  }
  Vect c = averageColors(colors);
  pixels[index].r = c.getVectX();
  pixels[index].g = c.getVectY();
  pixels[index].b = c.getVectZ();
}

void test_render(int width, int height) {
  cout << "rendering ..." << endl;
  Camera *camera = getCamera(width, height);
  Raytracer *raytracer = get_scene();
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
