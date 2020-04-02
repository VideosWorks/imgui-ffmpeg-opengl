/***************************************************************************************
 *    imgui_fade_in_out.cpp is a file frome miniDart project
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


#include "imgui_fade_in_out.hpp"

md::FadeInOut::FadeInOut()
{
}


md::FadeInOut::~FadeInOut()
{
}

void md::FadeInOut::init()
{
    up_action = true;
    ImGuiIO& io = ImGui::GetIO();
    (void)io; // makes the compiler happy
}

// TODO : buttons 

float md::FadeInOut::heartBeat(float up_duration, float down_duration, float min_hb, float max_hb)
{
    static float opacity = 1.0f;
    set_range(min_hb, max_hb);

    up_hb_step   = calculate_step(get_range(), up_duration);
    down_hb_step = calculate_step(get_range(), down_duration);

    if (up_hb_action == true)
    {
        opacity += up_hb_step;

        if (opacity >= max_hb)
        {
            opacity = max_hb;
            up_hb_action = false;
        }
    }
    else if ((up_hb_action == false))
    {
        opacity -= down_hb_step;

        if (opacity <= min_hb)
        {
            opacity = min_hb;
            up_hb_action = true;
        }
    }

    return sin(opacity);
}

float md::FadeInOut::fadeInOut(float up_duration, float down_duration, float min, float max)
{
    static float opacity_hb = 1.0f;
    set_range(min, max);

    up_step   = calculate_step(get_range(), up_duration);
    down_step = calculate_step(get_range(), down_duration);

    if (ImGui::IsWindowHovered (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) ||
                                ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)))
    {
        opacity_hb += up_step;

        if (opacity_hb > max)
        {
            opacity_hb = max;
            up_action = false;
        }
    }
    else
    {
        opacity_hb -= down_step;

        if (opacity_hb < min)
        {
            opacity_hb = min;
            up_action = true;
        }
    }
    return opacity_hb;
}


