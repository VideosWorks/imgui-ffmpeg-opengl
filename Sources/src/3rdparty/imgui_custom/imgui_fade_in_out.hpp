/*
 * imgui_fade_in_out.hpp is a file frome miniDart project
 * Author : Eric Bachard  / monday april 1st 2020, 20:5133:03 (UTC+0200)
 * This file is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef  _CUSTOM_FADE_IN_OUT_HPP
#define __CUSTOM_FADE_IN_OUT_HPP

#include <imgui.h>


//template<typename T> static inline T ImMin(T lhs, T rhs)   { return lhs < rhs ? lhs : rhs; }
//template<typename T> static inline T ImMax(T lhs, T rhs)   { return lhs >= rhs ? lhs : rhs; }

namespace md
{
    class FadeInOut
    {
        public:
            FadeInOut();
            ~FadeInOut();

            void init();
            //void set_opacity( float value ) { opacity = value };
            //float get_opacity() { return opacity ; }
            void set_range (float min, float max);
            float get_range(void) { return range ;}

            float calculate_step(float, float);
            float fadeInOut(float, float, float, float);

            float up_step;
            float down_step;
            bool  up_action;
            float opacity;
            float min;
            float max;

        private:
            float range;

    };
} /* namespace md */


#endif /* __CUSTOM_FADE_IN_OUT_HPP */
