#include "vision_methods.h"
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "limits.h"
#include "float.h"
#include <math.h>
using namespace std;
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
	      for(int k=0;k<4;k++)//find min label if any
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
		  labels[clabel-1].insert(temp.begin(),temp.end());//we make able to do this better
		}
	      else // assign new 
		{
		  cnt++;
		  setPixel(img,i,j,cnt);
		  set<int> newSet;
		  newSet.insert(cnt-1);//again the off by one thing
		  labels.push_back(newSet);
		}
	    }
	}
    }
  /*
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
  */
  vector< set<int> >::iterator outer;
  vector< set<int> >::iterator inner; 
  bool converge = false;
  while(!converge)// while we are making changes
    {
      converge=true;
      for(outer = labels.begin(); outer  < labels.end(); ++outer)//go through the list
	{
	  for( inner = outer+1; inner  < labels.end(); ++inner)//go through the list again
	    {
	      if( outer != inner )
		{
		  if(set_intersection(*outer,*inner))// if two elements have an intersection
		    {
		      converge = false;
		      *outer = set_union(*outer,*inner);// do the union
		      inner = labels.erase(inner);// delete the old one
		    }
		}
	    }
	}
    }
  /*
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
      }*/
  set_label_colors(labels,img,cnt);
  return retVal;
}
/******************************************************************************************/ 
void set_label_colors(vector< set<int> >& labels, Image* img, int init_colors)
{
  int final_colors = (int)labels.size();
  int dc = 255/(final_colors+1);//spread the coloring out nicely
  int * lut = new int[init_colors+1];// build a lut for color mapping
  // build our label -> final color look up table
  lut[0]=0; // oddness with the color map, we index at zero but colors start at 1
  for(int i=0; i<final_colors; i++) //for each final color
    {
      set<int>::iterator iter;
      int color = (i+1)*dc;
      for(iter = labels[i].begin();iter != labels[i].end(); ++iter)//get the initial colors
	{
	  lut[((*iter)+1)]=color;// add it to the lut
	}
    }
  //apply the lut to the image
  int w = getNCols(img);
  int h = getNRows(img);
  for(int i=0;i<h;i++) // i = y
    {
      for(int j=0;j<w;j++) // j = x 
	{
	  int c = getPixel(img,i,j);
	  setPixel(img,i,j,lut[c]);
	}
    }

  if(NULL != lut)//kill the lut
    {
      delete [] lut;
    }

}
/******************************************************************************************/ 
bool set_intersection(set<int>& a, set<int>& b)
{
  bool retVal = false;
  set<int> result;
  insert_iterator< set<int> > iter(result,result.begin());
  set_intersection(a.begin(),a.end(),b.begin(),b.end(),iter);
  if( !result.empty() )
    {
      retVal = true;
    }
  return retVal;
}
/******************************************************************************************/ 
set<int> set_union(set<int>& a, set<int>& b)
{
  set<int> retVal = a;
  retVal.insert(b.begin(),b.end());
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> get_morphology(Image * img)
{
  vector<SObjectLabel> retVal;
  map<int,SMomentData> color_to_obj;
   if(NULL == img )
    return retVal;
  int w = getNCols(img);
  int h = getNRows(img);
  int c = 0;
  std::map<int,SMomentData>::iterator mIter;
  for(int i=0;i<h;i++)// i = y
    {
      for(int j=0;j<w;j++) // j = x 
	{
	  c = getPixel(img,i,j);
	  if( c > 0 )// if we're above the threshold 
	    {
	      mIter =  color_to_obj.find(c); 
	      if( mIter != color_to_obj.end() )// we have a match
		{
		  color_to_obj[c].M00 += 1;
		  color_to_obj[c].M10 += j;
		  color_to_obj[c].M01 += i;
		  color_to_obj[c].M11 += i*j;
		  color_to_obj[c].M02 += i*i;
		  color_to_obj[c].M20 += j*j;
		}
	      else // new color
		{
		  SMomentData data;
		  data.M00 = 1;
		  data.M10 = j;
		  data.M01 = i;
		  data.M11 = i*j;
		  data.M02 = i*i;
		  data.M20 = j*j;
		  color_to_obj[c]=data;
		}
	    }
	}
    }
  for( mIter = color_to_obj.begin();
       mIter != color_to_obj.end();
       ++mIter)
    {
      SObjectLabel newObj;
      char foo[32];
      sprintf(foo,"%i",mIter->first);
      newObj.m_label = string(foo);
      newObj.m_area  = (mIter->second).M00;
      newObj.m_x_pos = (mIter->second).M10/(mIter->second).M00;
      newObj.m_y_pos = (mIter->second).M01/(mIter->second).M00;
      newObj.m_mu00 = static_cast<float>((mIter->second).M00);
      newObj.m_mu01 = 0.00;
      newObj.m_mu10 = 0.00;
      float fx =  ((float)(mIter->second).M10)/((float)(mIter->second).M00);
      float fy =  ((float)(mIter->second).M01)/((float)(mIter->second).M00);
      newObj.m_mu11 = static_cast<float>((mIter->second).M11)-(fx*((float)(mIter->second.M01)));
      newObj.m_mu20 = static_cast<float>((mIter->second).M20)-(fx*((float)(mIter->second.M10)));
      newObj.m_mu02 = static_cast<float>((mIter->second).M02)-(fy*((float)(mIter->second.M01)));
      float m20p=newObj.m_mu20/newObj.m_mu00;// second order moments
      float m02p=newObj.m_mu02/newObj.m_mu00;
      float m11p=newObj.m_mu11/newObj.m_mu00;
      //cout << m20p << " " << m02p << " " << m11p << endl;
      // this is the way derived from class notes
      float a = (float)((mIter->second).M02+(newObj.m_y_pos*newObj.m_y_pos*(mIter->second).M00)-(2*newObj.m_y_pos*(mIter->second).M01));
      float b = (float)(2*((mIter->second).M11-(newObj.m_y_pos*(mIter->second).M10)-(newObj.m_x_pos*(mIter->second).M01)+(newObj.m_x_pos*newObj.m_y_pos*(mIter->second).M00)));
      float c = (float)(((mIter->second).M20)+(newObj.m_x_pos*newObj.m_x_pos*(mIter->second).M00)-(2*newObj.m_x_pos*(mIter->second).M10));
      //cout << a << " " << b << " " << c << endl;
      newObj.m_angle = atan2((double)b,(double)(a-c))/2;
      newObj.m_angle = newObj.m_angle*(180/PI);
      float temp=((2.00*m11p)/(m20p-m02p));
      // first eigen value
      float e1 = ((m20p+m02p)/2.0)+(sqrt((4.0*m11p*m11p)+((m20p-m02p)*(m20p-m02p)))/2.00);
      // second eigen value
      float e2 = ((m20p+m02p)/2.0)-(sqrt((4.0*m11p*m11p)+((m20p-m02p)*(m20p-m02p)))/2.00);
      // roundness is the ratio of the eigen vectors 
      newObj.m_roundness = sqrt(e2)/sqrt(e1);
      // moment	  

      // THis is the wikipedia method using atan2
      //newObj.m_angle = (180.00/PI)*(atan2((2.00*m11p),(m20p-m02p))/2.00); // rotation angle
      //newObj.m_angle = (180.00/PI)*(atan(temp)/2.00);



      newObj.m_moment = (newObj.m_mu20+newObj.m_mu02)/(newObj.m_mu00*newObj.m_mu00);
      setPixel(img,newObj.m_y_pos,newObj.m_x_pos,255);
      //apply_label(img,newObj);
      //cout << "color " << newObj.m_label << "(" << newObj.m_x_pos
      //   << ", " << newObj.m_y_pos <<") " << (newObj.m_angle) << " " << (180/PI)*temp << " "  << newObj.m_roundness  << endl; 
      retVal.push_back(newObj);
    }
  
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> recognize(Image* img, std::vector<SObjectLabel>& db)
{
  vector<SObjectLabel> retVal;
  return retVal;
}
/******************************************************************************************/ 
int write_database(string fname, vector<SObjectLabel>& data)
{
  int retVal = 0;
  ofstream myFile;
  myFile.open(fname.c_str());
  if(myFile.good())
    {
      vector<SObjectLabel>::iterator iter;
      for(iter = data.begin(); iter != data.end(); ++iter)
	{
	  myFile << iter->m_label << " "
		 << iter->m_y_pos << " " 
		 << iter->m_x_pos << " " 
		 << iter->m_moment << " " 
		 << iter->m_angle << " " 
		 << iter->m_roundness << " "
	         << iter->m_area << " " 
		 << endl; 
	}
    }
  else
    {
      retVal = 1;
    }
  myFile.close();
  return retVal;
}
/******************************************************************************************/ 
vector<SObjectLabel> read_database(string fname)
{
  vector<SObjectLabel> retVal;
  ifstream myFile;
  int sz = 1024;
  char* buff = new char[sz];
  myFile.open(fname.c_str());
  if(myFile.is_open())
    {
      int derp;
      while(!myFile.eof())
	{
	  myFile.getline(buff,sz);
	  string sbuf(buff);
	  if( sbuf.length() > 0 )
	    {
	      stringstream ss(sbuf,stringstream::in);
	      SObjectLabel data;
	      ss  >> data.m_label 
		  >> data.m_y_pos 
		  >> data.m_x_pos 
		  >> data.m_moment
		  >> data.m_angle 
		  >> data.m_roundness
		  >> data.m_area;
	      retVal.push_back(data);
	      /* cout << data.m_label << " "
		 << data.m_y_pos << " " 
		 << data.m_x_pos << " " 
		 << data.m_moment << " " 
		 << data.m_angle << " " 
		 << data.m_roundness << " " 
		 << endl;*/ 
	    }
	}
    }
  if( NULL != buff )
    delete [] buff;
  return retVal;
}
/******************************************************************************************/ 
int compare_objects(Image* img,vector<SObjectLabel>& db, vector<SObjectLabel>& found, float threshold)
{
  int retVal = 0;
  vector<SObjectLabel>::iterator dbIter;
  vector<SObjectLabel>::iterator objIter;
  for(objIter = found.begin(); objIter != found.end(); ++objIter)
    {
      float best = FLT_MAX; 
      string best_label = "NoMatch";
      for(dbIter = db.begin(); dbIter != db.end(); ++dbIter )
	{
	  /*cout << objIter->m_moment << " "
	       << objIter->m_area << " " 
	       << dbIter->m_area << " " 
	       << objIter->m_roundness << " " << endl;
	  */
	  float mdiff = fabs(dbIter->m_moment-objIter->m_moment)/dbIter->m_moment;
	  //diff += fabs((float)1.00+(dbIter->m_area-objIter->m_area))/((float)dbIter->m_area);
	  float rdiff = fabs(dbIter->m_roundness-objIter->m_roundness)/dbIter->m_roundness;
	  float diff = (rdiff+mdiff)/2.00;
	  //cout << diff << endl;
	  if(diff < threshold )
	    {
	      // cout << "diff between " << dbIter->m_label << " and " << objIter->m_label << " " << diff << endl; 
	      best = diff;
	      best_label = dbIter->m_label;
	    }
	}
      if( best < threshold )
	{
	  retVal++;
	  apply_label(img,*objIter);
	}
      objIter->m_label=best_label;
    }
  return retVal;
}
/******************************************************************************************/ 
int apply_label(Image* img, SObjectLabel label)
{
  float theta = label.m_angle*(PI/180.00);
  float length = 15.00;
  int dx = static_cast<int>(length*cos(theta));
  int dy = static_cast<int>(length*sin(theta));
  int x = label.m_x_pos;
  int y = label.m_y_pos;
  line(img, y,x, y+dy, x+dx,255); 
  for( int idx=x;idx<x+3;idx++)
    {
      for( int idy=y;idy<y+3;idy++)
	{
	  setPixel(img,idy,idx,255);
	}
    }
  return(0);
}
/******************************************************************************************/ 
