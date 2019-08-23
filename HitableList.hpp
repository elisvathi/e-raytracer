#pragma once
#include "Hitable.hpp"
#include "Ray.hpp"

class HitableList : public Hitable {
public:
  bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
    HitRecord tempRecord;
    bool hited = false;
    double closest = t_max;
    vector<Hitable *> hitables = getHitables();
    for (int i = 0; i < (int)(hitables.size()); i++) {
      if (hitables[i]->hit(r, t_min, closest, tempRecord)) {
        hited = true;
        closest = tempRecord.t;
        rec = tempRecord;
      }
    }
    return hited;
  }
protected:
  virtual const vector<Hitable *> getHitables() = 0;
};
