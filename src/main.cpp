#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>

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

bool readVarsFromFile(int* H_MIN, int* H_MAX, int* V_MIN, int* V_MAX, int* S_MIN, int* S_MAX){
  std::ifstream config;
  config.open("config.txt");
  std::string line;
  if(config.is_open()){
    int count = 0;
    while(std::getline(config,line)){
      switch(count){
        case 0:
          (*H_MIN) = atoi(line.c_str());
          break;
        case 1:
          (*H_MAX) = atoi(line.c_str());
          break;
        case 2:
          (*S_MIN) = atoi(line.c_str());
          break;
        case 3:
          (*S_MAX) = atoi(line.c_str());
          break;
        case 4:
          (*V_MIN) = atoi(line.c_str());
          break;
        case 5:
          (*V_MAX) = atoi(line.c_str());
          break;
      }
      count += 1;
    }
  }
  config.close();
  return true;
}

int main(int argc, char* argv[]){

  std::cout <<"using default config.txt" << std::endl;

  running = true;
  Mat frame,HSV,threshold;
  Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
  Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));
  int H_MIN,S_MIN,V_MIN;
  int H_MAX,S_MAX,V_MAX;

  namedWindow("Frame",1);
  pthread_t imgThread;
  int id;

  while(running){
    switch(state){
      case INIT:
        std::cout << "Loading Data"<< std::endl;
        readVarsFromFile(&H_MIN,&H_MAX,&S_MIN,&S_MAX,&V_MIN,&V_MAX); //TODO create function
        std::cout << "Done" << std::endl;
        state = CONNECTING;
        std::cout <<"Connecting" << std::endl;
      break;
      case CONNECTING:
        if(cap.grab()){
          std::cout <<"Done" << std::endl;
          id = pthread_create(&imgThread,NULL,cameraBufferThread,(void *)"");
          pthread_detach(imgThread);
          state = LOOP;
        }
      case LOOP:
        if(imgArray[j].cols > 0){
          vector<vector<Point> > contours;
          vector<Vec4i> hierarchy;

          imgArray[j].copyTo(frame);
          cvtColor(frame, HSV, CV_BGR2HSV);

          inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN),Scalar(H_MAX, S_MAX, V_MAX), threshold);
          erode(threshold,threshold,erodeElement);
          dilate(threshold,threshold,dilateElement);

          findContours( threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

          for( int i = 0; i< contours.size(); i++ )
          {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
          }

          imshow("Frame", frame);
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
