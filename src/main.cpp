#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int, char**){
  VideoCapture cap; // Need to check the address
  cap.open("http://10.30.44.20/mjpg/video.mjpg")
  Mat frame;
  namedWindow("Frame",1);

  while(true){
    cap >> Frame;
    imshow("Frame", frame);
    waitKey(10);
  }


  return 0;

}
