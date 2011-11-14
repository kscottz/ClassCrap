#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 5 )
    {
      cout << "Argument error" << endl;
      return 1;
    }

  char* gradFile = argv[1];
  int w = 0;
  int h = 0;
  TGradImg gradient = loadGradient(gradFile,w,h);

  vector<SPoint2D> seeds;
  seeds = loadSeedPoints(argv[2]);

  Image* mask = new Image();
  readImage(mask,argv[3]);

  Image* depth = NULL; 

  depth = calculateDepth(seeds,mask,gradient,w,h); 
  
  writeImage(depth,argv[4]);

  cleanup(mask);

  return 0; 
}
