#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{

  //if(argc != 6 )
  // {
  //    cout << "Argument error" << endl;
  //   return 1;
  //  }
  for( int i=0; i < 6; i++)
    cout << argv[i] << endl;

  cout << argv[0] << endl;
  char * orig  = argv[1];
  Image * inImg = new Image();
  int retVal = readImage(inImg,orig);

  char * accfname  = argv[2];
  Image * acc = new Image();
  retVal = readImage(acc,accfname);

  char * edgefname  = argv[3];
  Image * edge  = new Image();
  retVal = readImage(edge,edgefname);

  int thresh = atoi(argv[4]);

  char * output  = argv[5];
  Image* outImg = NULL;
  
  threshold(edge,128);
  threshold(acc,thresh);
  Image * reconstruct = NULL; 
  reconstruct = clone(edge);
  hough_reconstruct(acc,reconstruct);
  outImg = logicalAnd(edge,reconstruct);
  
  retVal = writeImage(outImg,output);
  if( retVal )
    {
      //cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  return 0;
}
