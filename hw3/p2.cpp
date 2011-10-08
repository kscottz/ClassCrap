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
  int thresh  = atoi(argv[2]); 
  char * outimg = argv[3];
  char * houghimg = argv[4];
  Image * inImg = new Image();
  int retVal = readImage(inImg,inname);
  
  if( retVal || inImg == NULL )
    {
      cout << "Bad input image name:" << inname << endl;
      return retVal;
    }
  threshold(inImg,thresh);
  retVal = writeImage(inImg,outimg);
  Image * accumulator = NULL;
  accumulator = do_hough_line(inImg);
  retVal = writeImage(accumulator,houghimg);

  if( retVal )
    {
      cout << "Error saving file: " << endl;
      return retVal;
    }
  return 0;
}

