#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <pthread.h>

using namespace cv;
using namespace std;

bool running = false;
Mat imgArray [3] = {};
long timeArray[3] = {};
int i = 0;
int j = 2;

VideoCapture cap;

void *cameraBufferThread(void* threadarg){

  while(running){

    cap >> imgArray[i];
    i ++;
    j ++;
    if(j > 2){
      j = 0;
    }
    if(i > 2){
      i = 0;
    }
  }
}

int main(int, char**){

  cap.open("http://10.30.44.20/mjpg/video.mjpg");
  Mat frame;
  namedWindow("Frame",1);
  pthread_t imgThread;
  int id;
  id = pthread_create(&imgThread,NULL,cameraBufferThread,(void));

  while(true){
    if(imgArray[j] != NULL){
      imshow("Frame", frame);
      waitKey(10);
    }
  }


  return 0;

}
