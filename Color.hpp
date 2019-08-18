#ifndef _COLOR_H
#define _COLOR_H
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

  void setColorRed(double value){red = value;}
  void setColorGreen(double value){green = value;}
  void setColorBlue(double value){blue = value;}
  void setColorSpecial(double value){special = value;}
};

Color::Color(): red(0.5), green(0.5), blue(0.5){}

Color::Color(double x,double y,double z,double i): red(x), green(y), blue(z) ,special(i){}
#endif
