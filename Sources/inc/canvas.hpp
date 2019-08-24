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

namespace md

{
    class Canvas
    {
        public:
            Canvas();
            ~Canvas();
            bool  init(ImVec2, ImVec2);
            void preview();
            bool addObject();

            bool adding_circle;
            bool adding_circle2;
            bool adding_preview1;
            bool adding_preview2;
            bool adding_rect;
            bool adding_rect2;

            bool alpha_half_preview;
            bool drag_and_drop;
            bool options_menu;

            ImVec4 bcol;
            // future use
            // ImVec4 ocol;

            int iconWidth;
            int iconHeight;
            int frame_padding;

            float    P1P4;
            float    radius_x;
            float    radius_y;
            float    rotation;
            float    arrowLength;
            float    arrowWidth;

            bool     selected;
            bool     hovered;
            bool     record;

            void           setMousePosValid(int, float);
            void           setCurrentActiveDrawnObject(DrawnObject *);
            DrawnObject *  getCurrentActiveDrawnObject(void);
            void           update(int selectedObject, ImVector <ImVec2> points, int, float);
            void           updateSelectedArea(ImVector <ImVec2> points, ImU32, float);
            int            draw();
            void           clean();
            bool           remove();

            bool           inside(ImVec2, ImVector<ImVec2>);

            void           catchPrimitivesPoints(void);
            int            show();

            void           loadCanvasObjectsIcons(void);
            void           createCanvasObjectsImagesTexIds(void);
            void           cleanCanvasObjectsImagesTexIds(void);

            GLuint         canvasObjectImageTexId[CANVAS_OBJECTS_TYPES_MAX];
            cv::Mat        canvasObjectImage[CANVAS_OBJECTS_TYPES_MAX];

            md::TextCanvas * mp_TextCanvas;

            ImVec2 *         p_topLeft;
            ImVec2 *         p_bottomRight;

            ImDrawList *     p_drawList;
            ImVec2           mouse_pos_in_image;
            ImVector <ImVec2> arrow_points;
            DrawnObject      aDrawnObject;
            DrawnObject *    p_aDrawnObject = & aDrawnObject;
            std::vector <DrawnObject> currentlyDrawnObjects;

        private:
            DrawnObject currentActiveObject;
    };
} /* namespace md */

#endif /* __CANVAS_HPP__ */
