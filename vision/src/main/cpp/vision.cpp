#include "vision.h"
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <iostream>

#include <cameraserver/CameraServer.h>
#include <cscore.h>

using namespace cv;
using namespace std;

RNG rng(12345);
Rect bounding_rect;
int thresh = 100;
int width_goal;
int height_goal;

//--HumanAquisition
bool TargetFaceAquired;
bool TargetHumanAquired;
int HumanXoffset, HumanYoffset;

//--ObjectAquisition
bool TargetObjectAquired;
int objXoffset, objYoffset;


int ResWidth = 320, ResHeight = 240;

void curtinfrc_vision::run() {
  

  // This creates a webcam on USB, and dumps it into a sink. The sink allows us to access the image with sink.GrabFrame
#ifdef __DESKTOP__
  cs::UsbCamera cam{"USBCam", 0};
#else
  cs::UsbCamera cam{"USBCam", 4};
#endif
  cs::CvSink sink{"USB"};
  sink.SetSource(cam);


  // The camera defaults to a lower resolution, but you can choose any compatible resolution here.
  cam.SetResolution(ResWidth, ResHeight);

  width_goal = ResWidth/2;
  height_goal = ResHeight/2;


  auto video_mode = cam.GetVideoMode();
  std::cout << "Width: " << video_mode.width << " Height: " << video_mode.height << std::endl;
  std::cout << "Goals: " << width_goal << "," << height_goal << endl; 

  // This lets us see the camera output on the robot dashboard. We give it a name, and a width and height.
  cs::CvSource output = frc::CameraServer::GetInstance()->PutVideo("USB Camera", video_mode.width, video_mode.height);

  cv::Mat current_frame{video_mode.height, video_mode.width, CV_8UC3};

  /// Set up the pedestrian detector --> let us take the default one
  HOGDescriptor hog;
  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  /// Set up tracking vector
  vector<Point> track;


  auto inst = nt::NetworkTableInstance::GetDefault();
  auto visionTable = inst.GetTable("VisionTracking");
  UsePedestrianTrackEntry = visionTable->GetEntry("Camera Set");

  UsePedestrianTrack = UsePedestrianTrackEntry.GetBoolean(true);

  if(UsePedestrianTrack){
    while (UsePedestrianTrack) {
      // Grab a frame. If it's not an error (!= 0), convert it to grayscale and send it to the dashboard.
      if (sink.GrabFrame(current_frame) != 0) {

        Mat img = current_frame.clone();
        resize(img,img,Size(img.cols*1, img.rows*1));

        vector<Rect> found;
        vector<double> weights;

        hog.detectMultiScale(img, found, weights);

        /// draw detections and store location
        for( size_t i = 0; i < found.size(); i++ )
        {
          Rect r = found[i];
          rectangle(img, found[i], cv::Scalar(0,0,255), 2);
          stringstream temp;
          temp << weights[i];
          putText(img, temp.str(),Point(found[i].x,found[i].y+50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255));
          track.push_back(Point(found[i].x+found[i].width/2,found[i].y+found[i].height/2));
        }

        /// plot the track so far
        for(size_t i = 1; i < track.size(); i++){
          line(img, track[i-1], track[i], Scalar(255,255,0), 2);
        }

      #ifdef __DESKTOP__
        imshow("VisionTracking", img);
      #else
        output.PutFrame(img);
      #endif
        waitKey(10);

      }
    }
  }
  else {
    while(UsePedestrianTrack = false){
      // Shiny Object Tracking
    }
  }
}
