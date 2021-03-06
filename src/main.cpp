#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/timeb.h>

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

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int getMilliSpan(int nTimeStart){
	int nSpan = getMilliCount() - nTimeStart;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

void *cameraBufferThread(void* threadarg){
	int value;
  while(running){
	value = getMilliCount();
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
    cout << "---" <<getMilliSpan(value) << endl << "---" <<std::endl;
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
	int val;
  std::cout <<"using default config.txt" << std::endl;

  running = true;
  Mat frame,HSV,threshold,tmp2;
  Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
  Mat dilateElement = getStructuringElement( MORPH_RECT,Size(5,5));
  int H_MIN,S_MIN,V_MIN;
  int H_MAX,S_MAX,V_MAX;

  //namedWindow("Frame",1);

  pthread_t imgThread;
  int id;

  while(running){
    switch(state){
      case INIT:
        std::cout << "Loading Data"<< std::endl;
        readVarsFromFile(&H_MIN,&H_MAX,&S_MIN,&S_MAX,&V_MIN,&V_MAX);
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

	  val = getMilliCount();
      if(imgArray[j].data){

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        imgArray[j].copyTo(frame);
        cvtColor(frame, HSV, CV_BGR2HSV);

        inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN),Scalar(H_MAX, S_MAX, V_MAX), tmp2);

        erode(tmp2,threshold,erodeElement);
        dilate(tmp2,threshold,dilateElement);

        findContours( threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        vector<Rect> boundRect(contours.size());
        int j = 0;
        for(int i = 0; i < contours.size(); i ++){
          approxPolyDP(Mat(contours[i]), contours[i], 2, true);
          if(contours[i].size() < 12 && contours[i].size() > 6){
            boundRect[j] = boundingRect( Mat(contours[i]) );
            rectangle(frame,boundRect[j],Scalar(0,0,255));
          }
        }
        tmp2.copyTo(threshold);
        for(int i = 0; i < boundRect.size(); i ++){
          Rect r = boundRect[i];
          if((r.area() < 7000)){
            if (r.area() > 500 ) {
              Rect midSeventyFive = Rect((int) (r.tl().x) + (int) (r.width * .25), (int) (r.tl().y), (int) (r.width * .5),(int) (r.height * .75));
              Mat roi = threshold(midSeventyFive);
              Mat target = threshold(r);
              double count = 0;
              roi = (roi == 0);
              double density = (sum(target)/r.area())[0];
              if(!(density > 75 && density < 250) || (double)r.width / (double)r.height < 1){
                std::cout << "Bad" << endl;
                boundRect[i] = Rect(10,10,10,10);
              }
            }
          }
        }
        for( int i = 0; i< contours.size(); i++ )
        {
          Scalar color = Scalar( 0, 255, 0);
          drawContours( frame, contours, i, color, 2, 8, hierarchy, 0, Point() );
        }
        int maxid = 0;
        for(int i = 0; i < boundRect.size(); i ++){
          if(boundRect[maxid].area() < boundRect[i].area()){
            maxid = i;
          }

        }

        rectangle(frame,boundRect[maxid],Scalar(255,0,0));
        //imshow("Frame",frame);
        waitKey(33);
        cout << getMilliSpan(val) << std::endl;
      }else{
        std::cout <<"No Data" << std::endl;
        waitKey(33);
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
