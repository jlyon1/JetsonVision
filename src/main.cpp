#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <pthread.h>
#include <iostream>

using namespace cv;
using namespace std;

bool running = true;
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

int state =0;
const int INIT = 0;
const int CONNECTING = 1;
const int LOOP = 2;

bool readVarsFromFile(){

}

int main(int, char**){
  running = true;
  Mat frame;
  int H_MIN,S_MIN,V_MIN;
  int H_MAX,S_MAX,V_MAX;

  namedWindow("Frame",1);
  pthread_t imgThread;
  int id;

  while(running){
    switch(state){
      case INIT:
        id = pthread_create(&imgThread,NULL,cameraBufferThread,(void *)"");
        pthread_detach(imgThread);
        readVarsFromFile(); //TODO create function
        state = LOOP;
      break;
      case LOOP:
        if(imgArray[j].cols > 0){
          imshow("Frame", imgArray[j]);
          waitKey(10);
        }
      break;
    }

  }

  while(true){

  }

  return 0;

}
