#pragma once
#include "Vect.hpp"
#include "Texture.hpp"
class Map {
  Vect color = Vect(0,0,0);
  bool activeTexture = true;
  Texture* texture = nullptr;

public:
  Map():color(Vect(0,0,0)){}
  Map(Vect c):color(c){}

  Vect getColor(double u, double v){
    if(isActiveTexture()) {
      return texture->getColor(u,v);
    }
    return color;
  }

  void setColor(Vect c){
    color = c;
  }

  Texture* getTexture(){
    return texture;
  }

  void setTexture(Texture * texture ){
    this->texture = texture;
  }

private:
  bool isActiveTexture(){
    return texture !=nullptr && activeTexture;
  }
};
