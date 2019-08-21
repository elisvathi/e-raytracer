#pragma once
#include "math.h"
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

  double operator%(Vect other){
    return x * other.x + y * other.y + z * other.z;
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

  Vect refract(Vect incoming,Vect normal, double ior){
    double r = 1 / ior;
    double c = (!normal)%incoming;
    double e = sqrt(1- pow(r, 2)*(1-pow(c,2)));
    return (incoming + (normal * e)) * r;
  }

};


Vect::Vect(){}

Vect::Vect(double i, double j, double k): x(i), y(j), z(k) {}
