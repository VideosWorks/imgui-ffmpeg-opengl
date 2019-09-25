/*
 *  File text_incrustation.cpp, belongs to miniDart project
 *  Copyright Eric Bachard, 2019 july 31st
 *  This file is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <imgui.h>
#include <imgui_stdlib.h>
#include "imgui_helpers.h"
#include <application.hpp>
#include "text_incrustation.hpp"
#include "fonts.hpp"

#include <opencv2/opencv.hpp>

#include <string>

#ifdef BUILDING_FRENCH
#include "miniDart_fr.hpp"
#else
#include "miniDart_en-US.hpp"
#endif


#define COMMENT_SIZE_MAX 1024

// FIXME : HiDpi immediately breaks that
#define X_MAX 1440
#define Y_MAX 1080

#define MAX_FONT_BASELINE 30
#define MIN_FONT_SIZE   6
#define MAX_FONT_SIZE  80

#define NO_TEST

//#ifndef CANVAS_DEBUG
//#define CANVAS_DEBUG
//#endif

extern bool b_inserting_text;
static bool toReturn = true;

bool text_in_video_helper(md::TextCanvas * pTextCanvas)
{
    static bool b_refresh = true;
    static ImGuiComboFlags combo_flags = ImGuiComboFlags_PopupAlignLeft;
    static const char* font_item_current = font_items[0];

    ImGui::AlignTextToFramePadding();
    ImGui::Text(USED_FONT); ImGui::SameLine();

    ImGui::PushItemWidth(150);

    if (ImGui::BeginCombo("##Police truetype en cours d'utilisation", font_item_current, combo_flags)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(font_items); n++)
        {
            bool is_selected = (font_item_current == font_items[n]);

            if (ImGui::Selectable(font_items[n], is_selected))
            {
                font_item_current = font_items[n];
                pTextCanvas->pTextObject->annotations_current_font = SystemFontFile[n];
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
        b_refresh = true ;
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Choix de la taille de la police de caractère

    static const int default_ttf_font_size = 24;
    static       int         ttf_font_size = default_ttf_font_size;
    static       int     old_ttf_font_size = ttf_font_size;

    ImGui::Text(FONTSIZE);

    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    ImGui::SliderInt("###do not display", &ttf_font_size, MIN_FONT_SIZE,MAX_FONT_SIZE);

    if (old_ttf_font_size != ttf_font_size)
    {
        pTextCanvas->pTextObject->fontHeight = ttf_font_size;
        old_ttf_font_size = ttf_font_size;
        b_refresh = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Durée du commentaire incrusté dans la vidéo
    static const int default_comment_duration = 10;
    static       int         comment_duration = default_comment_duration;
    static       int     old_comment_duration = comment_duration;

    ImGui::Checkbox(UNDEFINED_DURATION_NOTATION, &pTextCanvas->pTextObject->b_unlimited_annotation);

    if (!pTextCanvas->pTextObject->b_unlimited_annotation)
    {
        ImGui::SameLine();

        ImGui::Text(ANNOTATION_DURATION_IN_S);
        ImGui::SameLine();
        ImGui::PushItemWidth(150);

        ImGui::SliderInt("##do not display", &comment_duration, 3,20);

        if (old_comment_duration != comment_duration)
        {
            pTextCanvas->pTextObject->duration = comment_duration;
            pTextCanvas->pTextObject->frameCount = ImGui::GetIO().Framerate * pTextCanvas->pTextObject->duration;
            old_comment_duration = comment_duration;
            b_refresh = true;
        }
        ImGui::PopItemWidth();
    }

    // Dessiner le fond coloré du rectangle ? // défaut : OUI
    static const bool b_default_draw_box  = true;
    static       bool         b_draw_box  = b_default_draw_box;
    static       bool     old_b_draw_box  = b_draw_box;

    ImGui::SameLine();
    ImGui::Checkbox(DRAW_A_BACKGROUND_UNDER_THE_TEXT_Q, &b_draw_box);

    if (old_b_draw_box != b_draw_box)
    {
        pTextCanvas->pTextObject->b_fillpoly = b_draw_box;
        old_b_draw_box = b_draw_box;
        b_refresh = true;
    }

    if (b_refresh == true)
    {
        pTextCanvas->pTextObject->b_text_changed = true;
        b_refresh = false;
    }

    // Positionnement du commentaire dans la vidéo
    ImGui::PushID("#automatic positioning");
    ImGui::Checkbox(AUTOMATIC_POSITIONING_OF_THE_TEXT_Q, &pTextCanvas->pTextObject->b_text_auto_positioned);

    if (ImGui::IsItemClicked(0))
        b_refresh = true;

    ImGui::PopID();

    if (!pTextCanvas->pTextObject->b_text_auto_positioned)
    {
        static int default_pos_x = pTextCanvas->pTextObject->textOrg.x;
        static int         pos_x = default_pos_x;
        static int     old_pos_x = pos_x;

        static int default_pos_y = pTextCanvas->pTextObject->textOrg.y;
        static int         pos_y = default_pos_y;
        static int     old_pos_y = pos_y;

        ImGui::SameLine();

        ImGui::Text(HORIZONTAL_POSITIONING);
        ImGui::SameLine();

        ImGui::PushItemWidth(150);
        ImGui::SliderInt("##do_not_display me", &pos_x, 0, X_MAX);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::Text(VERTICAL_POSITIONING);
        ImGui::SameLine();

        ImGui::PushItemWidth(150);
        ImGui::SliderInt("##do_not_display me me", &pos_y, 0, Y_MAX);
        ImGui::PopItemWidth();

        if (old_pos_x != pos_x)
        {
            pTextCanvas->pTextObject->textOrg.x = pos_x;
            old_pos_x = pos_x;
            b_refresh = true;
        }

        if (old_pos_y != pos_y)
        {
            pTextCanvas->pTextObject->textOrg.y = pos_y;
            old_pos_y = pos_y;
            b_refresh = true;
        }
    }
    else
    {
        // if not, the text position will become  uncontrolable
        pTextCanvas->pTextObject->delta = cv::Point(0.0f, 0.0f);
        // bottom centered is the default
        static const int default_hPosition = H_CENTER;
        static int               hPosition = default_hPosition;
        static int           old_hPosition = hPosition;

        static const int default_vPosition = V_BOTTOM;
        static int               vPosition = default_vPosition;
        static int           old_vPosition = vPosition;

        ImGui::SameLine();
        ImGui::Text(H_POSITION);
        ImGui::SameLine();
        ImGui::RadioButton(ON_THE_LEFT , &hPosition, 0); ImGui::SameLine();
        ImGui::RadioButton(CENTERED, &hPosition, 1); ImGui::SameLine();
        ImGui::RadioButton(ON_THE_RIGHT , &hPosition, 2);

        ImGui::SameLine();
        ImGui::Text(V_POSITION);
        ImGui::SameLine();
        ImGui::RadioButton(ON_TOP  , &vPosition, 0); ImGui::SameLine();
        ImGui::RadioButton(IN_THE_MIDDLE, &vPosition, 1); ImGui::SameLine();
        ImGui::RadioButton(AT_THE_BOTTOM , &vPosition, 2);

        if ( old_hPosition != hPosition )
        {
            pTextCanvas->pTextObject->hPosition = hPosition;
            old_hPosition = hPosition;
            b_refresh = true;
        }

        if ( old_vPosition != vPosition )
        {
            pTextCanvas->pTextObject->vPosition = vPosition;
            old_vPosition = vPosition;
            b_refresh = true;
        }
    }


    // Boite de saisie
    static char comment[COMMENT_SIZE_MAX] = "";

    char buf_comment[2*COMMENT_SIZE_MAX]; sprintf(buf_comment, TEXT_AND_STRING, comment);
    // FIXME
    ImGui::PushID(COMMENT);
    HelpMarker(USE_RIGHT_CLICK_TO_TYPE_A_TEXT_OR_CHANGE_IT); ImGui:: SameLine(); ImGui::Button(buf_comment);

    if (ImGui::BeginPopupContextItem(COMMENT))
    {
        b_inserting_text = true;
////!!
        ImGui::PushItemWidth(300);
        ImGui::InputText(INSERT, comment, IM_ARRAYSIZE(comment));
////!!

        ImGui::PopItemWidth();

        if (ImGui::Button(VALIDATE_BUTTON))
        {
            std::cout << THE_MESSAGE_CONTAINS <<  comment << std::endl;
            b_inserting_text = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();

    bool b_can_insert_text = (strlen(comment) > 0) ? true : false;

//    ImGui::Text(" (clic droit pour modifier)");

    static float b = 1.0f; // 0.6f
    static float c = 0.5f; // 1.0f
    static int i = 3;

//    ImGui::SameLine(); 

    if (ImGui::Button(SUPPRESS_THE_TEXT))
    {
        comment[0] = '\0';
        buf_comment[0] = '\0';
        pTextCanvas->stopStringIncrustation();
        toReturn = false;
    }

    if (b_can_insert_text)
    {
        ImGui::SameLine();

        ImGui::PushID(INSERT);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/8.0f, b, b));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, b, b));
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, c, c));
        ImGui::Button(INSERT);

        if (ImGui::IsItemClicked(0))
        {

            // first, clean the previous incrustation, but keep its properties
            pTextCanvas->clearString();

            pTextCanvas->pTextObject->frameCount = ImGui::GetIO().Framerate * pTextCanvas->pTextObject->duration;

            // insert the text in the video, then empty the string
            // annotation::insert( comment, fontsize, and so on)
            if (pTextCanvas->pTextObject)
                pTextCanvas->pTextObject->sText = comment;


            if (pTextCanvas->wrap_text_in_lines() <= 0)
                pTextCanvas->pTextObject->b_displayable = false;

#ifdef CANVAS_DEBUG
            std::cout << "Le message : \"" << comment << "\" a été incrusté dans la vidéo" << std::endl;
            std::cout << ".. et pTextCanvas->pTextObject->sText contient : " << pTextCanvas->pTextObject->sText << std::endl;
            std::cout << "\nLa structure pTextCanvas->pTextObject contient : " << std::endl;

            // initialize the text to be overlayed
            std::cout << "annotations_current_font :             " << pTextCanvas->pTextObject->annotations_current_font << std::endl;
            std::cout << "sText             " << pTextCanvas->pTextObject->sText.c_str() << std::endl;
            std::cout << "ligne1            " << pTextCanvas->pTextObject->ligne1 << std::endl;
            std::cout << "ligne2            " << pTextCanvas->pTextObject->ligne2 << std::endl;
            std::cout << "duration          " << pTextCanvas->pTextObject->duration << std::endl;
            std::cout << "b_draw_text_baseline  " << pTextCanvas->pTextObject->b_draw_text_baseline << std::endl;
            std::cout << "b_draw_outline    " << pTextCanvas->pTextObject->b_draw_outline << std::endl;
            std::cout << "b_fillPoly        " << pTextCanvas->pTextObject->b_fillpoly << std::endl;
            std::cout << "frameCount        " << pTextCanvas->pTextObject->frameCount << std::endl;
            std::cout << "number_of_lines   " << pTextCanvas->pTextObject->number_of_lines << std::endl;
            std::cout << "textSize          " << pTextCanvas->pTextObject->textSize << std::endl;
            std::cout << "fontHeight        " << pTextCanvas->pTextObject->fontHeight << std::endl;
            std::cout << "thickness         " << pTextCanvas->pTextObject->thickness << std::endl;
            std::cout << "lineType          " << pTextCanvas->pTextObject->lineType << std::endl;
            std::cout << "lineStyle         " << pTextCanvas->pTextObject->lineStyle << std::endl;
            std::cout << "baseline          " << pTextCanvas->pTextObject->baseline << std::endl;
            std::cout << "textOrg           " << pTextCanvas->pTextObject->textOrg << std::endl;
            std::cout << "delta             " <<pTextCanvas->pTextObject->delta  << std::endl;
            std::cout << "box_points        " << pTextCanvas->pTextObject->box_points[1][4] << std::endl;
            std::cout << "fontColor         " << pTextCanvas->pTextObject->fontColor << std::endl;
            std::cout << "outlineColor      " << pTextCanvas->pTextObject->outlineColor << std::endl;
            std::cout << "backgroundColor   " << pTextCanvas->pTextObject->backgroundColor << std::endl;
            std::cout << "baselineColor     " << pTextCanvas->pTextObject->baselineColor << std::endl;
            std::cout << "box_points[1][4]  " << pTextCanvas->pTextObject->box_points[1][4] << std::endl;
            std::cout << "box_points[1][0]  " << pTextCanvas->pTextObject->box_points[0][0] << std::endl;
            std::cout << "box_points[1][1]  " << pTextCanvas->pTextObject->box_points[0][1] << std::endl;
            std::cout << "box_points[1][2]  " << pTextCanvas->pTextObject->box_points[0][2] << std::endl;
            std::cout << "box_points[1][3]  " << pTextCanvas->pTextObject->box_points[0][3] << std::endl;
#endif
            // we can recalculate everything to be drawn
            b_refresh = true;
            // clean up
            // is it that important to clean the buffer ? e.g. avoid to retype the comment can be usefull, and so on
            //            comment[0] = '\0';
            toReturn = true;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    ImGui::SameLine();

    if (ImGui::Button(ADVANCED_PARAMETERS))
        ImGui::OpenPopup(ADDITIONAL_PARAMETERS);

    if (ImGui::BeginPopupModal(ADDITIONAL_PARAMETERS, NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // paramètres avancés (qu'on peut réinitialiser)


///////////////////////////////////////////////////////////////////////////////

        // Couleur de la police de caractères  // blanche par défaut
        static const int font_default_red_comp   = 255;
        static const int font_default_green_comp = 255;
        static const int font_default_blue_comp  = 255;

        static       int font_red_comp   = font_default_red_comp;
        static       int font_green_comp = font_default_green_comp;
        static       int font_blue_comp  = font_default_blue_comp;

        static       int old_font_red_comp   = font_red_comp;
        static       int old_font_green_comp = font_green_comp;
        static       int old_font_blue_comp  = font_blue_comp;

        ImGui::Text(FONT_COLOR);
        ImGui::SliderInt(RED_FONT_COMP, &font_red_comp, 0,255);
        ImGui::SliderInt(GREEN_FONT_COMP, &font_green_comp, 0,255);
        ImGui::SliderInt(BLUE_FONT_COMP, &font_blue_comp, 0,255);

        if ((old_font_red_comp != font_red_comp) || (old_font_green_comp != font_green_comp) || (old_font_blue_comp != font_blue_comp))
        {
            pTextCanvas->pTextObject->fontColor = cv::Scalar(font_blue_comp, font_green_comp, font_red_comp);
            old_font_red_comp   = font_red_comp;
            old_font_green_comp   = font_green_comp;
            old_font_blue_comp   = font_blue_comp;

            b_refresh = true;
        }

///////////////////////////////////////////////////////////////////////////////

        // Ligne de base de la police de caractères (pour l'ajuster au cadre)

        static const int default_ttf_font_baseline = DEFAULT_TEXT_INCRUSTATION_BASELINE;
        static       int         ttf_font_baseline = default_ttf_font_baseline;
        static       int     old_ttf_font_baseline = ttf_font_baseline;

        ImGui::Text(FONT_BASELINE);
        ImGui::SliderInt("###baseline value", &ttf_font_baseline, 0, Y_MAX);

        if (old_ttf_font_baseline != ttf_font_baseline)
        {
            pTextCanvas->pTextObject->baseline = ttf_font_baseline;
            old_ttf_font_baseline = ttf_font_baseline;
            b_refresh = true;
        }

///////////////////////////////////////////////////////////////////////////////

        // dessiner la ligne de base du texte ?  // défaut : NON
        static const bool b_default_draw_text_baseline = false;
        static       bool         b_draw_text_baseline = b_default_draw_text_baseline;
        static       bool     old_b_draw_text_baseline = b_draw_text_baseline;


        ImGui::Checkbox(DRAW_THE_BASELINE_Q, &b_draw_text_baseline);

        if (old_b_draw_text_baseline != b_draw_text_baseline)
        {
            pTextCanvas->pTextObject->b_draw_text_baseline = b_draw_text_baseline;
            old_b_draw_text_baseline = b_draw_text_baseline;

            b_refresh = true;
        }

///////////////////////////////////////////////////////////////////////////////

        // Couleur de la ligne de base du texte
        static const int text_baseline_default_red_comp   = 0;
        static const int text_baseline_default_green_comp = 0;
        static const int text_baseline_default_blue_comp  = 0;

        static       int text_baseline_red_comp   = text_baseline_default_red_comp;
        static       int text_baseline_green_comp = text_baseline_default_green_comp;
        static       int text_baseline_blue_comp  = text_baseline_default_blue_comp;

        static       int old_text_baseline_red_comp   = text_baseline_red_comp;
        static       int old_text_baseline_green_comp = text_baseline_green_comp;
        static       int old_text_baseline_blue_comp  = text_baseline_blue_comp;

        if (b_draw_text_baseline)
        {
            // Choix du cadre contenant le texte  // noir par défaut
            ImGui::Text(BOUNDING_BOX_COLOR);
            ImGui::SliderInt(BB_RED_COMP_COLOR, &text_baseline_red_comp, 0,255);
            ImGui::SliderInt(BB_GREEN_COMP_COLOR, &text_baseline_green_comp, 0,255);
            ImGui::SliderInt(BB_BLUE_COMP_COLOR, &text_baseline_blue_comp, 0,255);
        }

        if ((old_text_baseline_red_comp != text_baseline_red_comp) || (old_text_baseline_green_comp != text_baseline_green_comp) || (old_text_baseline_blue_comp != text_baseline_blue_comp))
        {
            pTextCanvas->pTextObject->baselineColor = cv::Scalar(text_baseline_blue_comp, text_baseline_green_comp, text_baseline_red_comp);
            old_text_baseline_red_comp   = text_baseline_red_comp;
            old_text_baseline_green_comp   = text_baseline_green_comp;
            old_text_baseline_blue_comp   = text_baseline_blue_comp;
            b_refresh = true;
        }

///////////////////////////////////////////////////////////////////////////////

        // Couleur du fond
        static const int box_background_default_red_comp   = 0;
        static const int box_background_default_green_comp = 0;
        static const int box_background_default_blue_comp  = 0;

        static       int box_background_red_comp   = box_background_default_red_comp;
        static       int box_background_green_comp = box_background_default_green_comp;
        static       int box_background_blue_comp  = box_background_default_blue_comp;

        static       int old_box_background_red_comp   = box_background_red_comp;
        static       int old_box_background_green_comp = box_background_green_comp;
        static       int old_box_background_blue_comp  = box_background_blue_comp;

        if (b_draw_box)
        {
            // Couleur de remplissage du fond  // noir par défaut
            ImGui::Text(BACKGROUND_INCRUSTATION_COLOR);
            ImGui::SliderInt(BACKGROUND_RED_COMP_COLOR, &box_background_red_comp, 0,255);
            ImGui::SliderInt(BACKGROUND_GREEN_COMP_COLOR, &box_background_green_comp, 0,255);
            ImGui::SliderInt(BACKGROUND_BLUE_COMP_COLOR, &box_background_blue_comp, 0,255);
        }

        if ((old_box_background_red_comp != box_background_red_comp) || (old_box_background_green_comp != box_background_green_comp) || (old_box_background_blue_comp != box_background_blue_comp))
        {
            pTextCanvas->pTextObject->backgroundColor = cv::Scalar(box_background_blue_comp, box_background_green_comp, box_background_red_comp);
            old_box_background_red_comp   = box_background_red_comp;
            old_box_background_green_comp   = box_background_green_comp;
            old_box_background_blue_comp   = box_background_blue_comp;
            b_refresh = true;
        }

///////////////////////////////////////////////////////////////////////////////

        // dessiner le tour du rectangle coloré ? // défaut : NON
        static const bool b_default_draw_box_outline  = false;
        static       bool         b_draw_box_outline  = b_default_draw_box_outline;
        static       bool     old_b_draw_box_outline  = b_draw_box_outline;

        // Couleur du cadre autour de la police
        static const int box_outline_default_green_comp = 0;
        static const int box_outline_default_blue_comp  = 0;
        static const int box_outline_default_red_comp   = 0;

        static       int box_outline_green_comp = box_outline_default_green_comp;
        static       int box_outline_blue_comp  = box_outline_default_blue_comp;
        static       int box_outline_red_comp   = box_outline_default_red_comp;

        static       int old_box_outline_red_comp   = box_outline_red_comp;
        static       int old_box_outline_green_comp = box_outline_green_comp;
        static       int old_box_outline_blue_comp  = box_outline_blue_comp;

        ImGui::Checkbox(DRAW_THE_FRAME_BORDER_Q, &b_draw_box_outline);

        if (b_draw_box_outline)
        {
            // Couleur du bord du cadre contenant le texte  // noir par défaut
            ImGui::Text(OUTLINE_COLOR);
            ImGui::SliderInt(OUTLINE_RED_COMP_COLOR, &box_outline_red_comp, 0,255);
            ImGui::SliderInt(OUTLINE_GREEN_COMP_COLOR, &box_outline_green_comp, 0,255);
            ImGui::SliderInt(OUTLINE_BLUE_COMP_COLOR, &box_outline_blue_comp, 0,255);
        }

        if (old_b_draw_box_outline != b_draw_box_outline)
        {
            pTextCanvas->pTextObject->b_draw_outline = b_draw_box_outline;
            old_b_draw_box_outline = b_draw_box_outline;

            b_refresh = true;

        }

        if ((old_box_outline_red_comp != box_outline_red_comp) || (old_box_outline_green_comp != box_outline_green_comp) || (old_box_outline_blue_comp != box_outline_blue_comp))
        {
            pTextCanvas->pTextObject->outlineColor = cv::Scalar(box_outline_blue_comp, box_outline_green_comp, box_outline_red_comp);
            old_box_outline_red_comp   = box_outline_red_comp;
            old_box_outline_green_comp   = box_outline_green_comp;
            old_box_outline_blue_comp   = box_outline_blue_comp;

            b_refresh = true;
        }
///////////////////////////////////////////////////////////////////////////////


        //  ImGui::Dummy(ImVec2(150.0, 0.0)); ImGui::SameLine();
        if (ImGui::Button(BACK_TO_DEFAULT_VALUES))
        {
            comment_duration = default_comment_duration;

            ttf_font_size = default_ttf_font_size;

            font_red_comp = font_default_red_comp;
            font_green_comp = font_default_green_comp;
            font_blue_comp = font_default_blue_comp;

            pTextCanvas->pTextObject->b_text_auto_positioned = true;
            pTextCanvas->pTextObject->hPosition = H_CENTER;
            pTextCanvas->pTextObject->vPosition = V_BOTTOM;

            b_draw_box           = b_default_draw_box;
            b_draw_box_outline   = b_default_draw_box_outline;
            b_draw_text_baseline = b_default_draw_text_baseline;

            box_background_red_comp = box_background_default_red_comp;
            box_background_green_comp = box_background_default_green_comp;
            box_background_blue_comp = box_background_default_blue_comp;

            box_outline_red_comp = box_outline_default_red_comp;
            box_outline_green_comp = box_outline_default_green_comp;
            box_outline_blue_comp = box_outline_default_blue_comp;

            text_baseline_red_comp   = text_baseline_default_red_comp;
            text_baseline_green_comp = text_baseline_default_green_comp;
            text_baseline_blue_comp  = text_baseline_default_blue_comp;

            pTextCanvas->pTextObject->baseline = DEFAULT_TEXT_INCRUSTATION_BASELINE;
            pTextCanvas->pTextObject->delta = cv::Point(0.0f, 0.0f);

            b_refresh = true;
        }

        ImGui::SameLine(); ImGui::Dummy(ImVec2(50.0, 0.0)); ImGui::SameLine();


        if (b_refresh == true)
            pTextCanvas->pTextObject->b_text_changed = true;

        // ImGui::SetItemDefaultFocus();

        if (ImGui::Button(ANNOTATIONS_OK_BUTTON, ImVec2(120,0)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    } // BeginPopupModal()

    return toReturn;
}
