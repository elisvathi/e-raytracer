#pragma once
#include "Ray.hpp"
#include "Hitable.hpp"
#include "Vect.hpp"

class Material {
public:
  virtual bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) = 0;
};


class Lambertian : public Material{
public:
  Vect albedo;
  Lambertian(Vect a):albedo(a){}
  bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
      Vect target = rec.p + rec.normal + randomUnit();
      scattered  = Ray(rec.p, target-rec.p);
      attentuation = albedo;
      return true;
  }
};

class Metal: public Material{
public:
  Vect albedo;
  double fuzz;
  Metal(Vect a, double f = 0.0):albedo(a){
    if(f < 1){
      fuzz = f;
    }else{
      fuzz =1;
    }
  }

  bool scatter(Ray& r_in, HitRecord& rec, Vect& attentuation, Ray& scattered) {
    Vect reflected = r_in.getRayDirection().reflect(rec.normal);
    scattered = Ray(rec.p, reflected + randomUnit() * fuzz);
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
    return true;
  }

};
