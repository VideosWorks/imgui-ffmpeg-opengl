/* 
 * file magnifier.cpp, belongs to miniDart project
 * Copyright Eric Bachard 2019 August 5th   00:012:00   CEST
 * License : GPL v2
 * See: http://www.gnu.org/licenses/gpl-2.0.html
 */


#include "magnifier.hpp"


md::Magnifier::Magnifier(md::Canvas * p_Canvas)
{
    pCanvas = p_Canvas;
    p_adding_rect = &p_Canvas->adding_rect;
}

md::Magnifier::~Magnifier()
{
}

int md::Magnifier::start(int w, float ratio)
{
    m_w = w;
    m_ratio = ratio;
    // current_delayTab_drawing_task = DRAWING_ZOOMED_AREA;
    return DRAWING_ZOOMED_AREA;
}

bool md::Magnifier::update(bool * pb_zoom_available, bool b_paused, GLuint backgndTextureID)
{
    textureID = backgndTextureID;

    static float aZoom = 1.0f;
    float W = pCanvas->bottomRight.x - pCanvas->topLeft.x;
    float H = pCanvas->bottomRight.y - pCanvas->topLeft.y;

    // minimal dimensions for the zoom (else, we can't close it)
    if (W < 260.0f)
        W = ZOOM_WIDTH_MIN;

    if (H < 260.0f)
        H = ZOOM_HEIGHT_MIN;

    static ImVec2 size(W,H);

    ImVec2 center((pCanvas->bottomRight.x + pCanvas->topLeft.x)/2.0f + LEFT_IMAGE_BORDER,
                  (pCanvas->bottomRight.y + pCanvas->topLeft.y)/2.0f);

    ImVec2 delta = ImVec2( W/(2.0f*aZoom), H/(2.0f*aZoom));
    ImVec2 uv0   = ImVec2((center.x - delta.x)/m_w, (center.y - delta.y)/(m_w*m_ratio));
    ImVec2 uv1   = ImVec2((center.x + delta.x)/m_w, (center.y + delta.y)/(m_w*m_ratio));

    if ((*pb_zoom_available == true ) && !b_paused)
    {
        ImGui::SetItemAllowOverlap();
        ImGuiWindowFlags zoomWinFlags = 0;
        zoomWinFlags |= ImGuiWindowFlags_NoSavedSettings;

        // FIXME
        //ImGui::SetCursorPos(pCanvas->topLeft);
        //ImGui::InvisibleButton("##dummy", size);

        //if (ImGui::IsItemActive() && ImGui::GetIO().KeyCtrl && ImGui::IsMouseDragging(0,0.0f))
        //    move();

        if (p_adding_rect)
            ImGui::SetNextWindowSize(ImVec2(2*(W + 18.0f), 2*(H + 70.0f)));

        ImGui::Begin( ICON_FA_SEARCH ZOOM_WINDOW);
        {
            // resizable window
            ImGui::Image(reinterpret_cast<void * >(textureID),
                         ImVec2(ImGui::GetWindowWidth() - 18.0f, ImGui::GetWindowHeight() - 70.0f),
                         uv0,uv1, ImColor(255,255,255,255), ImColor(255,255,255,255));

            ImGui::Text(ZOOM_VALUE); ImGui::SameLine();
            ImGui::PushItemWidth(125);
            ImGui::SliderFloat("##ghost", &aZoom, 1.0f, 4.0f, "##%.1f");
            ImGui::PopItemWidth(); ImGui::SameLine();


            if (ImGui::Button(CLOSE_BUTTON))
            {
                // in this case, draw nothing in the lense
                ImGui::SetWindowSize(ImVec2(ZOOM_WIDTH_MIN, ZOOM_HEIGHT_MIN));
                *pb_zoom_available = false;
            }
        }
        ImGui::End();
    }
    return true;
}


void md::Magnifier::move(void)
{
    pCanvas->topLeft.x += ImGui::GetIO().MouseDelta.x;
    pCanvas->topLeft.y += ImGui::GetIO().MouseDelta.y;
    pCanvas->bottomRight.x += ImGui::GetIO().MouseDelta.x;
    pCanvas->bottomRight.y += ImGui::GetIO().MouseDelta.y;
}

