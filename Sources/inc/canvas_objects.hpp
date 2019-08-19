/*
 * canvas_objects.hpp
 * Copyright Eric Bachard  2019 August 1st
 * License MIT
 *
 */

#include <vector>
#include "imgui.h"

#ifndef __CANVAS_OBJECTS_HPP
#define __CANVAS_OBJECTS_HPP

#define    TEXT_OBJECT_IMAGE_PATH        "./images/canvas/image_text.png"
#define    FILLED_RECTANGLE_IMAGE_PATH   "./images/canvas/image_filled_rectangle.png"
#define    EMPTY_RECTANGLE_IMAGE_PATH    "./images/canvas/image_empty_rectangle.png"
#define    FILLED_ELLIPSE_IMAGE_PATH     "./images/canvas/image_filled_ellipse.png"
#define    EMPTY_ELLIPSE_IMAGE_PATH      "./images/canvas/image_empty_ellipse.png"
#define    RANDOM_LINE_IMAGE_PATH        "./images/canvas/image_random_line.png"
#define    RANDOM_ARROW_IMAGE_PATH        "./images/canvas/image_random_arrow.png"
#define    SIMPLE_ARROW_IMAGE_PATH       "./images/canvas/image_simple_arrow.png"
#define    SIMPLE_LINE_IMAGE_PATH        "./images/canvas/image_simple_line.png"

#define    TEXT_OBJECT_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_text_dark.png"
#define    FILLED_RECTANGLE_IMAGE_DARK_PATH   "./images/canvas/dark_theme/image_filled_rectangle_dark.png"
#define    EMPTY_RECTANGLE_IMAGE_DARK_PATH    "./images/canvas/dark_theme/image_empty_rectangle_dark.png"
#define    FILLED_ELLIPSE_IMAGE_DARK_PATH     "./images/canvas/dark_theme/image_filled_ellipse_dark.png"
#define    EMPTY_ELLIPSE_IMAGE_DARK_PATH      "./images/canvas/dark_theme/image_empty_ellipse_dark.png"
#define    RANDOM_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_random_line_dark.png"
#define    RANDOM_ARROW_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_random_arrow_dark.png"
#define    SIMPLE_ARROW_IMAGE_DARK_PATH       "./images/canvas/dark_theme/image_simple_arrow_dark.png"
#define    SIMPLE_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_line_dark.png"

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
    TEXT_OBJECT        =   0,
    FILLED_RECTANGLE   =   1,
    EMPTY_RECTANGLE    =   2,
    FILLED_ELLIPSE     =   3,
    EMPTY_ELLIPSE      =   4,
    RANDOM_LINE        =   5,
    RANDOM_ARROW       =   6,
    SIMPLE_ARROW       =   7,
    SIMPLE_LINE        =   8,
    IMAGE_OBJECT       = 100,
    FRAME_OBJECT       = 200,
    NOT_A_DRAWN_OBJECT = 400
} ObjectType;

typedef struct DrawnObject
{
    unsigned int anObjectType;
    float        thickness;
    bool         selected;
    bool         hovered;
    bool         record;

    ImGui::InvisibleButton() invisibleButton;

    ImVector <ImVec2>  objectPoints;
    ImU32  objBackgroundColor;
    ImU32  objOutlineColor;
} DrawnObject;


//std::vector <DrawnObject> DrawnObjects;

#define CANVAS_OBJECTS_TYPES_MAX    9

#endif /*  __CANVAS_OBJECTS_HPP */
