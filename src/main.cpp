#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat preProcess(Mat img){
  Mat temp;
  Mat threshold;
  cvtColor(img,temp,CV_BGR2HSV);
  inRange(temp,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
  
  return threshold;
}

int main(int, char**){
  VideoCapture cap("http://10.30.44.20/video.cgi?test.mjpg"); // Need to check the address
  Mat frame;
  namedWindow("Frame",1);
  while(true){
    cap >> frame;
    imshow("Frame", frame);
    waitkey(33);

  }
  return 0;

}
