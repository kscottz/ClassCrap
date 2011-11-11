#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
 

  if(argc != 10 )
    {
      cout << "Argument error" << endl;
      return 1;
    }

  vector<SVector3D> lights;
  lights = readVectors(argv[1]);
  vector<Image*> imgs;
  for( int i=2; i<7;i++)
    {
      Image* img = new Image();
      readImage(img,argv[i]);
      imgs.push_back(img);
    }
  Image* mask = new Image();
  readImage(mask,argv[7]);
  vector<Image*>::iterator iter;
  ImageColor* normalMap = NULL;
  normalMap = createNormalMap(imgs,mask,lights);
  writeImageColor(normalMap, argv[8]);
  Image* albedo = new Image();
  readImage(albedo,argv[9]);



  for(iter = imgs.begin(); 
      iter != imgs.end();
      ++iter)
    {
      cleanup(*iter);
    }
  cleanup(mask);
  cleanup(albedo);


}
