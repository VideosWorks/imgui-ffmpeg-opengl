/***************************************************************************************
 *    imgui_fade_in_out.hpp is a file frome miniDart project
 *    Author : Eric Bachard  / monday april 1st 2020, 20:5133:03 (UTC+0200)
 *    eric.bachard@free.fr
 *    2018 december 30th
 *
 *                                  MIT License
 *                                  -----------
 *                               eric.bachard@free.fr
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * **************************************************************************************/
#ifndef  _CUSTOM_FADE_IN_OUT_HPP
#define __CUSTOM_FADE_IN_OUT_HPP

#include <imgui.h>

namespace md
{
    class FadeInOut
    {
        public:
            FadeInOut();
            ~FadeInOut();

            void init();
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
