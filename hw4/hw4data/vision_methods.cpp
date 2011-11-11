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
#include "vvector.h"
using namespace std;
/******************************************************************************************/ 
// do the threshold
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
// build the blob coloring in place - return number of blobs
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
  set_label_colors(labels,img,cnt);
  return retVal;
}
/******************************************************************************************/ 
// this is second pass to color the blobs
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
// this is set intersection for the blob color algorithm
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
// this is set union for the blob coloring algo
set<int> set_union(set<int>& a, set<int>& b)
{
  set<int> retVal = a;
  retVal.insert(b.begin(),b.end());
  return retVal;
}
/******************************************************************************************/ 
// Calculate position, area, moments etc and return a "database"
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
		  if(j<color_to_obj[c].minX)
		    {
		      color_to_obj[c].minX = j;
		    }
		  if(j>color_to_obj[c].maxX)
		    {
		      color_to_obj[c].maxX = j;
		    }
		  if(i<color_to_obj[c].minY)
		    {
		      color_to_obj[c].minY = i;
		    }
		  if(i>color_to_obj[c].maxY)
		    {
		      color_to_obj[c].maxY = i;
		    }
		}
	      else // new color
		{
		  // caclulate the raw moments
		  SMomentData data;
		  data.minX = w;
		  data.maxX = 0;
		  data.minY = h;
		  data.maxY = 0;
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
  // do the calculation
  for( mIter = color_to_obj.begin();
       mIter != color_to_obj.end();
       ++mIter)
    {
      SObjectLabel newObj;
      char foo[32];
      sprintf(foo,"%i",mIter->first);
      newObj.m_minX = (mIter->second).minX;
      newObj.m_minY = (mIter->second).minY;
      newObj.m_maxX = (mIter->second).maxX;
      newObj.m_maxY = (mIter->second).maxY;
      newObj.m_label = string(foo);
      newObj.m_area  = (mIter->second).M00;
      newObj.m_x_pos = (mIter->second).M10/(mIter->second).M00;
      newObj.m_y_pos = (mIter->second).M01/(mIter->second).M00;
      newObj.m_mu00 = static_cast<float>((mIter->second).M00);
      newObj.m_mu01 = 0.00;
      newObj.m_mu10 = 0.00;
      float fx =  ((float)(mIter->second).M10)/((float)(mIter->second).M00);
      float fy =  ((float)(mIter->second).M01)/((float)(mIter->second).M00);
      // central moments
      newObj.m_mu11 = static_cast<float>((mIter->second).M11)-(fx*((float)(mIter->second.M01)));
      newObj.m_mu20 = static_cast<float>((mIter->second).M20)-(fx*((float)(mIter->second.M10)));
      newObj.m_mu02 = static_cast<float>((mIter->second).M02)-(fy*((float)(mIter->second.M01)));
      
      float m20p=newObj.m_mu20/newObj.m_mu00;// second order moments
      float m02p=newObj.m_mu02/newObj.m_mu00;
      float m11p=newObj.m_mu11/newObj.m_mu00;
      //cout << m20p << " " << m02p << " " << m11p << endl;

      // LOOK HERE - THIS IS THE WAY FOR CALCULATING ANGLE IN CLASS
      // IT KINDA SUCKS
      float a = (float)((mIter->second).M02+(newObj.m_y_pos*newObj.m_y_pos*(mIter->second).M00)-(2*newObj.m_y_pos*(mIter->second).M01));
      float b = (float)(2*((mIter->second).M11-(newObj.m_y_pos*(mIter->second).M10)-(newObj.m_x_pos*(mIter->second).M01)+(newObj.m_x_pos*newObj.m_y_pos*(mIter->second).M00)));
      float c = (float)(((mIter->second).M20)+(newObj.m_x_pos*newObj.m_x_pos*(mIter->second).M00)-(2*newObj.m_x_pos*(mIter->second).M10));
      //cout << a << " " << b << " " << c << endl;
      newObj.m_angle = atan2((double)b,(double)(a-c))/2;
      newObj.m_angle = newObj.m_angle*(180/PI);

       // THis is the wikipedia method for calculating angle using atan2
      newObj.m_angle = (180.00/PI)*(atan2((2.00*m11p),(m20p-m02p))/2.00); // rotation angle
      //float temp=((2.00*m11p)/(m20p-m02p));
      //newObj.m_angle = (180.00/PI)*(atan(temp)/2.00);

      // first eigen value
      float e1 = ((m20p+m02p)/2.0)+(sqrt((4.0*m11p*m11p)+((m20p-m02p)*(m20p-m02p)))/2.00);
      // second eigen value
      float e2 = ((m20p+m02p)/2.0)-(sqrt((4.0*m11p*m11p)+((m20p-m02p)*(m20p-m02p)))/2.00);
      // roundness is the ratio of the eigen vectors 
      newObj.m_roundness = sqrt(e2)/sqrt(e1);
      // hu moment	  
      newObj.m_moment = (newObj.m_mu20+newObj.m_mu02)/(newObj.m_mu00*newObj.m_mu00);
      //setPixel(img,newObj.m_y_pos,newObj.m_x_pos,255);
      //apply_label(img,newObj);
      //cout << "color " << newObj.m_label << "(" << newObj.m_x_pos
      //   << ", " << newObj.m_y_pos <<") " << (newObj.m_angle) << " " << (180/PI)*temp << " "  << newObj.m_roundness  << endl; 
      retVal.push_back(newObj);
    }
  
  return retVal;
}
/******************************************************************************************/ 
// read a db file
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
	    }
	}
    }
  if( NULL != buff )
    delete [] buff;
  return retVal;
}
/******************************************************************************************/ 
// compare a db and objects 
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

	  float mdiff = fabs(dbIter->m_moment-objIter->m_moment)/dbIter->m_moment;
	  float rdiff = fabs(dbIter->m_roundness-objIter->m_roundness)/dbIter->m_roundness;
	  float diff = (rdiff+mdiff)/2.00;
	  // take the average difference between the roundness and moment
	  if(diff < threshold )
	    {
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
// draw and object's centroid and angle 
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
Image* laplace(Image* img)
{
  int sz = 3;
  int ** filter = new int*[sz];
  for( int i=0; i < sz; i++)
    {
      filter[i] = new int[sz];
    }
  // row=y col=x
  filter[0][0] = 1;
  filter[0][1] = 4;
  filter[0][2] = 1;
  filter[1][0] = 4;
  filter[1][1] = -20;
  filter[1][2] = 4;
  filter[2][0] = 1;
  filter[2][1] = 4;
  filter[2][2] = 1; 

  return convolve(img, filter, sz); 
}
/******************************************************************************************/ 
Image* sobel(Image * img)
{
  int sz = 3;
  int ** hs = new int*[sz];
  int ** v = new int*[sz];
  for( int i=0; i < sz; i++)
    {
      hs[i] = new int[sz];
      v[i] = new int[sz];
    }
  hs[0][0] = -1;
  hs[0][1] = 0;
  hs[0][2] = 1;
  hs[1][0] = -2;
  hs[1][1] = 0;
  hs[1][2] = 2;
  hs[2][0] = -1;
  hs[2][1] = 0;
  hs[2][2] = 1;

  v[0][0] = 1;
  v[0][1] = 2;
  v[0][2] = 1;
  v[1][0] = 0;
  v[1][1] = 0;
  v[1][2] = 0;
  v[2][0] = -1;
  v[2][1] = -2;
  v[2][2] = -1;
 
  Image* hvals = convolve(img,hs,sz);
  Image* vvals = convolve(img,v,sz);
  Image * retVal = clone(img);
  int w = getNCols(img);
  int h = getNRows(img);
  for(int i=0;i<h;i++)//y
    {
      for(int j=0;j<w;j++)//x
	{
	  int m = getPixel(hvals,i,j);
	  int n = getPixel(vvals,i,j);
	  int c = sqrt((m*m)+(n*n));
	  //cout << m << " " << n << " " << c << endl; 
	  setPixel(retVal,i,j,c); 
	}
    }
  cleanup(hvals);
  cleanup(vvals);
  return retVal;
}
/******************************************************************************************/ 
Image* convolve(Image* img, int** filter, int sz)
{
  if(NULL == img )
    return NULL;
  Image * retVal = clone(img);
  int w = getNCols(img);
  int h = getNRows(img);
  int accumulator = 0;
  int c=0,m=0;
  int step = sz/2; // our padding for the image 
  for(int i=step;i<h-step;i++)//y
    {
      for(int j=step;j<w-step;j++)//x
	{
	  accumulator = 0;
	  for(int p=0; p < sz; p++)
	    {
	      for( int q=0; q < sz; q++)
		{
		  m = filter[p][q];// get the filter value
		  if( m != 0 )// save ourself a few steps for sobel
		    {
		      c = getPixel(img,i+(p-step),j+(q-step));// get the area around the target pixel 
		      accumulator += c*m;
		    }
		}
	    }
	  accumulator = CLAMP(abs(accumulator),0,255);
	  setPixel(retVal,i,j,accumulator);
	}
    }

  return retVal;
}
/******************************************************************************************/ 
Image* clone(Image* img)
{
  int w = getNCols(img);
  int h = getNRows(img);
  Image * retVal = new Image();
  setSize(retVal,h,w);
  setColors(retVal,255);
  for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
	{
	  setPixel(retVal,i,j,COLOR_BLACK);
	}
    }
  return(retVal);
}
/******************************************************************************************/ 
ImageColor* createColor( int w, int h )
{
  ImageColor* retVal = new ImageColor();
  setSizeColor(retVal,w,h);
  setColorsColor(retVal,255);
  for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
	{
	  setPixelColor(retVal,i,j,COLOR_BLACK,COLOR_BLACK,COLOR_BLACK);
	}
    }
  return(retVal);
}
/******************************************************************************************/ 
Image* do_hough_line(Image * img)
{
  int w = getNCols(img);
  int h = getNRows(img);
  int thetaRes = 180;
  int rhoRes = sqrt((w*w)+(h*h));
  Image* retVal = new Image();
  setSize(retVal,rhoRes/*w*/,thetaRes /*h*/);
  setColors(retVal,255);
  // setup the accumulator
  for(int i=0;i<thetaRes;i++)
    {
      for(int j=0;j<rhoRes;j++)
	{
	  setPixel(retVal,i,j,COLOR_BLACK);
	}
    }
  // draw the accumulator
  int c = 0;
  for(int i=0;i<h;i++)//h
    {
      //cout << "tick " << i << endl;
      for(int j=0;j<w;j++)
	{
	  c=getPixel(img,i,j);
	  if( c > 0 )
	    {
	      draw_hough(retVal,i,j);
	    }
	}
    }

  return retVal; 
}
/******************************************************************************************/ 
int draw_hough(Image *im, int x, int y)
{
  int thetas = getNCols(im);//THETA
  int rhos = getNRows(im);//RHO
  float xf = (float)x;
  float yf = (float)y;
  int c = 0;
  int rw = rhos/2;
  int max = 0;
  for(int i=0; i< thetas; i++)
    {
      float theta = ((((float)i)/((float)thetas))*PI)-(PI/2.00);
      float rho = (yf*cos(theta))-(xf*sin(theta)); // shift it to positve space
 
      int t = i;
      int r = (int)rho;
      //cout << "(" << r << " , " << t << ") " << rhos<< " " << thetas << endl;
      if(r < rhos && r >  0 )// get rid of clamp effect in setPixel
	{
	  c=getPixel(im,r,t);
	  if( c > max )
	    max = c;
	  setPixel(im,r,t,c+1);
	}
      
  }
  //cout << "MAX: " << max << endl; 
}
/******************************************************************************************/ 
int hough_reconstruct(Image* accumulator, Image* output)
{
  int retVal(0);
  int w = getNCols(accumulator);//theta
  int h = getNRows(accumulator);//rho

  int imgw = getNCols(output);
  int imgh = getNRows(output);

  int c = 0;
  for(int rho=0;rho<h;rho++)
    {
      for(int theta=0;theta<w;theta++)
	{
	  c=getPixel(accumulator,rho,theta);
	  if( c > 0 )//draw line 
	    {
	      float rhof = (float)(rho);
	      float thetaf = ((((float)theta)/((float)w))*PI)-(PI/2);
	      //cout << "(" << rhof << "," << thetaf << ")" << "(" << w << "," << h << ") "<< endl;
	      float m = tan(thetaf);
	      float b = rhof/cos(thetaf);
	      float y0 = (m*0.00)+b;
	      float y1 = (m*imgw)+b;
	      float x0 = -1.00*rhof/sin(thetaf);
	      float x1  =((((float)h)*cos(thetaf))-rhof)/sin(thetaf);
	      line(output,0,(int)y0,imgw,(int)y1,255);
	    }
	}
    }
  return retVal; 
}
/******************************************************************************************/ 
Image* logicalAnd(Image* imgA, Image* imgB)
{
  Image * retVal;
  retVal = clone(imgA);
  int w = getNCols(imgA);
  int h = getNRows(imgA);
  int a = 0;
  int b = 0;
  for(int i=0;i<h;i++)
    {
      for(int j=0;j<w;j++)
	{
	  a=getPixel(imgA,i,j);
	  b=getPixel(imgB,i,j);
	  int c=(a+b)/2;
	  if( c > 172 )
	    {
	      setPixel(retVal,i,j,COLOR_WHITE);
	    }
	}
    }
  return retVal;
}
/******************************************************************************************/ 
void cleanup(Image* img)
{
  if(NULL != img && NULL != img->data)
    {
      delete [] img->data;
      delete img;
      img = NULL; 
    } 
}
/******************************************************************************************/ 
Image * dilate(Image* img)
{
  int w = getNCols(img);
  int h = getNRows(img);
  Image * retVal = NULL;
  retVal = clone(img);
  for(int i=1;i<h-1;i++)
    {
      for(int j=1;j<w-1;j++)
	{
	  int me = getPixel(img,i,j);
	  int up = getPixel(img,i,j-1);
	  int down = getPixel(img,i,j+1);
	  int left = getPixel(img,i-1,j);
	  int right = getPixel(img,i+1,j);
       
	  if(up>0||down>0||left>0||right>0||me>0)
	    {
	      setPixel(retVal,i,j,255);
	    }
	}
    }
  return retVal;
}
/******************************************************************************************/ 
int read_pfile(char* fname, int& x,int& y, float& r)
{
  int retVal = 0;
  ifstream myFile;
  int sz = 1024;
  char* buff = new char[sz];
  myFile.open(fname);
  if(myFile.is_open())
    {
      int derp;
      myFile.getline(buff,sz);
      string sbuf(buff);
      if( sbuf.length() > 0 )
	{
	  retVal = 1;
	  stringstream ss(sbuf,stringstream::in);
	  SObjectLabel data;
	  ss  >> x 
	      >> y 
	      >> r;
	}
    }
  if( NULL != buff )
    delete [] buff;
  return retVal;
}
/******************************************************************************************/ 
SVector3D findLightingVector( Image* img, int x, int y, float r)
{
  SVector3D retVal;
  retVal.x = 0.00;
  retVal.y = 0.00;
  retVal.z = 0.00;
  int w = getNCols(img);
  int h = getNRows(img);
  int v = 0, vt = 0;
  int vx = 0;
  int vy = 0;
  for(int i=1;i<h-1;i++) //y 
    {
      for(int j=1;j<w-1;j++)//x
	{
	   vt = getPixel(img,i,j);
	   if( vt > v ) 
	     {
	       v = vt;
	       vx = j;
	       vy = i;
	     }
	}
    }
  float fx = 1.00*((float)vx-x);
  float fy = 1.00*((float)vy-y);
  cout << x << " " << y << " " << vx << " " << vy << endl;
  cout << fx << " " << fy << endl;
  retVal.x = fx/r;
  retVal.y = fy/r;
  retVal.z = -1.0 * ( sqrt( (r*r-(fx*fx)-(fy*fy)) )/r);
  return retVal; 
}
/******************************************************************************************/ 
int writeVectors(char* fname, vector<SVector3D> vecs)
{
  int retVal = -1;
  ofstream myFile;
  myFile.open(fname);
  if(myFile.good())
    {
      vector<SVector3D>::iterator iter;
      for(iter = vecs.begin(); iter != vecs.end(); ++iter)
	{
	  retVal++;
	  myFile << iter->x << " "
		 << iter->y << " " 
		 << iter->z << " " 
		 << endl; 
	}
    }
  else
    {
      retVal = -1;
    }
  myFile.close();
  return retVal; 
}
/******************************************************************************************/ 
vector<SVector3D> readVectors(char* fname)
{
  ifstream myFile;
  int sz = 1024;
  char* buff = new char[sz];
  vector<SVector3D> retVal;
  myFile.open(fname);
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
	      SVector3D data;
	      ss  >> data.x 
		  >> data.y 
		  >> data.z;
	      retVal.push_back(data);
	    }
	}
    }
  if( NULL != buff )
    delete [] buff;
  return retVal;

}
/******************************************************************************************/ 
Image* createMask(vector<Image*> imgs)
{
  Image * retVal = NULL;
  if( imgs.size() <= 0 )
    {
      return retVal;
    }
  retVal = clone(imgs[0] );
  int w = getNCols(imgs[0]);
  int h = getNRows(imgs[0]);
  int c = 0;
  vector<Image*>::iterator iter;
  for(int i=0;i<h;i++) //y 
    {
      for(int j=0;j<w;j++)//x
	{
	  bool found = false;
	  for( iter = imgs.begin(); 
	       iter != imgs.end();
	       ++iter) 
	    {
	      Image* foo = (*iter);
	      c = getPixel(foo,i,j);
	      if( c > 0 )
		{
		  found = true;
		  break;
		}
	    }
	  if( found )
	    {
	      setPixel(retVal,i,j,COLOR_WHITE);
	    }
	  else
	    {
	      setPixel(retVal,i,j,COLOR_BLACK);
	    }
	}
    }
  return retVal;
}
/******************************************************************************************/ 
ImageColor * createNormalMap(vector<Image*> imgs, Image* mask, vector<SVector3D> lights)
{
  int w = getNCols(mask);
  int h = getNRows(mask);
  
  ImageColor* retVal = createColor(w,h);
  vector<Image*>::iterator imgIter;
  vector<SVector3D>::iterator lightIter;
  // build the s matrix which is fixed we'll build it from all five sources so it is over
  // determined and we don't have to do the search for the best solution. 
  /* float S[5][3];
  int count = 0;
  for( lightIter = lights.begin();
       lightIter != lights.end();
       ++lightIter )
    {
      S[count][0] = lights->x;
      S[count][1] = lights->y;
      S[count][2] = lights->z;
    }*/
  //now we invert the s matrix 
  int idx[2][3]; // color index pair
  for(int i=0;i<h;i++) //y 
    {
      for(int j=0;j<w;j++)//x
	{
	  int test = getPixel(mask,i,j);
	  if(test > 0)// if we have an object
	    {
	      for( int k = 0;k < 3; k++) // reset our best values array. 
		{
		  idx[0][k] = 0;// color 
		  idx[1][k] = 0;// index
		}

	      int count = 0; // the color index
	      for(imgIter = imgs.begin(); // the three brightest pixels
		  imgIter!= imgs.end();
		  ++imgIter)
		{
		  int intensity = getPixel((*imgIter),i,j);
		  for( int k=0; k < 3; k++)
		    {
		      if(intensity > idx[0][k] )
			{
			  idx[0][k] = intensity;
			  idx[1][k] = count;
			  break;
			}
			
		    }
		  count++;
		}
	      SVector3D normal;
	      float albedo = constructNormal(lights,idx,normal);
	    }// if test
	}
    }
  return retVal;
}
/******************************************************************************************/
float constructNormal(vector<SVector3D>& lights, int choice[2][3], SVector3D& normal)
{
  float I[3];
  I[0] = choice[0][0];
  I[1] = choice[0][1];
  I[2] = choice[0][2];

  float S[3][3];
  S[0][0]= lights[(choice[1][0])].x;
  S[1][0]= lights[(choice[1][0])].y;
  S[2][0]= lights[(choice[1][0])].z;
 
  S[0][1]= lights[(choice[1][1])].x;
  S[1][1]= lights[(choice[1][1])].y;
  S[2][1]= lights[(choice[1][1])].z;
  
  S[0][2]= lights[(choice[1][2])].x;
  S[1][2]= lights[(choice[1][2])].y;
  S[2][2]= lights[(choice[1][2])].z;

  float S_INV[3][3];
  float det = 0.00;
  //  INVERT_3x3(S_INV,det,S);
  INVERT_3X3(S_INV,det,S);
  if( det == 0.00 )
    cout << "FUCK!" << endl; 

  float M[3];
  MAT_DOT_VEC_3X3(M,S_INV,I);
  float length = 0.00;
  VEC_LENGTH(length,M);
  float albedo = length*PI;
  normal.x = M[0]/length;
  normal.y = M[1]/length;
  normal.z = M[2]/length;
  VEC_PRINT(M);


}
/******************************************************************************************/ 
