/*
 * file magnifier.cpp, belongs to miniDart project
 * Copyright Eric Bachard 2019 August 5th   00:012:00   CEST
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __MAGNIFIER_HPP
#define __MAGNIFIER_HPP

#include "imgui.h"
#include "application.hpp"
#include "canvas.hpp"
#include "canvas_objects.hpp"
#include "FontAwesome.h"

#ifdef BUILDING_FRENCH
#include "miniDart_fr.hpp"
#else
#include "miniDart_en-US.hpp"
#endif


namespace md
{
    class Magnifier
    {
        public:
            Magnifier(md::Canvas *);
            ~Magnifier();

            md::Canvas * pCanvas;
            bool *       p_adding_rect;
            GLuint       textureID;
            int          m_w;
            float        m_ratio;

            int          start( int /* w */, float /* ratio */);
            bool         update(bool *, bool, GLuint);
            void         move(void);
    };
}    /*  namespace md */


#endif /* __MAGNIFIER_HPP  */
