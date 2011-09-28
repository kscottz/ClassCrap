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
  std::vector<SObjectLabel> db;
  db = read_database(dbName);
  if( db.empty() )
    {
      cout << "Didn't get any objects from database" << endl;
      return retVal;
    }
  
  std::vector<SObjectLabel> data;
  data = get_morphology(inImg);
  float threshold=0.1;
  int count  = compare_objects(inImg,db,data,threshold);
  cout << "Found " << count << " matching objects" << endl; 
  // p22 and data1 - 2 matches
  // p23 and data1 - 2 matches
  // p21 and data3 - 2 matches
  // p22 and data3 - 2 matches
  char * oname = argv[3];
  retVal = writeImage(inImg,oname);
  if( retVal )
    {
      cout << "Error saving file: " << oname << endl;
      return retVal;
    }
  return 0;
}
