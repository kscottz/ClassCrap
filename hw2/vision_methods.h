#ifndef VISION_METHODS_H
#define VISION_METHODS_H
#include "vision_utilities.h"
#include <vector>
#include <set>
#define COLOR_WHITE 255
#define COLOR_BLACK 0 
//namespace vm
//{
struct SMomentData
{
  int M00;//area
  int M10;//sum x*I(x,y)
  int M01;//sum y*I(x,y)
  int M11;//sum x*y*I(x,y)
  int M20;//sum x^2*I(x,y)
  int M02;//sum y^2*I(x,y)
};

  struct SObjectLabel
  {
    int m_label;
    int m_x_pos;
    int m_y_pos;
    int m_area;
    float m_mu00;
    float m_mu10;
    float m_mu01;
    float m_mu11;
    float m_mu20;
    float m_mu02;
    float m_angle;
    float m_roundness;
  };

  int threshold(Image* img, int thresh);
  int connect_components(Image* img);
  bool set_intersection(std::set<int>& a, std::set<int>& b);
  std::set<int> set_union(std::set<int>& a, std::set<int>& b);
  void set_label_colors(std::vector< std::set<int> >& labels, Image* img, int init_colors);
  std::vector<SObjectLabel> get_morphology(Image * img);
  std::vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db);
//}
#endif //VISION_UTILITIES_H
