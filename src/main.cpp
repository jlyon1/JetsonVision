#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

int main(int, char**){
  VideoCapture cap; // Need to check the address
  cap.open("http://10.30.44.20/axis-cgi/mjpeg/video.cgi?asdf.mjpeg");
  Mat frame;
  namedWindow("Frame",1);
  while(true){
    cap >> frame;
    if(frame.width > 0){
      imshow("Frame", frame);
    }

  }
  return 0;

}
