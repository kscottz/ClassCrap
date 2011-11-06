#include "vision_utilities.h"
#include "vision_methods.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


int main(int argc, char *argv[])
{

  if(argc != 3 )
    {
      cout << "Argument error" << endl;
      return 1;
    }
  char * inname = argv[1];

  Image * inImg = new Image();
  int retVal = readImage(inImg,inname);
  // threshold 
  threshold(inImg,5);
  // do a hacked connected components that spits out the result. 
  int count = connect_components(inImg);
  vector<SObjectLabel> labels; 
  labels = get_morphology(inImg);
  vector<SObjectLabel>::iterator iter;
  for( iter = labels.begin(); iter != labels.end(); ++iter)
    {
      cout << iter->m_maxX << " " << iter->m_minX << " "
	   << iter->m_maxY << " " << iter->m_minY << endl;

      float a = (float)iter->m_area;
      cout << (iter->m_maxX - iter->m_minX)/2.00 << " " << (iter->m_maxY - iter->m_minY)/2.00 << endl;
      cout << sqrt((a/3.1415962));
    }


  char * oname = argv[2];
  ofstream myFile;
  myFile.open(oname);
  if(myFile.good())
    {
      //vector<SObjectLabel>::iterator iter;
      for(iter = labels.begin(); iter != labels.end(); ++iter)
	{
	  float a = (float)iter->m_area;
	  myFile << iter->m_y_pos << " " 
		 << iter->m_x_pos << " " 
	         << sqrt(a/3.1415962)
		 << endl; 
	}
    }
  myFile.close();



  retVal = writeImage(inImg,"theresult.pgm");
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  cleanup(inImg);
  // should clean up
  return 0;
}
