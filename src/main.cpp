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

int state =0;
const int INIT = 0;
const int CONNECTING = 1;
const int LOOP = 2;
const int DISCONNECT = 3;

void *cameraBufferThread(void* threadarg){
  while(running){
    if(!cap.read(imgArray[j])){
      state = DISCONNECT;
      break;
    };
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



bool readVarsFromFile(){

}

int main(int, char**){
  running = true;
  Mat frame,HSV;
  int H_MIN,S_MIN,V_MIN;
  int H_MAX,S_MAX,V_MAX;

  namedWindow("Frame",1);
  pthread_t imgThread;
  int id;

  while(running){
    switch(state){
      case INIT:
        std::cout << "Loading Data"<< std::endl;
        readVarsFromFile(); //TODO create function
        std::cout << "Done" << std::endl;
        state = LOOP;
        std::cout <<"Connecting" << std::endl;
      break;
      case CONNECTING:
        if(cap.read(frame)){
          std::cout <<"Done" << std::endl;
          id = pthread_create(&imgThread,NULL,cameraBufferThread,(void *)"");
          pthread_detach(imgThread);
          state = LOOP;
        }
      case LOOP:

        if(imgArray[j].cols > 0){
          imgArray[j].copyTo(frame);
          cvtColor(frame, HSV, CV_BGR2HSV);

          imshow("Frame", HSV);
          waitKey(10);
        }
      break;
      case DISCONNECT:
        cap.release();
        running = false;
      break;
    }

  }

  return 0;

}
