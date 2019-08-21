#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <unistd.h>

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>


using namespace std;
using namespace cv;

class ball_track {

  public:     
    ball_track(int const camera, vector<int> low_HSV, vector<int> high_HSV, int erode_it, int dilate_it) : cap_(VideoCapture(camera)), low_HSV_(low_HSV),
    high_HSV_(high_HSV), erode_it_(erode_it), dilate_it_(dilate_it)
  {
    printf("VideoCapture cap_ is opened: %d\n", (int) cap_.isOpened());
    
    // setup camera capture
    cap_.set(CV_CAP_PROP_FPS, 10);
    cap_.set(CV_CAP_PROP_FRAME_WIDTH, 384);
    cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 216);
    

    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    cvCreateTrackbar("LowH", "Control", &low_HSV_[0], 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &high_HSV_[0], 179);

    cvCreateTrackbar("LowS", "Control", &low_HSV_[1], 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &high_HSV_[1], 255);

    cvCreateTrackbar("LowV", "Control", &low_HSV_[2], 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &high_HSV_[2], 255); 

    cvCreateTrackbar("ErodeIterations", "Control", &erode_it_, 5);
    cvCreateTrackbar("DilateIterations", "Control", &dilate_it_, 5);
    namedWindow("Display", CV_WINDOW_AUTOSIZE);
    namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
  }

    void track_ball_video() {
      
      
      Mat src_image;
      cap_.read(src_image);      

      int width = src_image.size().width;
      int height = src_image.size().height;
      Mat imgLines; 

      // setup ROI
      int center_x = width / 2;
      int center_y = height / 2;
      int roi_height = (int) (0.3 * height) / 2;
      int roi_width = (int) (0.3 * width) / 2;
      
      printf("roi width %d height %d\n", roi_width, roi_height);
      
      printf("make roi \n");
      
      printf("center %d %d dims %d %d\n", center_x, center_y, roi_height, roi_width);
      Rect roi = Rect(center_x, center_y, roi_width, roi_height);
      Rect def_roi = roi;
      
      Mat ref = imread("reference.png", CV_LOAD_IMAGE_COLOR);
      if (!ref.data) {
        printf("whoops\n");
      }
      cvtColor(ref, ref, COLOR_BGR2HSV);
      

      // calculate histogram of image
      int hbins = 15;
      int sbins = 15;
      int hist_size[2] = {hbins, sbins};

      float hranges[2] = {0, 180};
      float sranges[2] = {20, 255};
      float const *ranges[2] = {hranges, sranges};
      
      MatND roi_hist;
      int const channels[2] = {0, 1};
      calcHist(&ref, 1, channels, Mat(), roi_hist, 2, hist_size, ranges, true, false);
      normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
      printf("made hist");
      printf("track now\n");
      int last_x = -1;

      // setup termination criteria for camshift
      TermCriteria term_criteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 1);
      
      printf("enter loop\n");
      RotatedRect track_window; 
      MatND dsv;
      Mat hsv;
      Mat img_orig;
      Point2f rect_points[4];
      Mat mask;
      int x_center = 0;
      while (true) {

        // capture image
        imgLines = Mat::zeros(src_image.size(), CV_8UC3);
        if (!cap_.read(img_orig)) {
          printf("can't read from camera\n");
          break;
        }

        // convert to HSV space and generate backprojection
        Mat mask_conv = generate_mask(img_orig);
        cvtColor(img_orig, hsv, COLOR_BGR2HSV);
        calcBackProject(&hsv, 1, channels, roi_hist, dsv, ranges, 1, true);
        imshow("Display", hsv);
        // get new window
        //
        // If we lost the window, reset to scanning center of image
        if (track_window.size.width < 20 || track_window.size.height < 20) {
          roi = def_roi;
        }
        track_window = CamShift(dsv, roi, term_criteria); 
       
        // calculate x coordinate of object and scale to -127 to 128
        x_center = track_window.center.x;
        x_center = (int) (((((double) x_center) / width) - 0.5) * (127.0 / 0.5));
        printf("x values; %d\n", x_center);  
        // draw pts
        //
        // draw rectangle
        track_window.points(rect_points);
        for (int i = 0; i < 4; i++) {
          line(imgLines, rect_points[i], rect_points[(i+1) % 4], Scalar(110, 80, 80), 2, 8);
        }
        int scaled_x = (int) ((( ((double) last_x) / width) - 0.5) * (127.0 / 0.5));

        printf("scaled_x %d\n", scaled_x);

        // draw tracking window superimposed onto live feed
        printf("mask_t: %d, imgLines: %d\n", mask.type(), imgLines.type());
        imshow("Thresholded", mask_conv);
        imgLines.Mat::convertTo(mask_conv, 0);
        mask = img_orig + imgLines;
        imshow("Display", mask);
        waitKey(30);
      }
    }



  private:
    VideoCapture cap_;
    vector<int> low_HSV_;
    vector<int> high_HSV_;
    Mat image_;
    int erode_it_;
    int dilate_it_;

    Mat generate_mask(Mat img) {
      Mat mask;
      cvtColor(img, mask, COLOR_BGR2HSV);
      inRange(mask, low_HSV_, high_HSV_, mask);
      erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), erode_it_);
      dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), dilate_it_);
      dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), dilate_it_);
      erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), erode_it_);

      return mask;
    }

};

int main(int argc, char **argv) {
  printf("hello world\n");

  vector<int>  low_HSV = {0, 20, 0};
  vector<int>  high_HSV = {180, 255, 255};

  ball_track bt(0, low_HSV, high_HSV, 1, 1);
  bt.track_ball_video(); 
  
  //while(true) {
  //  Mat result = bt.capture_image();
  //  imshow("Display", result);
  //  usleep(1000);
  //}
}




