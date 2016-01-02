#include "headers.h"

int main(){
	trackbars();
	Mat frame,yay,mix,_yay;
	VideoCapture cap0(0);
	VideoCapture cap1("http://192.168.43.1:8080/video?.jpg");
	// cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	// cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	while(1){
		cap0.read(frame);
		inRange(frame,Scalar(h1,s1,v1),Scalar(h2,s2,v2),yay);
		imshow("yay",yay);
		cap1.read(mix);
		// mix=imread("/home/ankit/Pictures/snapshot.jpg");
		mix.copyTo(frame,yay);
		imshow("frame",frame);
		imshow("mix",mix);
		char ch=waitKey(10);
		if(ch==27)
			break;
	}
}