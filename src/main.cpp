#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <pthread.h>
#include <iostream>

using namespace cv;
using namespace std;

bool running = false;
Mat imgArray [3] = {};
long timeArray[3] = {};
int i = 0;
int j = 2;
VideoCapture cap("http://10.30.44.20/mjpg/video.mjpg");

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

void *processingThread(void* threadargs){

}

int main(int, char**){
  running = true;
  Mat frame;

  namedWindow("Frame",1);

  pthread_t imgThread;
  int id;

  id = pthread_create(&imgThread,NULL,cameraBufferThread,(void *)"");
  pthread_detach(imgThread);

  while(true){

    if(imgArray[j].cols > 0){

      imshow("Frame", imgArray[j]);
      waitKey(10);
    }
  }

  return 0;

}
