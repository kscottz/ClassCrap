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
  char * inname = argv[1];
  char * accName = argv[2];
  int thresh  = atoi(argv[3]); 
  char * outimg = argv[4];
  Image * inImg = new Image();
  int retVal = readImage(inImg,inname);
  Image * accumulator = new Image();
  retVal = readImage(accumulator,accName);
  threshold(accumulator,thresh);
  hough_reconstruct(accumulator,inImg);
  
  retVal = writeImage(inImg,outimg);
  cleanup(inImg);
  cleanup(accumulator);
  return 0;
}


