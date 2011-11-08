#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{

  if(argc != 8 )
    {
      cout << "Argument error" << endl;
      return 1;
    }
  char* fname = argv[1];
  int x = 0, y = 0;
  float r = 0.00;
  read_pfile(fname,x,y,r);

  
  vector<SVector3D> results; 
  for( int i=0; i < 5; i++ )
    {
      char* imgName = argv[i+2];
      Image* img = new Image();
      readImage(img,imgName);
      SVector3D best; 
      best = findLightingVector(img,x,y,r);
      cout << "For " << imgName << " got " << best.x << " " << best.y << " " << best.z << endl;
      results.push_back(best);
      cleanup(img);
    }

  char* outfile = argv[7];
  writeVectors(outfile,results);

  return 0;
}

