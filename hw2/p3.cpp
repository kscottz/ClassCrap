#include "vision_utilities.h"
#include "vision_methods.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{

  if(argc != 4 )
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
  std::string dbName(argv[2]);
 

  std::vector<SObjectLabel> data;
  data = get_morphology(inImg);
  retVal = write_database(dbName,data);
  if( retVal )
    {
      cout << "Could not write output database" << endl;
      return retVal;
    }
  
  char * oname = argv[3];
  retVal = writeImage(inImg,oname);
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  return 0;
}
