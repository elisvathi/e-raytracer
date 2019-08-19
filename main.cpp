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
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Raytracer.hpp"
#include "Map.hpp"
#include "Texture.hpp"
#include "Vect.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Utils.hpp"
#include <SDL2/SDL.h>

using namespace std;


Raytracer get_scene(){
  Vect O(0, 0, 0);
  Vect X(1, 0, 0);
  Vect Y(0, 1, 0);
  Vect Z(0, 0, 1);
  Color white_light(1.0, 1.0, 1.0, 0.0);
  Color pretty_green(1.0, 0.0, 0.0, 0.3);
  Color red_col(1.0, 0.0, 0.0, 0.3);
  Color blue_col(0.0, 0.0, 1.0, 0.3);
  Color green(0.0, 1.0, 0.0, 0.3);
  Color maroon(0.5, 0.25, 0.25, 0);
  Color gray(0.5, 0.5, 0.5, 0.0);
  Color black(0.0, 0.0, 0.0, 0.0);

  Vect lighPos(-7, 10, -10);
  Light *scene_light = new Light(lighPos, white_light);
  Vect secondLightPos(-10, 10, -10);
  Light *scene_light2 = new Light(secondLightPos, white_light);

  Vect second_sphere_center(0, 0.0, 0);
  Vect first_sphere_center(0, -1.0, 0);
  Sphere *second_sphere = new Sphere(second_sphere_center, 0.5, blue_col);
  Sphere *scene_sphere = new Sphere(first_sphere_center, 1, pretty_green);
  Plane *scene_plane = new Plane(Y, -2, white_light);
  Checkerboard *boardTexture = new Checkerboard();
  Map *cmap = new Map(white_light);
  cmap->setTexture(boardTexture);
  scene_plane->get_material()->setDiffuse(cmap);
  scene_plane->get_material()->set_reflectionCoeficient(1.0);
  scene_sphere->get_material()->set_refractionIOR(1.2);
  scene_sphere->get_material()->set_refractionCoeficient(1.0);
  Scene *scene  = new Scene();
  scene->addObject(scene_plane);
  scene->addObject(scene_sphere);
  scene->addObject(second_sphere);
  scene->addLight(scene_light);
  scene->addLight(scene_light2);
  // scene->addLight(&scene_light3);
  Raytracer raytracer (scene);
  return raytracer;
}

Camera getCamera(int width, int height){
  Vect lookAt(0, 0, 0);
  Vect campos(0, 0, -5);
  Camera camera(campos, lookAt);
  return camera;
}

void test_render(int width, int height){
  cout << "rendering ..." << endl;
  Camera camera = getCamera(width, height);
  Raytracer raytracer = get_scene();
  int dpi = 72;
  int n = width * height;
  RGBType *pixels = new RGBType[n];
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int index = y * width + x;
      Ray cam_ray = camera.getOriginRay(width, height, x, y);
      Color c = raytracer.getColor(cam_ray);
      pixels[index].r = c.getColorRed();
      pixels[index].g = c.getColorGreen();
      pixels[index].b = c.getColorBlue();
    }
  }
  saveBmp("scene.bmp", width, height, dpi, pixels);
}

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool running = true;
SDL_Window* gWindow = NULL;
SDL_Renderer* renderer;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gBuffer = NULL;

bool init(){
  bool success = true;
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
      printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    }
  else
    {
      gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
      if( gWindow == NULL )
        {
          printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
          success = false;
        }
      else
        {
          gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
  return success;
}
void close()
{
  SDL_FreeSurface( gBuffer );
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
  SDL_DestroyWindow( gWindow );
  gWindow = nullptr;
  SDL_Quit();
}

void mainLoop(){
  SDL_Event event;
  while(running){
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
}

int main(int argc, char **argv) {
  if( !init() )
    {
      printf( "Failed to initialize!\n" );
    }
  else
    {
      test_render(SCREEN_WIDTH, SCREEN_HEIGHT);
      mainLoop();
    }
  close();
  return 0;
}
