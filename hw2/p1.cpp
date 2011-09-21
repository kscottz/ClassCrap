//#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
extern void derp(void);
int main(int argc, char *argv[])
{
  derp();
  for(int i=0; i < argc; i++ )
    {
      cout << argv[i] << endl;
    }
  if(argc < 2 || argc > 2 )
    {
      cout << "Argument error" << endl;
      return 1;
    }
  char * inname = argv[0];

  Image * inImg = NULL;
  int retVal = readImage(inImg,inname);
  if( retVal || inImg == NULL )
    {
      cout << "Bad input image name:" << inname << endl;
      return retVal;
    }

  int thresh  = atoi(argv[1]);
  if( thresh  > 255 || thresh < 0 )
    {
      cout << "Bad threshold value - valid range is 0 to 255" << endl;
      return 1;
    }

  retVal =  threshold(inImg, thresh);
  if( retVal )
    {
      cout << "Failed to threshold image." << endl;
      return retVal; 
    }

  char * oname = argv[2];
  retVal = writeImage(inImg,oname);
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  return 0;
}
