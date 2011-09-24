#include "vision_methods.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include "limits.h"
using namespace std;
//using namespace vm;
/******************************************************************************************/ 
int threshold(Image* img, int thresh)
{
  int retVal = 0;
  if(NULL == img )
    return -1;
  int w = getNCols(img);
  int h = getNRows(img);
  int c = 0;
  for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
	{
	  int c = getPixel(img,i,j);
	  if( c > thresh )
	    setPixel(img,i,j,COLOR_WHITE);
	  else
	    setPixel(img,i,j,COLOR_BLACK);
	}
    }
 
  return retVal;
}
/******************************************************************************************/ 
int connect_components(Image* img)
{
  int retVal = 0;
  if(NULL == img )
    return -1;
  vector< set<int> > labels;
  int w = getNCols(img);
  int h = getNRows(img);
  int cnt =0;
  int c = 0;
  int connect[4];
  connect[0] = -1; //west
  connect[1] = -1; //north
  connect[2] = -1; //n.w.
  connect[3] = -1; //n.e.
  int clabel=-1;
 
  for(int i=0;i<h;i++) // i = y
    {
      for(int j=0;j<w;j++) // j = x 
	{
	  int c = getPixel(img,i,j);
	  if( c > 0 ) // do we have a value
	    {
	      if(i>0) // we can look west
		{
		  connect[0]=getPixel(img,i-1,j); //If we have something copy it to our location
		}
	      if(j > 0)
		{
		  connect[1] = getPixel(img,i,j-1);
		}
	      if( j > 0 && i > 0 )
		{
		  connect[2] = getPixel(img,i-1,j-1);
		}
	      if( j+1 < w && i > 0 )
		{
		  connect[3] = getPixel(img,i-1,j+1);
		}
	      clabel = INT_MAX;
	      set<int> temp;
	      for(int k=0;k<4;k++)
		{
		  if(connect[k] > 0 && connect[k] < clabel)
		    {
		      temp.insert(connect[k]-1);// off by one because we assign colors starting at 1
		      clabel = connect[k];
		    }
		  connect[k] = -1;
		}
	      
	      if( clabel < INT_MAX ) // if we have a value use it
		{
		  setPixel(img,i,j,clabel);
		  set<int>::iterator iter;
		  for(iter = temp.begin(); iter!=temp.end(); ++iter)
		    {
		      labels[*iter].insert(temp.begin(),temp.end());
		    }
		}
	      else // assign new 
		{
		  cnt++;
		  setPixel(img,i,j,cnt);
		  set<int> newSet;
		  newSet.insert(cnt);
		  labels.push_back(newSet);
		}
	    }
	}
    }
  vector< set<int> >::iterator iter;
  set<int>::iterator target;
  int idx = 0;
  cout << "DERP" << labels.size() << endl;
  for(iter = labels.begin(); iter != labels.end();++iter)
    {
      cout << idx << " : ";
      for(target = iter->begin();target!=iter->end();++target)
	{
	  cout << *target << " ";
	}
      cout << endl;
      idx++;
    }
  //    for( target = (labels.begin()++); target !=labels.end();)
  //	{
  //	}
  //  }
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> get_morphology(Image * img)
{
  vector<SObjectLabel> retVal;
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db)
{
  vector<SObjectLabel> retVal;
  return retVal;
}
/******************************************************************************************/ 

