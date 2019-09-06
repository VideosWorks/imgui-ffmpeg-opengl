/* 
 * file canvas.hpp, belongs to miniDart project
 * Copyright Eric Bachard 2019 August 19th   17:55:00   CEST
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include "application.hpp"
#include "canvas_objects.hpp"
#include "draw_text.hpp"
#include <math.h>
#include "imgui_helpers.h"
#include <list>

#define DEFAULT_OBJECT_THICKNESS  2.5f
#define DEFAULT_ICON_WIDTH          32
#define DEFAULT_ICON_HEIGHT         32
#define DEFAULT_FRAME_PADDING        4

#define EPSILON  1e-5

namespace md

{

class Canvas
{
    public:
        Canvas();
        ~Canvas();

        bool           init();
        void           update();

        bool           addObject();
        bool           adding_circle;
        bool           adding_circle2;
        bool           adding_preview1;
        bool           adding_preview2;
        bool           adding_rect;
        bool           adding_rect2;

        ImVec4  bcol;
        // future use
        // ImVec4 ocol;

        int            iconWidth;
        int            iconHeight;
        int            frame_padding;

        void           setMousePosValid(int, float);
        void           preview(int selectedObject, ImU32, int, float, float);
        void           updateSelectedArea(ImVector <ImVec2> points, ImU32, float);
        // FIXME, usefull
        //void           setSelectedAreaPoints(ImVec2, ImVec2);

        int            draw();
        void           clean();
        bool           remove(unsigned int);
        bool           moveObjectTo(unsigned int, int);

        bool           insideCircle(ImVec2, ImVec2, float);
        bool           intersectEmptyCircle(ImVec2, ImVec2, float, float);

        //             intersection when : (x == A) OR (x === B)  OR ((vec A)*(vec B) < EPSILON  AND  ( x bettwen xB and xC) AND ( y  between yB and yC))
        bool           intersectSegment(ImVec2 /* mousePos */, ImVec2 /* Point_A */ , ImVec2  /* Point_B */);
        bool           mousePosIsPoint(ImVec2 /* mousePos */, ImVec2 /* aGivenPoint */);

        bool           insideSimpleArrow(ImVec2, ImVector<ImVec2>);
        bool           insidePolygon(ImVec2, ImVector<ImVec2>);

        //             only horizontal rectangle are drawn
        bool           insideFilledRectangle(ImVec2, ImVector<ImVec2>);
        bool           intersectEmptyRectangle(ImVec2, ImVector<ImVec2>, ImVector<ImVec2>);

        bool           insideEllipse(ImVec2, float, float, float, float, float, ImVec2, ImVec2); // includes empty ellipse
        bool           intersectEmptyEllipse(ImVec2, float, float, float, float, float, ImVec2, ImVec2, float /* thickness */);

        bool           insideCurve(ImVec2, ImVector<ImVec2>);
        bool           insideArrow(ImVec2, ImVector<ImVec2>);

        void           setSelected(unsigned int, bool);
        inline unsigned int   getCurrentActiveDrawnObjectIndex(void) { return currentActiveDrawnObjectIndex ; }

        void           catchPrimitivesPoints(void);
        int            show();

        void           loadCanvasObjectsIcons(void);
        void           createCanvasObjectsImagesTexIds(void);
        void           cleanCanvasObjectsImagesTexIds(void);

        GLuint         canvasObjectImageTexId[CANVAS_OBJECTS_TYPES_MAX];
        cv::Mat        canvasObjectImage[CANVAS_OBJECTS_TYPES_MAX];

        md::TextCanvas * mp_TextCanvas;

        ImVec2           topLeft;
        ImVec2           bottomRight;

        ImDrawList *     p_drawList;
        ImVec2           mouse_pos_in_image;
        ImVector <ImVec2> arrow_points;
        ImVector <ImVec2> zoom_area_points;
        DrawnObject      aDrawnObject;

        std::vector <DrawnObject> currentlyDrawnObjects;

    private:
        unsigned int    currentActiveDrawnObjectIndex;

};

} /* namespace md */

#endif /* __CANVAS_HPP__ */
