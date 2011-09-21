#ifndef VISION_METHODS_H
#define VISION_METHODS_H
#include "vision_utilities.h"
#include <vector>

struct SObjectLabel
{
  int m_label;
  int m_x_pos;
  int m_y_pos;
  float m_moment;
  float m_angle;
  float m_roundness;
};

int theshold(Image* img, unsigned int threshold);
int connect_components(Image* img);
std::vector<SObjectLabel> get_morphology(Image * img);
std::vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db);
#endif //VISION_UTILITIES_H
