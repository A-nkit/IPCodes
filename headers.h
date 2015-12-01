#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
using namespace cv;
using namespace std;

void trackbars(int *h1,int *h2,int *s1,int *s2,int *v1,int *v2){
	namedWindow("trackWindow",0);
	createTrackbar("H_MIN","trackWindow",h1,255);
	createTrackbar("H_MAX","trackWindow",h2,255);
	createTrackbar("S_MIN","trackWindow",s1,255);
	createTrackbar("S_MAX","trackWindow",s2,255);
	createTrackbar("V_MIN","trackWindow",v1,255);
	createTrackbar("V_MAX","trackWindow",v2,255);
}
