/*
 * imgui_fade_in_out.cpp is a file frome miniDart project
 * Author : Eric Bachard  / monday april 1st 2020, 20:5133:03 (UTC+0200)
 * This file is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "imgui_fade_in_out.hpp"

template<typename T> static inline T ImMin(T lhs, T rhs)   { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs)   { return lhs >= rhs ? lhs : rhs; }


md::FadeInOut::FadeInOut()
{
}


md::FadeInOut::~FadeInOut()
{
}

void md::FadeInOut::init()
{
    up_action = true;
    opacity   = 0.0f;
}

void md::FadeInOut::set_range(float min, float max)
{
    range = max - min;
}


float md::FadeInOut::calculate_step (float f_range, float f_duration )
{
    ImGuiIO& io = ImGui::GetIO();

    return ((f_range * io.DeltaTime) / f_duration);
}

float md::FadeInOut::fadeInOut(float up_duration, float down_duration, float min, float max)
{
    up_action = true;
    set_range(min, max);

    up_step   = calculate_step(get_range(), up_duration);
    down_step = calculate_step(get_range(), down_duration);

    if (ImGui::IsWindowHovered (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) ||
                                ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)))
    {
        if (up_action == true)
        {
            if (opacity <= max)
                opacity += up_step;
            else
            {
                opacity = max;
                up_action = false;
            }
        }
    }
    else
    {
        if (opacity >= min)
            opacity -= down_step;
        else
        {
            opacity = min;
            up_action = false;
        }
    }
    return opacity;
}

