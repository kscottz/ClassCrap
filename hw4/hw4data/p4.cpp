#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
 

  if(argc != 11 )
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
  Image* albedo = clone(imgs[0]);
  vector<SVector3D> normals;
  normalMap = createNormalMap(imgs,mask,lights,albedo,normals);
  vector<Gradient> grads = normals2grads(normals); 
  saveGradients(grads,argv[10]);
  writeImageColor(normalMap, argv[8]);
  writeImage(albedo,argv[9]);


  for(iter = imgs.begin(); 
      iter != imgs.end();
      ++iter)
    {
      cleanup(*iter);
    }
  cleanup(mask);
  cleanup(albedo);


}
