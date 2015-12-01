#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

   Mat threshold_output;
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;

Mat src; Mat src_gray;
 int thresh = 100;
 int max_thresh = 255;
 RNG rng(12345);

 /// Function header

/** @function main */
int main(  )
 {
   /// Load source image and convert it to gray
   VideoCapture cap(0);

   int iLowH = 0;
 int iHighH = 100;

  int iLowS = 21; 
 int iHighS = 255;

  int iLowV = 24;
 int iHighV = 111;
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);


   while(cap.read(src))

   /// Convert image to gray and blur it
   {cvtColor( src, src_gray, CV_BGR2HSV );
  inRange(src_gray, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), threshold_output); //Threshold the image
 imshow("hi", threshold_output);
medianBlur(threshold_output, threshold_output, 15);

   erode(threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) );
  dilate( threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 

   //morphological closing (removes small holes from the foreground)
  dilate( threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) ); 
  erode(threshold_output, threshold_output, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)) );
 
   //Create trackbars in "Control" window
  
   /// Find contours
   findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

   /// Find the convex hull object for each contour
   vector<vector<Point> >hull( contours.size() );
   vector<vector<Point> >hull1( contours.size() );
   for( int i = 0; i < contours.size(); i++ )
      { approxPolyDP(contours[i],hull[i] , 21, false); 
        convexHull( hull[i], hull1[i], false ); 
        
      }

   /// Draw contours + hull results
   Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
   for( int i = 0; i< contours.size(); i++ )
      {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        // drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // drawContours( src, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        drawContours( src, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // drawContours( src, hull1, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        // drawContours( drawing, hull1, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      }

   /// Show in a window
   namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
   imshow( "Hull demo", drawing );
imshow( "source_window", src );

   int c = waitKey(10);
        if( (char)c == 27 ) { break; } 
        
}
   return(0);
}