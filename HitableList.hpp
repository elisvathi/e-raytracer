#pragma once
#include "Hitable.hpp"
#include "Ray.hpp"
#include "BoundingBox.hpp"
#include "BoundingBoxNode.hpp"

class HitableList : public Hitable {
  BoundingBoxNode *tree = nullptr;
  double tMin = 0;
  double tMax = 0;
  bool acceleratorCreated  = false;
public:

  void buildTree(double time0, double time1){
    int n;
    Hitable ** list = getHitables(n);
    tree = new BoundingBoxNode(list, n, time0, time1);
  }

  bool hitAccelerator(Ray &r, double t_min, double t_max,HitRecord &rec){
    if(tree != nullptr){
      return tree->hit(r, t_min, t_max, rec);}
    return hitNoAccelerator(r, t_min, t_max, rec);
  }

  bool hitNoAccelerator(Ray &r, double t_min, double t_max,HitRecord &rec){
    HitRecord tempRecord;
    bool hited = false;
    double closest = t_max;
    int n;
    Hitable** hitables = getHitables(n);
    for (int i = 0; i < n; i++) {
      if (hitables[i]->hit(r, t_min, closest, tempRecord)) {
        hited = true;
        closest = tempRecord.t;
        rec = tempRecord;
      }
    }
    return hited;
  }

  bool hit(Ray &r, double t_min, double t_max, HitRecord &rec) {
    return hitAccelerator(r, t_min, t_max, rec);
  }

  bool bounding_box(double t0, double t1, BoundingBox &box) {
    int n;
    Hitable** list = getHitables(n);
    if (n < 1)
      return false;
    BoundingBox tempBox;
    bool firstHit = list[0]->bounding_box(t0, t1, tempBox);
    if(!firstHit){
      return false;
    }else{
      box = tempBox;
    }
    for(int i = 1; i < n; i++){
      if(list[i]->bounding_box(t0, t1, tempBox)){
        box = surroundingBox(box, tempBox);
      }else{
        return false;
      }
    }
    return true;
  }

protected:
  virtual Hitable** getHitables(int& count) = 0;
};

class SimpleHitableList:public HitableList{
public:
  Hitable** hitables;
  int count;
  SimpleHitableList(){}
  SimpleHitableList(Hitable** l, int c):hitables(l), count(c){}
  virtual Hitable** getHitables(int& cnt) {
    cnt = count;
    return hitables;
  };
};
