#ifndef VISION_METHODS_H
#define VISION_METHODS_H
#include "vision_utilities.h"
#include <vector>
#include <set>
#include <string>
#define COLOR_WHITE 255
#define COLOR_BLACK 0 

/*********************************************************/
/********************************************************/

struct SVector3D
{
  float x;
  float y;
  float z;
};

struct SMomentData
{
  int M00;//area
  int M10;//sum x*I(x,y)
  int M01;//sum y*I(x,y)
  int M11;//sum x*y*I(x,y)
  int M20;//sum x^2*I(x,y)
  int M02;//sum y^2*I(x,y)
  int minX;
  int maxX;
  int minY;
  int maxY;
};

struct SObjectLabel
{
  
  std::string m_label;
  int m_x_pos;
  int m_y_pos;
  float m_angle;
  float m_roundness;
  float m_moment;
  int m_area;
  float m_mu00;
  float m_mu10;
  float m_mu01;
  float m_mu11;
  float m_mu20;
  float m_mu02;
  int m_minX;
  int m_maxX;
  int m_minY;
  int m_maxY;
};

int threshold(Image* img, int thresh);
int connect_components(Image* img);
bool set_intersection(std::set<int>& a, std::set<int>& b);
std::set<int> set_union(std::set<int>& a, std::set<int>& b);
void set_label_colors(std::vector< std::set<int> >& labels, Image* img, int init_colors);
std::vector<SObjectLabel> get_morphology(Image * img);
std::vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db);
int write_database(std::string fname, std::vector<SObjectLabel>& data);
std::vector<SObjectLabel> read_database(std::string fname);
int compare_objects(Image* img, std::vector<SObjectLabel>& db, std::vector<SObjectLabel>& found, float threshold);
int apply_label(Image* img, SObjectLabel label);
Image* laplace(Image* img);
Image* sobel(Image * img);
Image* convolve(Image* img, int** filter, int sz);
Image* clone(Image* img);
ImageColor* createColor( int w, int h );
Image* do_hough_line(Image * img); 
int draw_hough(Image *im, int x, int y);
int hough_reconstruct(Image* accumulator, Image* output);
Image* logicalAnd(Image* imgA, Image* imgB);
void cleanup(Image* img);
Image * dilate(Image* img);
int read_pfile(char* fname, int& x,int& y, float& r);
SVector3D findLightingVector( Image* img, int x, int y, float r); 
int writeVectors(char* fname, std::vector<SVector3D> vecs);
std::vector<SVector3D> readVectors(char* fname);
Image* createMask(std::vector<Image*> imgs);
int scaleFloat(float a, float max, float min); 
void createAlbedoImage(int width, int height, float max, float min, float * data, Image* img);
ImageColor * createNormalMap(std::vector<Image*> imgs, Image* mask, std::vector<SVector3D> lights, Image* albedoImg);
float constructNormal(std::vector<SVector3D>& lights, int choice[2][3], SVector3D& normal);
int scale(float x); // clamp x=+/-1 to [0,255]



#endif //VISION_UTILITIES_H
