#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

int main(){
	Vec3b pixCol;
	int x=0,y=0,x1=0,x2=0,y1=0,y2=0,d;
	int h=0,s=0,v=0,i=0,h_min=0,h_max=0,s_min=0,s_max=0,v_min=0,v_max=0;
	vector< vector <Point> > contours;
	vector< vector <Point> > contours1;
	vector<Vec4i> hierarchy; 
	Moments moment, moment1, moment2;
	bool first = false;
	bool remem = false;
	bool track = false;
	VideoCapture cam;
	cam.open(CV_CAP_ANY);
	cam.set(CV_CAP_PROP_FRAME_WIDTH,640);
 	cam.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	Mat img1,img2,delImg,hsv,threshed,img3;
	while(1){
		char ch = waitKey(1);
		if(ch == 'q')
			exit(0);
		if(ch == 'o')
			first = true;

		cam >> img1;
		waitKey(5);
		cam >> img2;
		delImg = abs(img2 - img1);

		// imshow("del",delImg);

		cvtColor(img1,hsv,COLOR_BGR2HSV);
		medianBlur(delImg,delImg,21);
		inRange(delImg,Scalar(0,15,15),Scalar(255,255,255),delImg);
		findContours(delImg,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
		int j, maxindex1, maxindex2, maxarea1=0, maxarea2=0, area;
		if(first){
			imshow("del",delImg);
			if(contours.size() > 1){
	            for(int j=0; j< contours.size(); j++)
	            {
	                moment = moments((Mat)contours.at(j));
	                area= moment.m00;
	                if(area>maxarea1)
	                {
	                    maxarea2 = maxarea1;
	                    maxindex2 = maxindex1;
	                    maxarea1 = area;
	                    maxindex1 = j;
	                }
	            }
	            if(maxarea1 && maxarea2){
		            moment1 = moments((Mat)contours.at(maxindex1));
		            x1 = moment1.m10/maxarea1;
		            y1 = moment1.m01/maxarea1;
		            moment2 = moments((Mat)contours.at(maxindex2));
		            x1 = moment2.m10/maxarea2;
		            y1 = moment2.m01/maxarea2;
		            drawContours(img2,contours,maxindex1,Scalar(255,0,0),3);
		            drawContours(img2,contours,maxindex2,Scalar(255,0,0),3);
		        }
	        }
	        maxindex1=0, maxindex2=0, maxarea1=0, maxarea2=0, area=0;
	        waitKey(1);	        
	        first = false;
	        remem = true;
	    }
		x = (x1+x2)/2;
		y = (y1+y2)/2;
		circle(img2, Point(x,y), 2, Scalar(0,255,0), 3);
			
		imshow("ank",img2);
		//cerr<<(int)first<<'\n';
		//cerr<<'\n'<<x<<'\t'<<y;
		if(remem){
			for(i=0;i<5;i++){
				//cvtColor(img,hsv,COLOR_BGR2HSV);
				pixCol = hsv.at<Vec3b>(Point(320,240));
				//cerr<<'\n'<<(int)pixCol[0]<<'\t'<<(int)pixCol[1]<<'\t'<<(int)pixCol[2];
				h+=(int)pixCol[0];
				s+=(int)pixCol[1];
				v+=(int)pixCol[2];
			}
			remem = false;
			track = true;
		}
		if(0){
			h/=5;
		    s/=5;
		    v/=5;
		    h_min = (h>30)?(h-30):0;
		    h_max = (h<225)?(h+30):255;
		    s_min = (s>60)?(s-60):0;
		    s_max = (s<195)?(s+60):255;
		    v_min = (v>50)?(v-50):0;
		    v_max = (v<205)?(v+50):255;

		    cam>>img3;

	        inRange(hsv, Scalar(h_min,s_min,v_min), Scalar(h_max, s_max, v_max), threshed);
	        medianBlur(threshed,threshed,9);

	        findContours(threshed,contours1,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	        if(contours.size() > 1){
	            int j, maxindex, maxarea=0, area;
	            for(j=0; j< contours.size(); j++)
	            {
	                moment = moments((Mat)contours1.at(j));
	                area= moment.m00;
	                if(area>maxarea)
	                {
	                    maxarea = area;
	                    maxindex = j;
	                }
	            }
	            moment = moments((Mat)contours1.at(maxindex));
	            x = moment.m10/maxarea;
	            y = moment.m01/maxarea;
	            drawContours(img1,contours,-1,Scalar(255,0,0),3);
	            circle(img1, Point(x,y), 2, Scalar(0,255,0), 3);
	            cerr <<'\n'<<x<<'\t'<<y;
	            maxindex=0, maxarea=0, area=0;
	        }
	        imshow("Camera", threshed);
	        imshow("img1", img1);	        
		}
	}
	return 0;
}