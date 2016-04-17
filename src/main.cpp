#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

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
