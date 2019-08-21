#pragma once

#include "Utils.hpp"

class Color{
  double red, green, blue, special;
public:
  Color();
  Color(double,double,double,double);
  // method function
  double getColorRed(){return red;}
  double getColorGreen(){return green;}
  double getColorBlue(){return blue;}
  double getColorSpecial(){return special;}
  int getRed() { return (int)(red * 255); }
  int getGreen() { return (int)(green * 255); }
  int getBlue() { return (int)(blue * 255); }

  void setColorRed(double value){red = value;}
  void setColorGreen(double value){green = value;}
  void setColorBlue(double value){blue = value;}
  void setColorSpecial(double value){special = value;}

  Color multiply(double val) {
    return Color(red*val, green*val, blue*val, special);
  }

  Color multiply(Color c){
    return Color(red*c.red, green*c.green, blue*c.blue, special);
  }
  Color add(Color b){
    double redcol = red+b.red;
    double greencol = green+b.green;
    double bluecol = blue+b.blue;
    return Color(min(redcol, 1), min(greencol, 1), min(bluecol, 1), 0);
    // return Color(red+c.red, green*c.green, blue*c.blue, special);
  }

  Color screen(Color b){
   double redcol = 1.0 - 2.0* (1.0 - red)*(1.0-b.red);
   double greencol = 1.0 - 2.0* (1.0 - green)*(1.0-b.green);
   double bluecol = 1.0 - 2.0* (1.0 - blue)*(1.0-b.blue);
   return Color(redcol, greencol, bluecol, 0);
  }

  Color overlay(Color b) {

    if(b.red == 0 && b.green == 0 && b.blue == 0){
      return Color(red, green, blue, special);
    }

    double redcol = 2.0 * red * b.red;
    if(red >= 0.5){
      redcol = 1.0 - 2.0* (1.0 - red)*(1.0-b.red);
    }
    double greencol = 2.0 * green * b.green ;
    if(green >= 0.5){
      greencol = 1.0 - 2.0* (1.0 - green)*(1.0-b.green);
    }
    double bluecol = 2.0 * blue * b.blue ;
    if(blue >= 0.5){
      bluecol = 1.0 - 2.0* (1.0 - blue)*(1.0-b.blue);
    }
    return Color(redcol, greencol, bluecol,special);
  }
};

Color::Color(): red(0.5), green(0.5), blue(0.5){}

Color::Color(double x,double y,double z,double i): red(x), green(y), blue(z) ,special(i){}
