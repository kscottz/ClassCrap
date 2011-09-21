#include "vision_methods.h"
using namespace std;
//using namespace vm;
/******************************************************************************************/ 
int threshold(Image* img, int thresh)
{
  int retVal = 0;
  if(NULL == img )
    return -1;
  int w = getNCols(img);
  int h = getNRows(img);
  int c = 0;
  for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
	{
	  int c = getPixel(img,i,j);
	  if( c > thresh )
	    setPixel(img,i,j,COLOR_WHITE);
	  else
	    setPixel(img,i,j,COLOR_BLACK);
	}
    }
 
  return retVal;
}
/******************************************************************************************/ 
int connect_components(Image* img)
{
  int retVal = 0;
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> get_morphology(Image * img)
{
  vector<SObjectLabel> retVal;
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db)
{
  vector<SObjectLabel> retVal;
  return retVal;
}
/******************************************************************************************/ 

