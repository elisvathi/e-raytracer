#pragma once
#include "Ray.hpp"
#include "Utils.hpp"
#include "Vect.hpp"
#include "math.h"

class Camera {
  Vect campos, camdir, camright, camdown;
  double angle = degreesToRadians(55.0);
  double aperture = 0.00;
  double time1, time2;
  double focusDist = 1;

public:
  Camera();
  Camera(Vect, Vect, Vect, Vect);
  Camera(Vect, Vect, Vect);
  // method functions
  Vect getCameraPosition() { return campos; }
  Vect getCameraDirection() { return camdir; }
  Vect getCameraRight() { return camright; }
  Vect getCameraDown() { return camdown; }

  double getAperture() { return aperture * 2.0; }
  void setAperture(double value) { aperture = value / 2.0; }

  double getFov() { return angle; }
  void setFov(double value) { angle = value; }

  double getFocusDist(){return focusDist;}
  void setFocusDist(double value){focusDist = value;}

  double getTime1(){return time1;}
  void setTime1(double value){time1 = value;}
  void setTime2(double value){time2 = value;}

  double getTime2(){return time2;}

  Ray getOriginRay(double aspectRatio, double x, double y) {
    Vect randDir = randomInDisk()* aperture;
    Vect offset = (camright * randDir.x())  + (camdown * randDir.y());
    Vect dir = getOriginDirection(aspectRatio, x, y, offset);
    Ray r (campos + offset, dir);
    r.setTime(drand48() * (time2 - time1) + time1);
    return r;
  }

private:

  double screenWidth(double aspectRatio){
    double theta = angle * M_PI / 180;
    double maxHeight = 2 * tan(theta / 2);
    return maxHeight * aspectRatio;
  }

  double screenHeight() {
    double theta = angle * M_PI / 180;
    return 2 * tan(theta / 2);
  }

  Vect upperRight(double aspectRatio, double& dx, double& dy){
    dx = screenWidth(aspectRatio);
    dy = screenHeight();
    return campos + ( camdir.normalize() * focusDist ) - (camright * (dx / 2) * focusDist) -
        (camdown * (dy / 2) * focusDist);
  }

  Vect getOriginDirection(double aspectRatio, double u, double v, Vect offset = Vect(0,0,0)) {
    double dx, dy;
    Vect u_Right = upperRight(aspectRatio, dx, dy);
    Vect dir = u_Right + (camright * (u * dx)) + (camdown * (v * dy));
    return (dir - campos - offset).normalize();
  }
};

Camera::Camera()
    : campos(Vect()), camdir(Vect(0, 0, 1)), camright(Vect()), camdown(Vect()) {
}

Camera::Camera(Vect p, Vect d, Vect r, Vect dn)
    : campos(p), camdir(d), camright(r), camdown(dn) {}

Camera::Camera(Vect p, Vect lookat, Vect up = Vect(0,1,0)) {
  campos = p;
  // camdir = !(p - lookat).normalize();
  camdir = (lookat - p).normalize();
  camright = (up ^ camdir).normalize();
  camdown = camdir ^ camright;
}
