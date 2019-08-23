#pragma once
#include "math.h"
#include <random>
#include <vector>
using namespace std;
class Vect{
  double x, y, z;
public:
  Vect();
  Vect(double, double, double);
  // method functions
  double getVectX() { return x; };
  double getVectY() { return y; };
  double getVectZ() { return z; };

  double magnitude(){
    return sqrt( x * x + y * y + z * z);
  }
  Vect operator+ (Vect other){
    return Vect(x + other.x, y + other.y, z + other.z);
  }

  Vect operator- (Vect other){
    return Vect(x - other.x, y - other.y, z - other.z);
  }

  Vect operator*(double value){
    return Vect(x*value, y*value, z*value);
  }

  Vect operator!(){
    return *this * -1;
  }

  Vect operator-(){
    return *this * -1;
  }

  Vect rotateX(double angle) {
    return Vect(x, y * cos(angle) - z * sin(angle),
                y * sin(angle) + z * cos(angle));
  }

  Vect rotateY(double angle) {
    return Vect(x * cos(angle) + z * sin(angle), y,
                -1 * x * sin(angle) + z * cos(angle));
  }

  Vect rotateZ(double angle) {
    return Vect(x * cos(angle) - y * sin(angle),
                x * sin(angle) + y * cos(angle), z);
  }

  Vect rotateByNormal(Vect normal, double angle){
    return Vect(0,0,0);
  }


  double operator%(Vect other){
    return x * other.x + y * other.y + z * other.z;
  }

  Vect operator*(Vect other){
    return Vect(x * other.x , y * other.y , z * other.z);
  }

  double sumSquares(){
    return x * x + y * y + z * z;
  }

  double angleBetween(Vect other){
    return acos((*this%other)/sqrt(sumSquares() * other.sumSquares()));
  }

  Vect operator^(Vect other){
    return Vect(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
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


Vect::Vect(){}

Vect::Vect(double i, double j, double k): x(i), y(j), z(k) {}

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
