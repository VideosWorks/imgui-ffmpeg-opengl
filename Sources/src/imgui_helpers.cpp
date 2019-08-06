/*
 * Fichier imgui_helpers.cpp pour le projet miniDart
 * Auteur : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */
#include <imgui_helpers.h>
#include <cmath>

void reorder_points( ImVec2 * p_topLeft, ImVec2 * p_bottomRight)
{
    float temp = 0.0f;
    if ((p_topLeft->x != p_bottomRight->x) && (p_topLeft->y!=p_bottomRight->y))
    {
        // Case 1 : // 1 x
                    // x 2 => rectangle is correct : do nothing
        // Case 2 : // x 1 => rectangle drawn for top right to bottom left : needs x permutation on top and bottom means swap(topLeft.x, bottomRight.x)
                    // 2 x
        if ((p_topLeft->x > p_bottomRight->x) && (p_topLeft->y < p_bottomRight->y) )
        {
            temp = p_topLeft->x;
            p_topLeft->x = p_bottomRight->x;
            p_bottomRight->x = temp;
        }
        // Case 3 : // x 2 : rectangle drawn from bottom left to top right : needs y permutation on top and bottom, means swap (topLeft.y, bottomRight.y)
                    // 1 x 
        else if ((p_topLeft->y > p_bottomRight->y) && (p_topLeft->x < p_bottomRight->x) )
        {
            temp = p_topLeft->y;
            p_topLeft->y = p_bottomRight->y;
            p_bottomRight->y = temp;
        }
        // Case 4 : // 2 x
                    // x 1
        // => the rectangle is drawn from permutation of everything (x and y),
        // means we need to swap (topLeft.x, bottomRight.x) AND swap(topLeft.y, bottomRight.y)

        else if ((p_topLeft->y > p_bottomRight->y) && (p_topLeft->x > p_bottomRight->x))
        {
            int value = p_topLeft->x;
            p_topLeft->x = p_bottomRight->x;
            p_bottomRight->x = value;
            value = p_topLeft->y;
            p_topLeft->y = p_bottomRight->y;
            p_bottomRight->y = value;
        }
    }
}

double sqr(double val)
{
    return val*val;
}

double length(ImVec2 * p_p1, ImVec2* p_p2 )
{
    return sqrt(sqr(p_p2->x - p_p1->x) + sqr(p_p2->y - p_p1->y));
}


