#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int, char**){
  VideoCapture cap; // Need to check the address

  Mat frame;
  namedWindow("Frame",1);
  frame = imread("./pic1.png");
  imshow("Frame", frame);
  
  waitKey();

  return 0;

}
