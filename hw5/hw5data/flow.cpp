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

  if(argc != 5 )
    {
      cout << "Argument error" << endl;
      return 1;
    }
  char * inname = argv[1];

  Image * img0 = new Image();
  Image * img1 = new Image();
  Image * output = NULL; 

  readImage(img0,argv[1]);
  readImage(img1,argv[2]);
  int wndw_sz = atoi(argv[3]);
  output = doOpticalFlow(img0,img1,wndw_sz);
  writeImage(output, argv[4]);
  cleanup(img0);
  cleanup(img1);
  cleanup(output);
  return 0;
}
