#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

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
  
  if( retVal || inImg == NULL )
    {
      cout << "Bad input image name:" << inname << endl;
      return retVal;
    }
  char * oname = argv[2];
  Image* output = sobel(inImg);
  retVal = writeImage(output,oname);
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  // should clean up
  return 0;
}
