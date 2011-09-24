#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{

  for(int i=0; i < argc; i++ )
    {
      cout << argv[i] << endl;
    }
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

  std::vector<SObjectLabel> data;
  data = get_morphology(inImg);

  char * oname = argv[2];
  retVal = writeImage(inImg,oname);
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  return 0;
}
