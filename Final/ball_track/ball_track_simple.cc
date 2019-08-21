#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <unistd.h>

using namespace std;
using namespace cv;

class ball_track {

  public:     
    ball_track(int const camera, vector<int> low_HSV, vector<int> high_HSV, int erode_it, int dilate_it) : cap_(VideoCapture(camera)), low_HSV_(low_HSV),
    high_HSV_(high_HSV), erode_it_(erode_it), dilate_it_(dilate_it)
  {
    printf("VideoCapture cap_ is opened: %d\n", (int) cap_.isOpened());

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

      Mat img;
      cap_.read(img);
      int last_x = -1;
      int last_y = -1;

      int width = img.size().width;
      int height = img.size().height;

      Mat imgLines = Mat::zeros(img.size(), CV_8UC3);

      while (true) {
        Mat img_orig;
        if (!cap_.read(img_orig)) {
          printf("can't read from camera\n");
          break;
        }
        Mat mask = generate_mask(img_orig);
        Moments oMoments = moments(mask);
        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00;

        // if object is more than 10k pixels
        if (dArea > 10000) {
          int cur_x = dM10 / dArea;
          int cur_y = dM01 / dArea;
          if (last_x >= 0 && cur_x >= 0 && last_y >= 0 && cur_y >= 0) {
            line(imgLines, Point(cur_x, cur_y), Point(last_x, last_y), Scalar(0,0,255), 2);
          }
          last_x = cur_x;
          last_y = cur_y;
        }

        //compute x scale
        
        int scaled_x = (int) ((( ((double) last_x) / width) - 0.5) * (127.0 / 0.5));

        printf("scaled_x %d\n", scaled_x);


        printf("mask: %d %d, imgLines: %d %d\n", mask.size().width, mask.size().height, imgLines.size().width, imgLines.size().height);
        printf("mask_t: %d, imgLines: %d\n", mask.type(), imgLines.type());
        Mat mask_conv;
        imshow("Thresholded", mask);
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
      inRange(img, low_HSV_, high_HSV_, mask);
      erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), erode_it_);
      dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), dilate_it_);
      dilate(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), dilate_it_);
      erode(mask, mask, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)), Point(-1, -1), erode_it_);

      return mask;
    }

};

int main(int argc, char **argv) {
  printf("hello world\n");

  vector<int>  low_HSV = {0, 181, 200};
  vector<int>  high_HSV = {112, 255, 255};

  ball_track bt(0, low_HSV, high_HSV, 1, 1);
  bt.track_ball_video(); 

  //while(true) {
  //  Mat result = bt.capture_image();
  //  imshow("Display", result);
  //  usleep(1000);
  //}
}




