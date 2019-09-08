/* 
 * file canvas.cpp, belongs to miniDart project
 * Copyright Eric Bachard 2019 August 19th   17:55:00   CEST
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "canvas.hpp"
#include "helpers.h"

typedef enum MoveToChoice
{
  PUT_OBJECT_ON_TOP,
  ONE_LEVEL_UP,
  PUT_OBJECT_AT_THE_BOTTOM,
  ONE_LEVEL_DOWN,
  DELETE_OBJECT
} MoveToChoice;


template<typename T> static inline T ImMin(T lhs, T rhs)   { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs)   { return lhs >= rhs ? lhs : rhs; }


const std::string canvasObjectImagePath[CANVAS_OBJECTS_TYPES_MAX] =
{
    SELECT_CURSOR_IMAGE_DARK_PATH,
    TEXT_OBJECT_IMAGE_DARK_PATH,
    FILLED_RECTANGLE_IMAGE_DARK_PATH,
    EMPTY_RECTANGLE_IMAGE_DARK_PATH,
    FILLED_CIRCLE_IMAGE_DARK_PATH,
    EMPTY_CIRCLE_IMAGE_DARK_PATH,
    FILLED_ELLIPSE_IMAGE_DARK_PATH,
    EMPTY_ELLIPSE_IMAGE_DARK_PATH,
    RANDOM_LINE_IMAGE_DARK_PATH,
    RANDOM_ARROW_IMAGE_DARK_PATH,
    SIMPLE_ARROW_IMAGE_DARK_PATH,
    SIMPLE_LINE_IMAGE_DARK_PATH,
};

md::Canvas::Canvas()
{
};

md::Canvas::~Canvas()
{
};


void md::Canvas::loadCanvasObjectsIcons(void)
{
    for (short i = 0 ; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        canvasObjectImage[i] = cv::imread(canvasObjectImagePath[i]);
    }
}

void md::Canvas::createCanvasObjectsImagesTexIds(void)
{
    for (short int i = 0; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        this->canvasObjectImageTexId[i] = glConvertMatToTexture(canvasObjectImage[i]);
    }
}

void md::Canvas::cleanCanvasObjectsImagesTexIds(void)
{
    short int i = 0;
    for (i = 0; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        if (canvasObjectImageTexId[i] != 0)
            glDeleteTextures(1, &canvasObjectImageTexId[i]);
    }
}

bool md::Canvas::init()
{
    adding_rect     = false;
    adding_preview1 = false;
    adding_rect2    = false;
    adding_preview2 = false;

    topLeft = ImVec2(0.0f, 0.0f);
    bottomRight = ImVec2(0.0f, 0.0f);

    DrawnObject aDrawnObject;
    aDrawnObject.thickness = DEFAULT_OBJECT_THICKNESS;

    iconWidth     = DEFAULT_ICON_WIDTH;
    iconHeight    = DEFAULT_ICON_HEIGHT;
    frame_padding = DEFAULT_FRAME_PADDING;
    bcol          = ImVec4(0.3f, 0.4f, 1.0f, 0.5f);
    return true;
}

void md::Canvas::preview(int selectedObject, ImU32 color, int w, float ratio, float outline_thickness)
{
    setMousePosValid(w, ratio);

    aDrawnObject.radius_x = 1.0f + ImGui::GetMouseDragDelta().x;
    aDrawnObject.radius_y = 1.0f + ImGui::GetMouseDragDelta().y;

    if (fabs(aDrawnObject.radius_x) <= 0.1f)
        aDrawnObject.radius_x = 1.0f;

    aDrawnObject.rotation = ImGui::GetIO().KeyCtrl ? aDrawnObject.radius_y / aDrawnObject.radius_x : 0.0f;

    switch(aDrawnObject.anObjectType)
    {
        case EMPTY_RECTANGLE:
        case EMPTY_CIRCLE:
        case EMPTY_ELLIPSE:
        case FILLED_RECTANGLE:
        case FILLED_CIRCLE:
        case FILLED_ELLIPSE:
        case SIMPLE_LINE:
        case SIMPLE_ARROW:
            catchPrimitivesPoints();

        break;

        case RANDOM_LINE:
            static bool adding_circle   = false;

            if (adding_circle)
            {
                aDrawnObject.anObjectType = selectedObject;
                aDrawnObject.objectPoints.push_back(mouse_pos_in_image);

                for (int i = 0 ; i < aDrawnObject.objectPoints.size(); i++)
                {
                    ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[i].x,
                                                                        mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[i].y),
                                                                 aDrawnObject.thickness,
                                                                 aDrawnObject.objBackgroundColor,
                                                                 8);
                }

                if (!ImGui::GetIO().MouseDown[0])
                {
                    adding_circle = false;
                    currentlyDrawnObjects.push_back(aDrawnObject);

                    while (!aDrawnObject.objectPoints.empty())
                    {
                        aDrawnObject.objectPoints.pop_back();
                    }
                }
            }

            if (ImGui::IsItemHovered())
            {
                if ( (ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging() )
                {
                    adding_circle = false;
                }

                if ((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && ImGui::IsMouseDragging())
                    adding_circle = true;

                if ( (!adding_circle && ImGui::IsMouseClicked(0)) )
                {
                    aDrawnObject.objectPoints.push_back(mouse_pos_in_image);
                    adding_circle = true;
                }
            }
        break;

        case RANDOM_ARROW:
            static bool adding_circle2   = false;

            if (adding_circle2)
            {
                aDrawnObject.anObjectType = selectedObject;
                arrow_points.push_back(mouse_pos_in_image);

                for (int i = 0 ; i < arrow_points.size(); i++)
                {
                    ImGui::GetOverlayDrawList()->AddCircleFilled( ImVec2(mp_TextCanvas->image_pos.x + arrow_points[i].x, mp_TextCanvas->image_pos.y + arrow_points[i].y), aDrawnObject.thickness, aDrawnObject.objBackgroundColor, 8);
                }

                if (!ImGui::GetIO().MouseDown[0])
                {
                    adding_circle2 = false;

                    aDrawnObject.objectPoints.push_back(arrow_points[0]);
                    aDrawnObject.objectPoints.push_back(arrow_points  [(int)(arrow_points.size()/3.0f)]);
                    aDrawnObject.objectPoints.push_back(arrow_points[(int)((2*arrow_points.size())/3.0f)]);
                    aDrawnObject.objectPoints.push_back(arrow_points[arrow_points.size()-1]);
                    aDrawnObject.P1P4 = sqrtf(  (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)
                                               *(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)
                                              + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)
                                               *(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y));
                    currentlyDrawnObjects.push_back(aDrawnObject);

                    arrow_points.clear();
                    aDrawnObject.objectPoints.clear();
                }
            }

            if (ImGui::IsItemHovered())
            {
                if ( (ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging() )
                    adding_circle2 = false;

                if ((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && ImGui::IsMouseDragging())
                    adding_circle2 = true;

                if ( (!adding_circle2 && ImGui::IsMouseClicked(0)) )
                {
                    arrow_points.push_back(mouse_pos_in_image);
                    adding_circle2 = true;
                }
            }
        break;

        case SELECT_CURSOR:
        case NOT_A_DRAWN_OBJECT:
        {
            if (adding_rect)
            {
                adding_preview1 = true;
                zoom_area_points.push_back(mouse_pos_in_image);

                if (!ImGui::GetIO().MouseDown[0])
                   adding_rect = adding_preview1 = false;
            }


            if (ImGui::IsItemHovered())
            {
                if ( (((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1) )  && (!zoom_area_points.empty()))) && !ImGui::IsMouseDragging() )
                {
                    adding_rect = false;
                    adding_preview1 = false;
                    zoom_area_points.pop_back();
                    zoom_area_points.pop_back();
                }

                if ( (!adding_rect && ImGui::IsMouseClicked(0)) )
                {
                    zoom_area_points.push_back(mouse_pos_in_image);
                    adding_rect = true;
                }
            }
            updateSelectedArea(zoom_area_points, color, outline_thickness);
            reorder_points(&topLeft, &bottomRight);

            if (adding_preview1)
                zoom_area_points.pop_back();
        }
        break;

        case TEXT_OBJECT:
        default:
        break;
    }
}

void md::Canvas::updateSelectedArea(ImVector <ImVec2> zoom_area_points, ImU32 color, float outline_thickness)
{
    for (int i = 0; i < zoom_area_points.Size - 1; i += 2)
    {
        if ((abs(zoom_area_points[i].x - zoom_area_points[i+1].x) > 2) && (abs(zoom_area_points[i].y - zoom_area_points[i+1].y)> 2))
        {
            ImGui::GetOverlayDrawList()->AddRect(ImVec2(mp_TextCanvas->image_pos.x + zoom_area_points[i].x, mp_TextCanvas->image_pos.y + zoom_area_points[i].y),
                                                 ImVec2(mp_TextCanvas->image_pos.x + zoom_area_points[i+1].x, mp_TextCanvas->image_pos.y + zoom_area_points[i+1].y),
                                                 color, 0.0f, ~0 ,outline_thickness);
            topLeft = zoom_area_points[i];
            bottomRight = zoom_area_points[i+1];
        }
        else
        {
            topLeft = ImVec2(0.0f,0.0f);
            bottomRight = ImVec2(ZOOM_WIDTH_MIN, ZOOM_HEIGHT_MIN);
        }
    }
}




bool  md::Canvas::addObject()
{
    return true;
}

void md::Canvas::update(ImVec2 mousePos)
{
    for (unsigned int i = 0; i < currentlyDrawnObjects.size(); i++)
    {
        {
            switch(currentlyDrawnObjects[i].anObjectType)
            {
                case FILLED_RECTANGLE:
                    currentlyDrawnObjects[i].hovered = insideFilledRectangle(mousePos, currentlyDrawnObjects[i].objectPoints);
                break;

                case FILLED_CIRCLE:
                    currentlyDrawnObjects[i].hovered = insideCircle(mousePos, currentlyDrawnObjects[i].objectPoints[0], currentlyDrawnObjects[i].R2_out );
                break;

                case FILLED_ELLIPSE:
                break;

                case EMPTY_RECTANGLE:
                break;

                case EMPTY_CIRCLE:
                    currentlyDrawnObjects[i].hovered = intersectEmptyCircle(mousePos,
                                                                            currentlyDrawnObjects[i].objectPoints[0],
                                                                            currentlyDrawnObjects[i].R2_in,
                                                                            currentlyDrawnObjects[i].R2_out);
                break;

                case EMPTY_ELLIPSE:
                break;

                case RANDOM_ARROW:
                break;

                case SIMPLE_ARROW:
                break;

                case SIMPLE_LINE:
                    currentlyDrawnObjects[i].hovered = intersectSegment(mousePos, currentlyDrawnObjects[i].objectPoints[0], currentlyDrawnObjects[i].objectPoints[0]);
                break;

                default:
                break;
            }
        }
    }
}

void  md::Canvas::setMousePosValid(int w, float ratio)
{
    mouse_pos_in_image = ImVec2(ImGui::GetIO().MousePos.x - mp_TextCanvas->image_pos.x,  (ImGui::GetIO().MousePos.y - mp_TextCanvas->image_pos.y));

    if ( mouse_pos_in_image.x < 0 )
        mouse_pos_in_image.x = LEFT_IMAGE_BORDER;

    if (( mouse_pos_in_image.y < 0 ))
        mouse_pos_in_image.y = TOP_IMAGE_BORDER;

    if ( mouse_pos_in_image.x > (RIGHT_IMAGE_BORDER) )
        mouse_pos_in_image.x = RIGHT_IMAGE_BORDER;

    if (( mouse_pos_in_image.y > BOTTOM_IMAGE_BORDER ))
        mouse_pos_in_image.y = BOTTOM_IMAGE_BORDER;
}


void md::Canvas::catchPrimitivesPoints(void)
{
    if (adding_rect2)
    {
        adding_preview2 = true;
        aDrawnObject.objectPoints.push_back(mouse_pos_in_image);

        aDrawnObject.P1P4 = sqrtf(  (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)
                                  + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y) );

        if (!aDrawnObject.objectPoints.empty())
        {
            switch(aDrawnObject.anObjectType)
            {
                case EMPTY_RECTANGLE:
                    ImGui::GetOverlayDrawList()->AddRect(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                         ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[1].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                         aDrawnObject.objBackgroundColor,
                                                         0.0f,
                                                         ~0 ,
                                                         aDrawnObject.thickness);
                break;

                case EMPTY_ELLIPSE:
                    ImGui::GetOverlayDrawList()->AddEllipse(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                   mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                            aDrawnObject.radius_x,
                                                            aDrawnObject.radius_y,
                                                            aDrawnObject.objBackgroundColor,
                                                            aDrawnObject.rotation,
                                                            32,
                                                            aDrawnObject.thickness);
                break;

                case FILLED_ELLIPSE:
                    ImGui::GetOverlayDrawList()->AddEllipseFilled(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                         mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                                  aDrawnObject.radius_x,
                                                                  aDrawnObject.radius_y,
                                                                  aDrawnObject.objBackgroundColor,
                                                                  aDrawnObject.rotation,
                                                                  32);
                break;

                case EMPTY_CIRCLE:
                    ImGui::GetOverlayDrawList()->AddCircle(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                  mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                           aDrawnObject.P1P4,
                                                           aDrawnObject.objBackgroundColor, 32, aDrawnObject.thickness);
                break;

                case FILLED_RECTANGLE:
                    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                      mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                               ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[1].x,
                                                                      mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                               aDrawnObject.objBackgroundColor);
                break;

                case FILLED_CIRCLE:
                    ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                        mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                                 aDrawnObject.P1P4,
                                                                 aDrawnObject.objBackgroundColor,
                                                                 32);
                break;

                case SIMPLE_LINE:
                    ImGui::GetOverlayDrawList()->AddLine(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                         ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[1].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                         aDrawnObject.objBackgroundColor,
                                                         aDrawnObject.thickness);
                break;

                case SIMPLE_ARROW:
                    ImGui::GetOverlayDrawList()->AddLine(ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[0].y),
                                                         ImVec2(mp_TextCanvas->image_pos.x + aDrawnObject.objectPoints[1].x,
                                                                mp_TextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                         aDrawnObject.objBackgroundColor,
                                                         aDrawnObject.thickness);

                    if (aDrawnObject.P1P4 > 1.5f * aDrawnObject.arrowLength)
                    {
                         ImVec2 pointC(  aDrawnObject.objectPoints[1].x - (aDrawnObject.arrowLength * (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/aDrawnObject.P1P4,
                                         aDrawnObject.objectPoints[1].y - (aDrawnObject.arrowLength * (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/aDrawnObject.P1P4);
                         ImVec2 pointD(  pointC.x + (aDrawnObject.arrowWidth*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/aDrawnObject.P1P4,
                                         pointC.y - (aDrawnObject.arrowWidth*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/aDrawnObject.P1P4);
                         ImVec2 pointE(  pointC.x - (aDrawnObject.arrowWidth*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/aDrawnObject.P1P4,
                                         pointC.y + (aDrawnObject.arrowWidth*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/aDrawnObject.P1P4);

                         ImGui::GetOverlayDrawList()->PathClear();
                         ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(pointD.x + mp_TextCanvas->image_pos.x, pointD.y + mp_TextCanvas->image_pos.y));
                         ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(aDrawnObject.objectPoints[1].x + mp_TextCanvas->image_pos.x, aDrawnObject.objectPoints[1].y + mp_TextCanvas->image_pos.y));
                         ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(pointE.x + mp_TextCanvas->image_pos.x, pointE.y + mp_TextCanvas->image_pos.y));
                         ImGui::GetOverlayDrawList()->PathStroke(aDrawnObject.objBackgroundColor, false, aDrawnObject.thickness);
                    }
                break;

                default:
                break;
            }
        }

        if (!ImGui::GetIO().MouseDown[0])
        {
#ifdef DEBUG
            std::cout << "Souris relâchée !!  " << __LINE__ << std::endl;
#endif
            adding_rect2    = false;
            adding_preview2 = false;

            switch(aDrawnObject.anObjectType)
            {
                case FILLED_ELLIPSE:
                case EMPTY_ELLIPSE:
                {
                    aDrawnObject.Xmax = aDrawnObject.objectPoints[0].x + ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y);
                    aDrawnObject.Xmin = aDrawnObject.objectPoints[0].x - ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y);
                    aDrawnObject.Ymax = aDrawnObject.objectPoints[0].y + ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y);
                    aDrawnObject.Ymin = aDrawnObject.objectPoints[0].y - ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y);

                    float e = 1 - (    (ImMin(aDrawnObject.radius_x, aDrawnObject.radius_y)*(ImMin(aDrawnObject.radius_x, aDrawnObject.radius_y)))
                                     / (ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y)*(ImMax(aDrawnObject.radius_x, aDrawnObject.radius_y))) );

                    float xA = aDrawnObject.radius_x * cos(aDrawnObject.rotation);
                    float yA = aDrawnObject.radius_y * sin(aDrawnObject.rotation);

                    aDrawnObject.F1.x = aDrawnObject.objectPoints[0].x + (e *( aDrawnObject.objectPoints[0].x - xA));
                    aDrawnObject.F1.y = aDrawnObject.objectPoints[0].y + (e *( aDrawnObject.objectPoints[0].x - yA));
                    aDrawnObject.F2.x = aDrawnObject.objectPoints[0].x - (e *( aDrawnObject.objectPoints[0].x - xA));
                    aDrawnObject.F2.y = aDrawnObject.objectPoints[0].y - (e *( aDrawnObject.objectPoints[0].x - xA));
                }
                break;

                case EMPTY_CIRCLE:
                case FILLED_CIRCLE:
                {
                    aDrawnObject.R2_out = (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)
                                        + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y);
                    aDrawnObject.P1P4 = sqrtf(aDrawnObject.R2_out);
                }
                break;

                case SIMPLE_ARROW:
                {
                    aDrawnObject.P1P4 = sqrtf(  (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)
                                              + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y) );
                }
                break;

                default:
                break;
             }

             currentlyDrawnObjects.push_back(aDrawnObject);
             aDrawnObject.objectPoints.clear();
        }
 
    } /* adding_rect2 */

    if (ImGui::IsItemHovered())
    {
        if ( (ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging() )
        {
            adding_rect2 = adding_preview2 = false;
        }

        if ( (!adding_rect2 && ImGui::IsMouseClicked(0)) )
        {
            aDrawnObject.objectPoints.push_back(mouse_pos_in_image);
            adding_rect2 = true;
        }
    }

    if ((adding_preview2) && !(aDrawnObject.anObjectType == RANDOM_LINE))
        aDrawnObject.objectPoints.pop_back();

} /* Canvas::catchPrimitivesPoints(void) */

 

int md::Canvas::draw()
{

    ImDrawList * p_drawList = ImGui::GetWindowDrawList();
    ImVec2 subview_size = mp_TextCanvas->image_size;
    p_drawList->PushClipRect(ImVec2(0.0f, 0.0f), ImVec2(mp_TextCanvas->image_pos.x + subview_size.x, mp_TextCanvas->image_pos.y + subview_size.y) );

    for (unsigned int i = 0; i < currentlyDrawnObjects.size(); i++)
    {
        {
            switch(currentlyDrawnObjects[i].anObjectType)
            {
                case EMPTY_RECTANGLE:
                    p_drawList->AddRect(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x, mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                        ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[1].x, mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y),
                                        currentlyDrawnObjects[i].objBackgroundColor,
                                        0.0f,
                                        ~0,
                                        currentlyDrawnObjects[i].thickness);
                break;

                case EMPTY_CIRCLE:
                                    p_drawList->AddCircle(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x, mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y),
                                    currentlyDrawnObjects[i].P1P4,
                                    currentlyDrawnObjects[i].objBackgroundColor, 32, currentlyDrawnObjects[i].thickness);
                break;

                case FILLED_RECTANGLE:
                    p_drawList->AddRectFilled(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                                     mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                              ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[1].x,
                                                     mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y),
                                              currentlyDrawnObjects[i].objBackgroundColor);
                break;

                case FILLED_CIRCLE:
                    p_drawList->AddCircleFilled(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                                       mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                                currentlyDrawnObjects[i].P1P4,
                                                currentlyDrawnObjects[i].objBackgroundColor,
                                                32);
                break;

                case EMPTY_ELLIPSE:
                    p_drawList->AddEllipse(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                                  mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                          currentlyDrawnObjects[i].radius_x,
                                          currentlyDrawnObjects[i].radius_y,
                                          currentlyDrawnObjects[i].objBackgroundColor,
                                          currentlyDrawnObjects[i].rotation,
                                          32,
                                          currentlyDrawnObjects[i].thickness);
                break;

                case FILLED_ELLIPSE:
                    p_drawList->AddEllipseFilled(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                                        mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                                currentlyDrawnObjects[i].radius_x,
                                                currentlyDrawnObjects[i].radius_y,
                                                currentlyDrawnObjects[i].objBackgroundColor,
                                                currentlyDrawnObjects[i].rotation,
                                                32);
                    break;


                case SIMPLE_LINE:
                    p_drawList->AddLine(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                               mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                        ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[1].x,
                                               mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y),
                                        currentlyDrawnObjects[i].objBackgroundColor,
                                        currentlyDrawnObjects[i].thickness);
                break;

                case SIMPLE_ARROW:

                    p_drawList->AddLine(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                               mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),
                                        ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[1].x,
                                               mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y),
                                        currentlyDrawnObjects[i].objBackgroundColor,
                                        currentlyDrawnObjects[i].thickness);

                    if (currentlyDrawnObjects[i].P1P4 > 1.5f * currentlyDrawnObjects[i].arrowLength)
                    {
                        ImVec2 pointC(  currentlyDrawnObjects[i].objectPoints[1].x - (currentlyDrawnObjects[i].arrowLength * (currentlyDrawnObjects[i].objectPoints[1].x - currentlyDrawnObjects[i].objectPoints[0].x))/currentlyDrawnObjects[i].P1P4,
                                        currentlyDrawnObjects[i].objectPoints[1].y - (currentlyDrawnObjects[i].arrowLength * (currentlyDrawnObjects[i].objectPoints[1].y - currentlyDrawnObjects[i].objectPoints[0].y))/currentlyDrawnObjects[i].P1P4);
                        ImVec2 pointD(  pointC.x + (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[1].y - currentlyDrawnObjects[i].objectPoints[0].y))/currentlyDrawnObjects[i].P1P4,
                                        pointC.y - (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[1].x - currentlyDrawnObjects[i].objectPoints[0].x))/currentlyDrawnObjects[i].P1P4);
                        ImVec2 pointE(  pointC.x - (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[1].y - currentlyDrawnObjects[i].objectPoints[0].y))/currentlyDrawnObjects[i].P1P4,
                                        pointC.y + (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[1].x - currentlyDrawnObjects[i].objectPoints[0].x))/currentlyDrawnObjects[i].P1P4);

                        p_drawList->PathClear();
                        p_drawList->PathLineTo(ImVec2(pointD.x + mp_TextCanvas->image_pos.x, pointD.y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathLineTo(ImVec2(currentlyDrawnObjects[i].objectPoints[1].x + mp_TextCanvas->image_pos.x, currentlyDrawnObjects[i].objectPoints[1].y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathLineTo(ImVec2(pointE.x + mp_TextCanvas->image_pos.x, pointE.y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathStroke(currentlyDrawnObjects[i].objBackgroundColor, false, currentlyDrawnObjects[i].thickness);
                    }
                break;

                case RANDOM_ARROW:
                    for (int j = 2 ; j < currentlyDrawnObjects[i].objectPoints.size() ; j++)
                    {
                        p_drawList->AddBezierCurve(   ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[0].x,
                                                             mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[0].y),  //P1 == start
                                                      ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[1].x,
                                                             mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[1].y), // P2 == CP1
                                                      ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[2].x,
                                                             mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[2].y), // P3 == CP2
                                                      ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[3].x,
                                                             mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[3].y), // P4 == end
                                                      currentlyDrawnObjects[i].objBackgroundColor,
                                                      currentlyDrawnObjects[i].thickness,
                                                      64);
                    }

                    if ((currentlyDrawnObjects[i].P1P4 > 0.0f) && (currentlyDrawnObjects[i].P1P4 > 1.5f * currentlyDrawnObjects[i].arrowLength))
                    {
                        ImVec2 pointC(  currentlyDrawnObjects[i].objectPoints[3].x - (currentlyDrawnObjects[i].arrowLength * (currentlyDrawnObjects[i].objectPoints[3].x - currentlyDrawnObjects[i].objectPoints[2].x))/currentlyDrawnObjects[i].P1P4,
                                        currentlyDrawnObjects[i].objectPoints[3].y - (currentlyDrawnObjects[i].arrowLength * (currentlyDrawnObjects[i].objectPoints[3].y - currentlyDrawnObjects[i].objectPoints[2].y))/currentlyDrawnObjects[i].P1P4);
                        ImVec2 pointD(  pointC.x + (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[3].y - currentlyDrawnObjects[i].objectPoints[2].y))/currentlyDrawnObjects[i].P1P4,
                                        pointC.y - (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[3].x - currentlyDrawnObjects[i].objectPoints[2].x))/currentlyDrawnObjects[i].P1P4);
                        ImVec2 pointE(  pointC.x - (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[3].y - currentlyDrawnObjects[i].objectPoints[2].y))/currentlyDrawnObjects[i].P1P4,
                                        pointC.y + (currentlyDrawnObjects[i].arrowWidth*(currentlyDrawnObjects[i].objectPoints[3].x - currentlyDrawnObjects[i].objectPoints[2].x))/currentlyDrawnObjects[i].P1P4);

                        p_drawList->PathClear();
                        p_drawList->PathLineTo(ImVec2(pointD.x + mp_TextCanvas->image_pos.x, pointD.y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathLineTo(ImVec2(currentlyDrawnObjects[i].objectPoints[3].x + mp_TextCanvas->image_pos.x, currentlyDrawnObjects[i].objectPoints[3].y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathLineTo(ImVec2(pointE.x + mp_TextCanvas->image_pos.x, pointE.y + mp_TextCanvas->image_pos.y));
                        p_drawList->PathStroke(currentlyDrawnObjects[i].objBackgroundColor, false, currentlyDrawnObjects[i].thickness);

                    }
                break;

                case RANDOM_LINE :
                    for (unsigned int i = 0 ; i < currentlyDrawnObjects.size(); i++)
                    {
                        for (int j = 0 ; j < currentlyDrawnObjects[i].objectPoints.size() ; j++)
                        {
                            if (currentlyDrawnObjects[i].anObjectType == RANDOM_LINE)
                                p_drawList->AddCircleFilled(ImVec2(mp_TextCanvas->image_pos.x + currentlyDrawnObjects[i].objectPoints[j].x,
                                                                   mp_TextCanvas->image_pos.y + currentlyDrawnObjects[i].objectPoints[j].y),
                                                            currentlyDrawnObjects[i].thickness,
                                                            currentlyDrawnObjects[i].objBackgroundColor,
                                                            8);
                        }

                    }

                break;

                default:
                break;
            }
        }
    }
    p_drawList->PopClipRect();

    return 0;
}

// Other possibility coul have been zero-rule algorithm : https://en.wikipedia.org/wiki/Nonzero-rule)
// Desserves a read : https://erich.realtimerendering.com/ptinpoly/

// Use even-odd rule, as a compromise inspired from
// https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm/20868925#20868925
// More information :
// https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule

bool md::Canvas::remove(unsigned int position)
{
    currentlyDrawnObjects.erase(currentlyDrawnObjects.begin()+position);
    return true;
}


bool md::Canvas::moveObjectTo(unsigned int positionInStack, int choice)
{
    switch(choice)
    {
        case PUT_OBJECT_ON_TOP:
        if (positionInStack < (currentlyDrawnObjects.size() - 1))
        {
            for (unsigned int position = positionInStack ; position < (currentlyDrawnObjects.size() - 1) ; position++)
            {
                std::swap(currentlyDrawnObjects[positionInStack-1], currentlyDrawnObjects[positionInStack]);
            }
        }
        break;

        case ONE_LEVEL_DOWN:
        if (positionInStack > 0)
        {
            std::swap(currentlyDrawnObjects[positionInStack-1], currentlyDrawnObjects[positionInStack]);
        }
        break;

        case ONE_LEVEL_UP:
        if (positionInStack < (currentlyDrawnObjects.size()-1))
        {
            std::swap(currentlyDrawnObjects[positionInStack], currentlyDrawnObjects[positionInStack+1]);
        }
        break;

        case PUT_OBJECT_AT_THE_BOTTOM:
        if (positionInStack > 0)
        {
            for (unsigned int position = (positionInStack -1) ; position > 0 ; position--)
            {
                std::swap(currentlyDrawnObjects[positionInStack-1], currentlyDrawnObjects[positionInStack]);
            }
        }
        break;

        case DELETE_OBJECT:
            remove(positionInStack);
        break;

        default:
        break;
    }

    return true;
}

bool md::Canvas::insideCircle(ImVec2 mousePos, ImVec2 center, float R2)
{
   if ( ((mousePos.x - center.x)*(mousePos.x - center.x)*(mousePos.y - center.y)*(mousePos.y - center.y)) <= R2 )
       return true;
   else
       return false;
}

bool md::Canvas::intersectEmptyCircle(ImVec2 mousePos, ImVec2 center, float R_in, float R_out)
{
    if (insideCircle(mousePos, center, R_out) && (!insideCircle(mousePos, center, R_in)))
        return true;
    else
        return false;
}

bool md::Canvas::intersectSegment(ImVec2 mousePos, ImVec2 p_A, ImVec2 p_B)
{
    // "far, close, inside" algorithm  Author : ericb  Sept 2019
    bool toReturn = false;

    float minX = ImMin(p_A.x, p_B.x);
    float maxX = ImMax(p_A.x, p_B.x);
    float minY = ImMin(p_A.y, p_B.y);
    float maxY = ImMax(p_A.y, p_B.y);

    if ((minX > mousePos.x) || ( maxX < mousePos.x))
        return false;

    if ((minY > mousePos.y) || ( maxY < mousePos.y))
        return false;

    if ( (mousePosIsPoint(mousePos, p_A) || (mousePosIsPoint(mousePos,p_B))) )
        return true;

    if ( ((p_A.x - mousePos.x)*( p_B.y - p_A.y) - (p_A.y - mousePos.y)*(p_B.x - p_A.x)) <= EPSILON )
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}

bool md::Canvas::mousePosIsPoint (ImVec2 mousePos, ImVec2 point)
{
    bool toReturn = false;

    if ((mousePos.x == point.x) && (mousePos.y == point.y))
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}


bool md::Canvas::insideSimpleArrow(ImVec2 mousePos, ImVector<ImVec2> polygon)
{
    bool toReturn = false;
    ImVector <ImVec2> arrowPolygon;
    arrowPolygon[0] = polygon[1];
    arrowPolygon[1] = polygon[2];
    arrowPolygon[2] = polygon[3];

    if (intersectSegment(mousePos, polygon[0], polygon[3]) || insidePolygon(mousePos, arrowPolygon))
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}

bool md::Canvas::insidePolygon(ImVec2 mousePos, ImVector<ImVec2> polygon)
{
    float minX = polygon[ 0 ].x;
    float maxX = polygon[ 0 ].x;
    float minY = polygon[ 0 ].y;
    float maxY = polygon[ 0 ].y;

    // http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
    // Working link : https://wrf.ecse.rpi.edu//Research/Short_Notes/pnpoly.html
    bool inside = false;

    for ( int i = 1 ; i < polygon.size() ; i++ )
    {
        ImVec2 q = polygon[ i ];

        minX = ImMin( q.x, minX );
        maxX = ImMax( q.x, maxX );
        minY = ImMin( q.y, minY );
        maxY = ImMax( q.y, maxY );
    }

    // outside of the whole bound
    if ( mousePos.x < minX || mousePos.x > maxX || mousePos.y < minY || mousePos.y > maxY )
        return false;

    for ( int i = 0, j = polygon.size() - 1 ; i < polygon.size() ; j = i++ )
    {

        //  s/>/>=/  + s/</<=/ to add the cases point is exactly a vertex of the polygon
        if ( ( polygon[ i ].y >= mousePos.y ) != ( polygon[ j ].y >= mousePos.y ) &&
             mousePos.x <= ( polygon[ j ].x - polygon[ i ].x ) * ( mousePos.y - polygon[ i ].y ) / ( polygon[ j ].y - polygon[ i ].y ) + polygon[ i ].x )
        {
            inside = !inside;
        }
    }
    return inside;
}

bool md::Canvas::insideFilledRectangle(ImVec2 mousePos, ImVector <ImVec2> Points)
{
    bool toReturn = false;

    if (fabs((Points[0].x - Points[1].x)*(Points[0].y - Points[1].y)) < EPSILON )
        return intersectSegment(mousePos, Points[0], Points[1]);

    if (insidePolygon(mousePos, Points))
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}


bool md::Canvas::intersectEmptyRectangle(ImVec2 mousePos, ImVector <ImVec2> Rect_ext, ImVector <ImVec2> Rect_int)
{
    bool toReturn = false;

    if (fabs((Rect_ext[0].x - Rect_ext[1].x)*(Rect_ext[0].y - Rect_ext[1].y)) < EPSILON )
        return intersectSegment(mousePos, Rect_ext[0], Rect_ext[1]);

    if (insidePolygon(mousePos, Rect_ext) && !(insidePolygon(mousePos, Rect_int)))

        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}

bool md::Canvas::insideEllipse(ImVec2 mousePos, float long_axis, float Xmin, float Xmax, float Ymin, float Ymax, ImVec2 F1, ImVec2 F2)
{
    bool toReturn = false;

    // yet a "far close" approach, to avoid burning too much of CPU
    if ( (mousePos.x < (Xmin)) || (mousePos.x > Xmax) || (mousePos.y > Ymax) || (mousePos.y < (Ymin)))
        return false;

    float F1P = sqrtf( ((mousePos.x - F1.x)*(mousePos.x - F1.x)) + ((mousePos.x - F1.x)*(mousePos.x - F1.x)) );
    float PF2 = sqrtf( ((mousePos.x - F1.x)*(mousePos.x - F1.x)) + ((mousePos.x - F1.x)*(mousePos.x - F1.x)) );

    if ((F1P + PF2) <= (2*long_axis))
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}


bool md::Canvas::intersectEmptyEllipse(ImVec2 mousePos, float long_axis, float Xmin, float Xmax, float Ymin, float Ymax, ImVec2 F1, ImVec2 F2, float thickness)
{
    bool toReturn = false;

    // yet a "far close" approach, to avoid burning too much of CPU
    if ( (mousePos.x < (Xmin)) || (mousePos.x > Xmax) || (mousePos.y > Ymax) || (mousePos.y < (Ymin)))
        return false;

    float F1P = sqrtf( ((mousePos.x - F1.x)*(mousePos.x - F1.x)) + ((mousePos.x - F1.x)*(mousePos.x - F1.x)) );
    float PF2 = sqrtf( ((mousePos.x - F1.x)*(mousePos.x - F1.x)) + ((mousePos.x - F1.x)*(mousePos.x - F1.x)) );

    if (((F1P + PF2) < (2*long_axis + thickness)) && !((F1P + PF2) < (2*long_axis - thickness)))
        toReturn = true;
    else
        toReturn = false;

    return toReturn;
}



void md::Canvas::setSelected(unsigned int position, bool isSelected)
{
    currentlyDrawnObjects[position].selected = isSelected;

    if (isSelected)
        currentActiveDrawnObjectIndex = (int)position;
    else
        currentActiveDrawnObjectIndex = -1;
}

// END CANVAS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
