#pragma once
#include <vector>
#include "BoundingBox.hpp"
#include "Hitable.hpp"
#include <iostream>
#include "stdlib.h"
using namespace std;

int boxCompare(const void* a, const void * b, int axis){
  BoundingBox first, second;
  Hitable *hitableA = *(Hitable**)a;
  Hitable *hitableB = *(Hitable**)b;
  if(!hitableA->bounding_box(0,0,first) || !hitableB->bounding_box(0, 0, second))
    cerr << "No bounding box";
  if(first.min()[axis] - second.min()[axis] < 0){
    return -1;
  }else{
    return 1;
  }
  return 0;
}

int boxCompareX(const void* a, const void *b){
  return boxCompare(a, b, 0);
}

int boxCompareY(const void* a, const void *b){
  return boxCompare(a, b, 1);
}

int boxCompareZ(const void* a, const void *b){
  return boxCompare(a, b, 2);
}

class BoundingBoxNode: public Hitable{
public:
  BoundingBox box;
  Hitable *left = nullptr;
  Hitable *right = nullptr;

  BoundingBoxNode(){

  }

  BoundingBoxNode(Hitable **list, int n, double time0, double time1) {
    // Select random axis
    int axis = int(3 * drand48());
    if (axis == 0) {
      // sort by x axis
      qsort(list,n, sizeof(Hitable*), boxCompareX);
    } else if (axis == 1) {
      // sort by y axis
      qsort(list,n, sizeof(Hitable*), boxCompareY);
    } else {
      // sort by z axis
      qsort(list,n, sizeof(Hitable*), boxCompareZ);
    }
    // Recursion ending conditions
    if (n == 1) {
      left = right = list[0];
    } else if (n == 2) {
      left = list[0];
      right = list[1];
    } else {
      // general conditions
      int halfn = n/2;
      left = new BoundingBoxNode(list, halfn, time0, time1);
      right = new BoundingBoxNode(list + halfn, n - halfn, time0, time1);
    }
    BoundingBox boxLeft, boxRight;
    if(!left->bounding_box(time0, time1, boxLeft) || !right->bounding_box(time0, time1, boxRight)){
      cerr << "No bounding box in the constructor";
    }
    box = surroundingBox(boxLeft, boxRight);
  }

  bool hit(Ray &r, double tmin, double tmax, HitRecord &rec) {
    if (box.hit(r, tmin, tmax)) {
      HitRecord leftRec, rightRec;
      bool hitLeft = left->hit(r, tmin, tmax, leftRec);
      bool hitRight = right->hit(r, tmin, tmax, rightRec);
      if (hitLeft && hitRight) {
        if (leftRec.t < rightRec.t) {
          rec = leftRec;
        } else {
          rec = rightRec;
        }
        return true;
      } else if (hitLeft) {
        rec = leftRec;
        return true;
      } else if (hitRight) {
        rec = rightRec;
        return true;
      } else {
        return false;
      }
    }
    return false;
  }

  bool bounding_box(double t0, double t1, BoundingBox &box) {
    box = this->box;
    return true;
  }
};
