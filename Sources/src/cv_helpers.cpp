/*
 * File cv_helpers.cpp, belongs to miniDart project
 * Copyright : Eric Bachard 2019 july 17th
 * This file is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <cstdio>
#include <opencv2/opencv.hpp>
#include "application.hpp"

//#include "cv_helpers.hpp"

#if !defined( PEN_WIDTH )
#define PEN_WIDTH 2
#endif

static int dBlue  = 255;
static int dRed   = 0;
static int dGreen = 0;

static bool b_even_events = false;
static bool b_clicked = false;

void message(void);
void MOUSE_CLICK(int, int , int , int, void *);
void addRectangle (cv::Mat, cv::Point, cv::Point, cv::Scalar);
bool recordAtGivenFPS(int, unsigned int, unsigned int);

cv::Point point1, point2, start_point;

//  The callback for mouse click event
void MOUSE_CLICK(int event, int x, int y, int flags, void *param)
{
#ifdef DEBUG
  std::cout<<"Event detected !! "<<std::endl;
#endif
  {
    switch(event)
    {
      case CV_EVENT_LBUTTONDOWN:
      {
        std::cout <<"Mouse LBUTTON Pressed"<< std::endl;
        b_even_events = false;
        b_clicked = true;
        point1.x = x;
        point1.y = y;
        point2.x = point1.x;
        point2.y = point1.y;
        start_point.x = point1.x;
        start_point.y = point1.y;
      }
      break;

      case CV_EVENT_MOUSEMOVE:
      {
#ifdef DEBUG
        std::cout << "Mouse moving" << std::endl;
#endif
        if (b_clicked)
        {
          start_point.x = point1.x;
          start_point.y = point1.y;
          point2.x = x;
          point2.y = y;
        }
        else
        {
          start_point.x = x;
          start_point.y = y;
        }
      }
      break;

      case CV_EVENT_LBUTTONUP:
      {
        b_clicked = false;
        std::cout << "Mouse LBUTTON Released" << std::endl;
        b_even_events = true;
        point2.x = x;
        point2.y = y;
        // LBUTTONUP is an even event, and to avoid noisy output, let's inform the user 
        // only after the new coordinates are set.
        message();
      }
      break;

      default:
      break;
    }
  }
}

void message(void)
{
  std::cout << "point1 coordinates : x1 = " << point1.x << ", y1 = " << point1.y << std::endl;
  std::cout << "point2 coordinates : x2 = " << point2.x << ", y2 = " << point2.y  << std::endl;
  std::cout << "\nType ESC to quit \n\n"    << std::endl;
}

void addRectangle (cv::Mat frame, cv::Point point1, cv::Point point2, cv::Scalar color)
{
    // Add the rectangle into the frame, only when a new rectangle has to be drawn
    if (((point2.x != point1.x) && (point2.y != point1.y)) && (b_even_events == true) && (b_clicked == false))
      rectangle(frame, cv::Point( point2.x, point2.y ), cv::Point( point1.x, point1.y ), cv::Scalar(dBlue, dGreen, dRed), PEN_WIDTH, 8, 0);

    // Feature : draw the new rectangle while resizing
    if ( b_clicked && (point2.x != point1.x) && (point2.y != point1.y))
      rectangle(frame, cv::Point( point2.x, point2.y ), cv::Point( start_point.x, start_point.y ), cv::Scalar(dBlue, dGreen, dRed), PEN_WIDTH, 8, 0);
}

bool recordAtGivenFPS(int fps, unsigned int current, unsigned int old)
{
    bool toReturn = false;

    unsigned int delta_time = (unsigned int)(1000.0f/fps);
    unsigned int loop_time = current - old;

    if (loop_time >= delta_time)
        toReturn = true;

    return toReturn;
}


