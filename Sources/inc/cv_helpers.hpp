/*
  File cv_helpers.hpp, belongs to miniDart project
  Copyright : Eric Bachard 2019 july 17th
  This file is under GPL v2 license
 */

#ifndef __CV_HELPERS_HPP
#define __CV_HELPERS_HPP

#include "imgui.h"
#include <opencv2/opencv.hpp>

cv::Point ImGui_toCV_Point(ImVec2 *);
ImVec2 CV_Point_toImGui(cv::Point *);

void message(void);
void MOUSE_CLICK(int, int , int , int, void *);
void addRectangle (cv::Mat, cv::Point, cv::Point, cv::Scalar);
bool recordAtGivenFPS(int, unsigned int, unsigned int);

#endif
