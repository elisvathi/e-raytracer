#pragma once
#include "Map.hpp"
#include "Ray.hpp"

class Material {
  Vect color;
  double reflectionCoeficient = 1.0;
  double ref_glossiness = 1.0;
  double refractionCoeficient = 0;
  double refractionIOR = 1;
  Map* diffuse = nullptr;
  Map* roughness = nullptr;
  Map* bump = nullptr;
  Map* tranlucency = nullptr;
  Map* specular = nullptr;
  Map* displace = nullptr;
  Map* reflection = nullptr;
  Map* fresnelIOR = nullptr;
  Map* reflectionGlossiness = nullptr;
  Map* refraction = nullptr;
  Map* refractionGlossiness = nullptr;
  Map* opacity = nullptr;

public:
  int glossinessSubdivisions = 3;
  Material();
  Material(Vect);

  Vect get_color() {
      return color;
  }

  virtual bool scatter(){
  }

  Vect get_color(double u, double v) {
    if (diffuse == nullptr) {
      return color;
    }
    return diffuse->getColor(u, v);
  };

  Map* getDiffuse(){
    return diffuse;
  }

  void setDiffuse(Map *map){
    diffuse = map;
  }

  void set_color(Vect color) {
    this->color = color;
  }

  double get_refractionCoeficient() {
    return refractionCoeficient;
  }

  void set_refractionCoeficient(double refractionCoeficient) {
    this->refractionCoeficient = refractionCoeficient;
  }

  double get_refractionIOR() {
    return refractionIOR;
  }

  void set_refractionIOR(double refractionIOR) {
    this->refractionIOR = refractionIOR;
  }

  double get_reflectionCoeficient() {
    return reflectionCoeficient;
  }

  void set_reflectionCoeficient(double reflectionCoeficient) {
    this->reflectionCoeficient = reflectionCoeficient;
  }

  double getRefGlossiness(){
    return ref_glossiness;
  }

  void setRefGlossiness(double value){
    ref_glossiness = value;
  }

};
Material::Material(){
  color = Vect(0,0,0);
}
Material::Material(Vect c) : color(c) {}
