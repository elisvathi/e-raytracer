#pragma once
#include "Vect.hpp"
#include "Ray.hpp"
#include "math.h"
#include "Utils.hpp"


class Camera{
  Vect campos, camdir, camright, camdown;
  double angle = degreesToRadians(75.0);
  double aperture = 0.01;
public:
  Camera();
  Camera(Vect, Vect, Vect, Vect);
  Camera(Vect, Vect);
  // method functions
  Vect getCameraPosition(){return campos;}
  Vect getCameraDirection(){return camdir;}
  Vect getCameraRight(){return camright;}
  Vect getCameraDown(){return camdown;}
  Ray getOriginRay(int width, int height, int x, int y){
    return Ray(campos, getRayDirection(width, height, x, y));
  }

  Ray getOriginRay(double aspectRatio, double x, double y){
    Vect dir = getOriginDirection(aspectRatio, x, y);
    dir = dir +  (randomInDisk() * aperture);
    return Ray(campos, dir);
  }

private:

  Vect getOriginDirection(double aspectRatio, double x, double y){
    double maxLength = 2 * tan(angle/2);
    bool wider = true;
    if(aspectRatio < 1){
      wider = false;
    }
    double dx = wider ? maxLength : maxLength * aspectRatio;
    double dy = wider ? maxLength / aspectRatio : maxLength;
    Vect starting = getCameraPosition() + getCameraDirection().normalize() +
                    !(getCameraRight() * (dx / 2)) +
      !(getCameraDown() * (dy / 2));
    Vect dir = starting + (getCameraRight() * (x * dx)) +
           (getCameraDown() * (dy - y * dy));
    return (dir - campos).normalize();
  }


  Vect getRayDirection(int width, int height, int x, int y){
    double xamnt, yamnt;
    double aspectratio = (double)width / (double)height;
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
    return (camdir + ((camright * (xamnt - 0.5)) + (camdown * (yamnt - 0.5))))
        .normalize();
  }
};

Camera::Camera()
    : campos(Vect()), camdir(Vect(0, 0, 1)), camright(Vect()), camdown(Vect()) {
}

Camera::Camera(Vect p, Vect d, Vect r, Vect dn)
    : campos(p), camdir(d), camright(r), camdown(dn) {}

Camera::Camera(Vect p, Vect lookat) {
  campos = p;
  camdir = !(p - lookat).normalize();
  camright = (Vect(0, 1, 0) ^ camdir).normalize();
  camdown = camright ^ camdir;
}
