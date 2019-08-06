/*
 * canvas_objects.hpp
 * Copyright Eric Bachard  2019 August 1st
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __CANVAS_OBJECTS_HPP
#define __CANVAS_OBJECTS_HPP

typedef enum ObjectType
{
    TEXT_OBJECT      = 0,
    FILLED_RECTANGLE = 1,
    EMPTY_RECTANGLE  = 2,
    FILLED_ELLIPSE   = 3,
    EMPTY_ELLIPSE    = 4,
    RANDOM_LINE      = 5,
    SIMPLE_ARROW     = 6,
    SIMPLE_LINE      = 7
} ObjectType;

#define    TEXT_OBJECT_IMAGE_PATH        "./images/canvas/image_text.png"
#define    FILLED_RECTANGLE_IMAGE_PATH   "./images/canvas/image_filled_rectangle.png"
#define    EMPTY_RECTANGLE_IMAGE_PATH    "./images/canvas/image_empty_rectangle.png"
#define    FILLED_ELLIPSE_IMAGE_PATH     "./images/canvas/image_filled_ellipse.png"
#define    EMPTY_ELLIPSE_IMAGE_PATH      "./images/canvas/image_empty_ellipse.png"
#define    RANDOM_LINE_IMAGE_PATH        "./images/canvas/image_random_line.png"
#define    SIMPLE_ARROW_IMAGE_PATH       "./images/canvas/image_simple_arrow.png"
#define    SIMPLE_LINE_IMAGE_PATH        "./images/canvas/image_simple_line.png"

#define    TEXT_OBJECT_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_text_dark.png"
#define    FILLED_RECTANGLE_IMAGE_DARK_PATH   "./images/canvas/dark_theme/image_filled_rectangle_dark.png"
#define    EMPTY_RECTANGLE_IMAGE_DARK_PATH    "./images/canvas/dark_theme/image_empty_rectangle_dark.png"
#define    FILLED_ELLIPSE_IMAGE_DARK_PATH     "./images/canvas/dark_theme/image_filled_ellipse_dark.png"
#define    EMPTY_ELLIPSE_IMAGE_DARK_PATH      "./images/canvas/dark_theme/image_empty_ellipse_dark.png"
#define    RANDOM_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_random_line_dark.png"
#define    SIMPLE_ARROW_IMAGE_DARK_PATH       "./images/canvas/dark_theme/image_simple_arrow_dark.png"
#define    SIMPLE_LINE_IMAGE_DARK_PATH        "./images/canvas/dark_theme/image_simple_line_dark.png"

#define CANVAS_OBJECTS_TYPES_MAX    8

#endif /*  __CANVAS_OBJECTS_HPP */
