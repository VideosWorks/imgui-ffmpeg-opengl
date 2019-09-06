/*
 * canvas_objects.hpp
 * Copyright Eric Bachard  2019 August 1st
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#include <vector>
#include "imgui.h"

#ifndef __CANVAS_OBJECTS_HPP
#define __CANVAS_OBJECTS_HPP

#define    TEXT_OBJECT_IMAGE_PATH             "./images/canvas/image_text.png"
#define    FILLED_RECTANGLE_IMAGE_PATH        "./images/canvas/image_filled_rectangle.png"
#define    EMPTY_RECTANGLE_IMAGE_PATH         "./images/canvas/image_empty_rectangle.png"
#define    FILLED_CIRCLE_IMAGE_PATH           "./images/canvas/image_filled_circle.png"
#define    EMPTY_CIRCLE_IMAGE_PATH            "./images/canvas/image_empty_circle.png"
#define    RANDOM_LINE_IMAGE_PATH             "./images/canvas/image_random_line.png"
#define    RANDOM_ARROW_IMAGE_PATH            "./images/canvas/image_random_arrow.png"
#define    SIMPLE_ARROW_IMAGE_PATH            "./images/canvas/image_simple_arrow.png"
#define    SIMPLE_LINE_IMAGE_PATH             "./images/canvas/image_simple_line.png"
#define    FILLED_ELLIPSE_IMAGE_PATH          "./images/canvas/image_filled_ellipse.png"
#define    EMPTY_ELLIPSE_IMAGE_PATH           "./images/canvas/image_empty_ellipse.png"
#define    SELECT_CURSOR_IMAGE_PATH           "./images/canvas/image_select_cursor.png"

#define    TEXT_OBJECT_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_text_dark.png"
#define    FILLED_RECTANGLE_IMAGE_DARK_PATH   "./images/canvas/dark_theme/image_filled_rectangle_dark.png"
#define    EMPTY_RECTANGLE_IMAGE_DARK_PATH    "./images/canvas/dark_theme/image_empty_rectangle_dark.png"
#define    FILLED_CIRCLE_IMAGE_DARK_PATH      "./images/canvas/dark_theme/image_filled_circle_dark.png"
#define    EMPTY_CIRCLE_IMAGE_DARK_PATH       "./images/canvas/dark_theme/image_empty_circle_dark.png"
#define    RANDOM_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_random_line_dark.png"
#define    RANDOM_ARROW_IMAGE_DARK_PATH       "./images/canvas/dark_theme/image_random_arrow_dark.png"
#define    SIMPLE_ARROW_IMAGE_DARK_PATH       "./images/canvas/dark_theme/image_simple_arrow_dark.png"
#define    SIMPLE_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_line_dark.png"
#define    FILLED_ELLIPSE_IMAGE_DARK_PATH     "./images/canvas/dark_theme/image_filled_ellipse_dark.png"
#define    EMPTY_ELLIPSE_IMAGE_DARK_PATH      "./images/canvas/dark_theme/image_empty_ellipse_dark.png"
#define    SELECT_CURSOR_IMAGE_DARK_PATH      "./images/canvas/dark_theme/image_select_cursor_dark.png"

typedef enum DrawingWhat
{
    DRAWING_ZOOMED_AREA,
    FREEHAND_DRAWING,
    DRAWING_TEXT,
    DRAWING_PRIMITIVE,
    DRAWING_NOTHING
} DrawingWhat;

typedef enum ObjectType
{
    SELECT_CURSOR      =   0,
    TEXT_OBJECT        =   1,
    FILLED_RECTANGLE   =   2,
    EMPTY_RECTANGLE    =   3,
    FILLED_CIRCLE      =   4,
    EMPTY_CIRCLE       =   5,
    FILLED_ELLIPSE     =   6,
    EMPTY_ELLIPSE      =   7,
    RANDOM_LINE        =   8,
    RANDOM_ARROW       =   9,
    SIMPLE_ARROW       =  10,
    SIMPLE_LINE        =  11,
    IMAGE_OBJECT       =  12,
    FRAME_OBJECT       =  13,
    NOT_A_DRAWN_OBJECT = 100
} ObjectType;

#define CANVAS_OBJECTS_TYPES_MAX   12

typedef struct DrawnObject
{
    unsigned int anObjectType;// object type, defines properties
    float        thickness;
    float        P1P4;        // line length
    float        R2_in;       // (intern radius)^2
    float        R2_out;      // (extern radius)^2

    // ellipse properties must be calculated just after preview
    float        Xmax;
    float        Xmin;
    float        Ymax;
    float        Ymin;
    ImVec2       F1;          // ellipse focus point 1
    ImVec2       F2;          //         focus point 2
    float        radius_x;    // ellipse x radius
    float        radius_y;    // ellipse y radius
    float        rotation;    // rotation angle (CTRL key + MouseDrag)

    float        arrowLength;
    float        arrowWidth;

    bool         selected;
    bool         hovered;
    bool         record;
    bool         has_outline;

    ImVector <ImVec2>  hullPoints; // inside helpers
    ImVector <ImVec2>  objectPoints;  // depends on the case
    ImU32  objBackgroundColor;
    ImU32  objOutlineColor;
} DrawnObject;

#endif /*  __CANVAS_OBJECTS_HPP */
