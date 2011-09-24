#ifndef VISION_METHODS_H
#define VISION_METHODS_H
#include "vision_utilities.h"
#include <vector>
#include <set>
#define COLOR_WHITE 255
#define COLOR_BLACK 0 
//namespace vm
//{

  struct SObjectLabel
  {
    int m_label;
    int m_x_pos;
    int m_y_pos;
    float m_moment;
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
