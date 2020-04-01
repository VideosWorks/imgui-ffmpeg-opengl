 /* imgui_spin_input.h. This file belongs to miniDart project.
  * Copyright : https://github.com/aaslane
  * See : https://github.com/ocornut/imgui/issues/2649 for the full story
  */

#ifndef __IMGUI_SPIN_INPUT_H
#define __IMGUI_SPIN_INPUT_H

namespace ImGui
{
    IMGUI_API bool SpinScaler(const char* label, ImGuiDataType data_type, void* data_ptr, const void* step, const void* step_fast, const char* format, ImGuiInputTextFlags flags);
    IMGUI_API bool SpinInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool SpinFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool SpinDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
};

#endif  /* __IMGUI_SPIN_INPUT_H */

