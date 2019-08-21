#pragma once
#include "Color.hpp"
#include "Texture.hpp"
class Map {
  Color color = Color(0,0,0,0);
  bool activeTexture = true;
  Texture* texture = nullptr;

public:
  Map():color(Color(0,0,0,0)){}
  Map(Color c):color(c){}

  Color getColor(double u, double v){
    if(isActiveTexture()) {
      return texture->getColor(u,v);
    }
    return color;
  }

  void setColor(Color c){
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
