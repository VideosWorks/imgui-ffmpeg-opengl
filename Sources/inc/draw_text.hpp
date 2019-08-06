/*
 * File draw_text.hpp, belongs to miniDart project
 * Copyright : Eric Bachard  / Created by Eric Bachard  2019 June 31
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __DRAW_TEXT_HPP
#define __DRAW_TEXT_HPP

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/freetype.hpp>

#include <sstream>
#include <iostream>
#include <cstring>
#include <cctype>
#include <string>

#define DEFAULT_INCRUSTATION_DURATION      10
#define DEFAULT_TEXT_INCRUSTATION_BASELINE  0
#define DEFAULT_THICKNESS                   1
#define DEFAULT_LINETYPE                    0
#define DEFAULT_LINESTYLE                   1
#define DEFAULT_TEXT_INCRUSTATION_BASELINE  0
#define DEFAULT_FONTSIZE                   24

#define CHARACTERS_PER_LINE_MAX 120
// Dear ImGui limit is 127 
// (or have a look at misc/cpp/imgui_stdlib.cpp)
#define CHARACTERS_MAX  125

#define WORD_SIZE_MAX 30

#ifdef NATIVE_BUILD
// Linux or Mac OS X
#define GAP 2
#else
// Windows
#define GAP 3
#endif


typedef enum H_position {
    H_LEFT = 0,
    H_CENTER = 1,
    H_RIGHT  = 2,
    H_MOVING = 3,
    H_CUSTOM = 4
} H_Position;

typedef enum V_position {
    V_TOP = 0,
    V_MIDDLE = 1,
    V_BOTTOM    = 2,
    V_MOVING = 3,
    V_CUSTOM = 4
} V_Position;


namespace md  // miniDart alias

{
    typedef struct Text_Object
    {
        const char * annotations_current_font; // default to  DroidSans.ttf on Linux, and Arial.ttf on Windows
        std::string sText;
        char ligne1[CHARACTERS_PER_LINE_MAX];
        char ligne2[CHARACTERS_PER_LINE_MAX];
        short int duration;
        bool b_unlimited_annotation;
        bool b_draw_text_baseline;
        bool b_draw_outline;
        bool b_fillpoly;
        bool b_displayable;
        bool b_text_changed;
        bool b_text_auto_positioned;
        int frameCount;
        int vPosition;
        int hPosition;
        short int number_of_lines;
        cv::Size textSize;
        char fontName[128]; 
        int fontHeight;
        int thickness;
        int lineType;
        int lineStyle;
        int baseline;
        cv::Ptr<cv::freetype::FreeType2> ft2;
        cv::Point     textOrg;
        cv::Point     delta; // allows to move (means animate) the text
        cv::Point     box_points[1][4];
        cv::Scalar    fontColor;
        cv::Scalar    outlineColor;
        cv::Scalar    backgroundColor;
        cv::Scalar    baselineColor;

    } TEXT_Object;

    class TextCanvas
    {
        public:
          // Ctor
          TextCanvas(md::TEXT_Object *);
          // Dtor
          ~TextCanvas();

          bool init();
          // This should be helpfull later
          std::vector <md::TEXT_Object > vTEXTObjects;
          md::TEXT_Object * pTextObject;
          // two lines maxi
          bool      createTextLine(short int line);
          short int wrap_text_in_lines(void);
          void      stopStringIncrustation(); // signal sent to the renderer, e.g. wrong string, and so on
          void      clearString(void);
          int       insertString( cv::Mat aFrame,
                                  md::TEXT_Object * pTextObject);

        private:
    };
}

#endif /*  __DRAW_TEXT_HPP  */
