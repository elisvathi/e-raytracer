#ifndef _CAMERA_H
#define _CAMERA_H
#include "Vect.hpp"

class Camera{
  Vect campos, camdir, camright, camdown;
public:
  Camera();
  Camera(Vect, Vect, Vect, Vect);
  Camera(Vect, Vect);
  // method functions
  Vect getCameraPosition(){return campos;}
  Vect getCameraDirection(){return camdir;}
  Vect getCameraRight(){return camright;}
  Vect getCameraDown(){return camdown;}
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

#endif
