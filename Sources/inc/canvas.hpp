/* 
 * file canvas.hpp, belongs to miniDart project
 * Copyright Eric Bachard 2019 August 19th   17:55:00   CEST
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __CANVAS_HPP__
#define __canvCANVAS_HPP__

#include "canvas_object.hpp"
#include <list>

class Canvas
{
  public:
    Canvas();
    ~Canvas();
    init();
    preview();
    addObject();

    bool adding_circle;
    bool adding_circle2;
    bool adding_preview1;
    bool adding_preview2;
    bool adding_rect;
    bool adding_rect2;

    float P1P4;
    float arrowLength;
    float arrowWidth;

    setMousePosValid();
    update();
    draw();
    clean();
    remove();

    ImVec2 mouse_pos_in_image;
    ImVector <ImVec2> arrow_points;
    DrawnObject  aDrawnObject;
    p_aDrawnObject = & aDrawnObject;
    std::vector <DrawnObject> currentlyDrawnObjects;

    // unused
    // ImVector <ImVec2> objectPoints;

    // Currently : one canvas per tab
    // Could be:
    // - one DrawnObjects in delayTab,
    // - one in GK folow up and so on
    // Use a list ? 
    // Something like :
    // std::list <DrawnObjects>  DrawnObjectsList;   ??
}

#endif /* __CANVAS_HPP__ */
