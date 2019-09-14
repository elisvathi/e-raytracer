#pragma once
#include "math.h"
#include <random>
#include <vector>
using namespace std;
class Vect{
  double _x, _y, _z;
public:
  Vect();
  Vect(double, double, double);
  // method functions
  double getVectX() { return _x; };
  double getVectY() { return _y; };
  double getVectZ() { return _z; };


  double x(){return _x;}
  double y(){return _y;}
  double z(){return _z;}

  void setX(double value){_x = value;}
  void setY(double value){_y = value;}
  void setZ(double value){_z = value;}

  double r(){return _x;}
  double g(){return _y;}
  double b(){return _z;}

  void setR(double value){_x = value;}
  void setG(double value){_y = value;}
  void setB(double value){_z = value;}


  double magnitude(){
    return sqrt(sumSquares());
  }
  Vect operator+ (Vect other){
    return Vect(x() + other.x(), y() + other.y(), z() + other.z());
  }

  Vect operator- (Vect other){
    return Vect(x() - other.x(), y() - other.y(), z() - other.z());
  }

  Vect operator*(double value){
    return Vect(x()*value, y()*value, z()*value);
  }

  Vect operator/(double value){
    return Vect(x()/value, y()/value, z()/value);
  }
  Vect operator!(){
    return *this * -1;
  }

  Vect operator-(){
    return *this * -1;
  }

  double& operator[](int i) {
    if (i == 0) {
      return _x;
    } else if (i == 1) {
      return _y;
    }
    return _z;
  }
  const double& operator[](int i) const {
    if (i == 0) {
      return _x;
    } else if (i == 1) {
      return _y;
    }
    return _z;
  }

  Vect rotateX(double angle) {
    return Vect(x(), y() * cos(angle) - z() * sin(angle),
                y() * sin(angle) + z() * cos(angle));
  }

  Vect rotateY(double angle) {
    return Vect(x() * cos(angle) + z() * sin(angle), y(),
                -1 * x() * sin(angle) + z() * cos(angle));
  }

  Vect rotateZ(double angle) {
    return Vect(x() * cos(angle) - y() * sin(angle),
                x() * sin(angle) + y() * cos(angle), z());
  }

  Vect rotateByNormal(Vect normal, double angle){
    return Vect(0,0,0);
  }


  double operator%(Vect other){
    return x() * other.x() + y() * other.y() + z() * other.z();
  }

  Vect operator*(Vect other){
    return Vect(x() * other.x() , y() * other.y() , z() * other.z());
  }

  double sumSquares(){
    return x() * x() + y() * y() + z() * z();
  }

  double angleBetween(Vect other){
    return acos((*this % other)/sqrt(sumSquares() * other.sumSquares()));
  }

  Vect operator^(Vect other){
    return Vect(y() * other.z() - z() * other.y(), z() * other.x() - x() * other.z(),
                x() * other.y() - y() * other.x());
  }

  Vect cloneDirection(double value) {
    return *this * (value/magnitude());
  }

  Vect normalize() {
    return cloneDirection(1);
  }

  Vect reflect( Vect norm ){
    return !((norm * ((norm % *this) * 2)) - *this);
  }

  bool refract(Vect normal, double ni_over_nt, Vect& refracted){
    Vect uv = normalize();
    double dt = uv % normal;
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if(discriminant> 0){
      refracted = ((uv - normal * dt) * ni_over_nt) - normal * sqrt(discriminant);
      return true;
    }
    return false;
  }

};


Vect::Vect(){
  setX(0);
  setY(0);
  setZ(0);
}

Vect::Vect(double i, double j, double k){
  setX(i);
  setY(j);
  setZ(k);
}

Vect averageColors(vector<Vect> colors){
  double totalRed = 0;
  double totalGreen = 0;
  double totalBlue = 0;
  int size = colors.size();
  for(int i = 0; i < size; i++){
    totalRed += colors[i].getVectX();
    totalGreen += colors[i].getVectY();
    totalBlue += colors[i].getVectZ();
  }
  return Vect(totalRed/size, totalBlue/size, totalGreen/size);
}

Vect randomUnit(){
  return Vect(0, 1, 0)
    .rotateX(M_PI * 2 * drand48())
    .rotateY(M_PI * 2 * drand48())
    .rotateZ(M_PI * 2 * drand48()).normalize();
}

Vect randomInDisk(){
  Vect p;
  do{
    p = Vect(drand48(), drand48(), 0) * 2 - Vect(1,1,0);
  }while( p % p > 1.0);
  return p;
}
