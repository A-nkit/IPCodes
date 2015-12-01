#include "headers.h"

Mat threshold_output;
vector< vector <Point> > contours(1);
vector<Vec4i> hierarchy;

Mat src; 
Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

int main(){
  VideoCapture cap(0);

  int h1 = 0;
  int h2 = 100;

  int s1 = 21; 
  int s2 = 255;

  int v1 = 24;
  int v2 = 111;

  trackbars(&h1,&h2,&s1,&s2,&v1,&v2);

  while(cap.read(src)){
    cvtColor( src, src_gray, CV_BGR2HSV );
    inRange(src_gray, Scalar(h1,s1,v1), Scalar(h2,s2,v2), threshold_output); 
    imshow("hi", threshold_output);
    
    medianBlur(threshold_output, threshold_output, 15);
    erode(threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) );
    dilate( threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 
    dilate( threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 
    erode(threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) );
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    int j, maxindex, maxarea=0, area;
    Moments moment;
    if(contours.size() > 1){
      for(j=0; j< contours.size(); j++){
        moment = moments((Mat)contours.at(j));
        area= moment.m00;
        if(area>maxarea){
          maxarea = area;
          maxindex = j;
        }
      }
    }
    vector<vector<Point> >hull(1);
    vector<vector<Point> >hull1(1);
    vector<vector<Point> >fhull(1);

    approxPolyDP(contours[maxindex],hull[0], 21, false); 
    convexHull( hull[0], hull1[0], false );        
    if(hull1[0].size()){
      fhull[0].push_back(hull1[0][0]);
      int k;
      double q,w,z;
      for(k=1; k< (hull1[0].size() - 1); k++){
        q = atan((hull1[0][k].y - hull1[0][k-1].y)/(hull1[0][k].x - hull1[0][k-1].x));
        w = atan((hull1[0][k+1].y - hull1[0][k].y)/(hull1[0][k+1].x - hull1[0][k].x));
        z = 3.1414 - q + w;
        if( (z < 3.84) && (z > 2.44)){
          fhull[0].push_back(hull1[0][k]);
        }
        else continue;
      }  
    }
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

    Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    // drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    // drawContours( src, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    drawContours( src, hull, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
    drawContours( drawing, hull, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
    drawContours( src, hull1, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );
    drawContours( drawing, hull1, 0, color, 1, 8, vector<Vec4i>(), 0, Point() );


    /// Show in a window
    namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE);
    imshow( "Hull demo", drawing );
    imshow( "source_window", src );

    int c = waitKey(10);
    if( (char)c == 27 )
      break; 
  }
  return(0);
}