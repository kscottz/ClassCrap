#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char *argv[])
{

  if(argc != 7 )
    {
      cout << "Argument error" << endl;
      return 1;
    }

  vector<Image*> imgs;
  for( int i=1; i<6;i++)
    {
      Image* img = new Image();
      readImage(img,argv[i]);
      imgs.push_back(img);
    }
  Image* result = NULL;
  result = createMask(imgs);
  writeImage(result,argv[6]);
  vector<Image*>::iterator iter;
  for(iter = imgs.begin(); 
      iter != imgs.end();
      ++iter)
    {
      cleanup(*iter);
    }
  cleanup(result);
  return 0;
}


