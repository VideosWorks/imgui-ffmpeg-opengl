/* **************************************************************************************
 *    Author: Eric Bachard
 *    eric.bachard@free.fr
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


#ifndef IMGUI_THEMES_HPP
#define IMGUI_THEMES_HPP

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <string>
#include <iostream>
#include <vector>
#include "imgui.h"
#include "imgui_internal.h"

// Extra Math Helpers (Set the proper define below in imgui_internal.h)
#ifdef IMGUI_DEFINE_MATH_OPERATORS
//static inline ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z, lhs.w+rhs.w); }
//static inline ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.w*rhs.w); }
static inline ImVec4 operator/(const ImVec4& lhs, const ImVec4& rhs)            { return ImVec4(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z, lhs.w/rhs.w); }

static inline ImVec4& operator+=(ImVec4& lhs, const ImVec4& rhs)            { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; lhs.w += rhs.w; return lhs;}
static inline ImVec4& operator-=(ImVec4& lhs, const ImVec4& rhs)            { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; lhs.w -= rhs.w; return lhs;}
static inline ImVec4& operator*=(ImVec4& lhs, const ImVec4& rhs)            { lhs.x *= rhs.x; lhs.y *= rhs.y; lhs.z *= rhs.z; lhs.w *= rhs.w; return lhs;}
static inline ImVec4& operator/=(ImVec4& lhs, const ImVec4& rhs)            { lhs.x /= rhs.x; lhs.y /= rhs.y; lhs.z /= rhs.z; lhs.w /= rhs.w; return lhs;}
#endif

namespace ImGui
{
    // New style inspired from Traditionnal Green (gtk+) on Linux + to be improved !
    IMGUI_API void  StyleColorsLightGreen(ImGuiStyle* dst = NULL); // the default style in miniDart software

    // New style inspired from Windows theme to be improved !
    IMGUI_API void  StyleColorsWindows(ImGuiStyle* dst = NULL);


} // namespace ImGui
#endif //IMGUI_TABS_HPP
