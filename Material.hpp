#pragma once
#include "Ray.hpp"
#include "Hitable.hpp"
#include "Vect.hpp"
#include "Texture.hpp"

class Material {
public:
  virtual bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) = 0;
  virtual Vect emitted(float u, float v, Vect& p){return Vect(0,0,0);}
};


class Lambertian : public Material{
public:
  Texture *albedo;
  Lambertian(Texture* a):albedo(a){}
  Lambertian(Vect a):albedo(new ConstantTexture(a)){}
  bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
      Vect target = rec.p + rec.normal + randomUnit();
      scattered  = Ray(rec.p, target-rec.p);
      scattered.setTime(r_in.getTime());
      attentuation = albedo->value(rec.u, rec.v, rec.p);
      return true;
  }
};

class Metal: public Material{
public:
  Vect albedo;
  double fuzz;
  Metal(Vect a, double f = 0.0): albedo(a){
    if(f < 1){
      fuzz = f;
    }else{
      fuzz =1;
    }
  }

  bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
    Vect reflected = r_in.getRayDirection().reflect(rec.normal);
    scattered = Ray(rec.p, reflected + randomUnit() * fuzz);
    scattered.setTime(r_in.getTime());
    attentuation = albedo;
    return scattered.getRayDirection() % rec.normal > 0;
  }

};

class Dielectric: public Material{
public:
  double ref_idx;
  Dielectric(double ri):ref_idx(ri){}
  bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
    Vect outward_normal;
    Vect reflected = r_in.getRayDirection().reflect(rec.normal);
    double ni_over_nt;
    attentuation = Vect(1.0, 1.0, 1.0);
    Vect refracted;
    double reflect_prob;
    double cosine;
    // check if is going in or out
    if(r_in.getRayDirection() % rec.normal > 0){
      outward_normal = !(rec.normal);
      ni_over_nt = ref_idx;
      cosine = (r_in.getRayDirection() % rec.normal) / r_in.getRayDirection().magnitude();
    }else{
      outward_normal = rec.normal;
      ni_over_nt = 1.0 / ref_idx;
      cosine = (-1 * (r_in.getRayDirection() % rec.normal)) / r_in.getRayDirection().magnitude();
    }
    if(r_in.getRayDirection().refract(outward_normal, ni_over_nt, refracted)){
      reflect_prob = schlick(cosine, ref_idx);
    }else{
      reflect_prob = 1.0;
    }
    if(drand48() < reflect_prob){
      scattered = Ray(rec.p, reflected);
    }else{
      scattered = Ray(rec.p, refracted);
    }
    scattered.setTime(r_in.getTime());
    return true;
  }

};

class DiffuseLight : public Material{
public:
  DiffuseLight(Texture * a): emit(a){}

  virtual bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
    return false;
  }

  virtual Vect emitted(float u, float v, Vect& p){
    return emit->value(u, v, p);
  }

  Texture *emit;
};


class Isotropic: public Material{
public:
  Texture *albedo;
  Isotropic(Texture * a): albedo(a){}
  virtual bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
    scattered = Ray(rec.p, randomUnit());
    attentuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }
};
