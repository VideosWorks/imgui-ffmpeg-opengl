#include <draw_text.hpp>
/*
 * File draw_text.cpp, belongs to miniDart project
 * Copyright : Eric Bachard  / Created by Eric Bachard  2019 June 31
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

//#define CANVAS_DEBUG
// usefull link for string : https://en.cppreference.com/w/cpp/string/basic_string/size

extern const char * SystemFontFile[];

namespace md

{
    TextCanvas::TextCanvas(md::TEXT_Object * pTextObject)
    {
        this->pTextObject = pTextObject;
        init();
        pTextObject->hPosition = H_CENTER;
        pTextObject->vPosition = V_BOTTOM;
        pTextObject->b_text_auto_positioned = true;
        pTextObject->b_fillpoly = true;
        pTextObject->b_unlimited_annotation = true;
        pTextObject->duration =  DEFAULT_INCRUSTATION_DURATION;
        pTextObject->annotations_current_font = SystemFontFile[0];
        pTextObject->fontHeight = DEFAULT_FONTSIZE;
        pTextObject->frameCount = DEFAULT_FRAMERATE_FPS * pTextObject->duration;
    };

    TextCanvas::~TextCanvas()
    {
    }

    bool TextCanvas::init()
    {
        // initialize the text to be overlayed
        pTextObject->sText = "";

        int i = 0;
        for (i  = 0 ; i < CHARACTERS_PER_LINE_MAX ; i++)
        {
            pTextObject->ligne1[i] = '\0';
            pTextObject->ligne2[i] = '\0';
        }

        pTextObject->b_draw_text_baseline = false;
        pTextObject->b_draw_outline = false;

        // done in the Ctor, but those lines are kept here for a good reason
        //pTextObject->b_fillpoly = true;
        //pTextObject->b_text_centered = true;
        pTextObject->b_displayable = false;
        pTextObject->frameCount = ImGui::GetIO().Framerate * pTextObject->duration;

        // TODO : uggly. implement vector of lines + push/pop
        pTextObject->number_of_lines = 1;
        pTextObject->textSize = cv::Size(DEFAULT_FONTSIZE,DEFAULT_FONTSIZE);

        // not a good idea : once the user selected the right 
        // fontsize, why return to the default value (maybe not that good) ?
        //pTextObject->fontHeight = DEFAULT_FONT_SIZE;

        pTextObject->thickness = DEFAULT_THICKNESS;
        pTextObject->lineType  = DEFAULT_LINETYPE;
        pTextObject->lineStyle = DEFAULT_LINESTYLE;
        pTextObject->textOrg   = cv::Point(0,0);

        // TODO LATER : animation purpose !
        pTextObject->delta   = cv::Point(0,0);
        pTextObject->box_points[1][4] = { { cv::Point(0,0) } };
        pTextObject->box_points[0][0] = pTextObject->textOrg + cv::Point(0, DEFAULT_TEXT_INCRUSTATION_BASELINE);
        pTextObject->box_points[0][1] = pTextObject->textOrg + cv::Point(pTextObject->textSize.width, DEFAULT_TEXT_INCRUSTATION_BASELINE);
        pTextObject->box_points[0][2] = pTextObject->textOrg + cv::Point(pTextObject->textSize.width,  + pTextObject->textSize.height);
        pTextObject->box_points[0][3] = pTextObject->textOrg + cv::Point(0, + pTextObject->textSize.height);

        pTextObject->fontColor = cv::Scalar(255,255,255);  // REMINDER : cv::Mat is BGR(A) !
        pTextObject->outlineColor = cv::Scalar(0,255,0);   // green
        pTextObject->backgroundColor = cv::Scalar(0,0,0);  // black
        pTextObject->baselineColor = cv::Scalar(0,0,255);  // red

        // Mandatory, to initialize the textbox
        pTextObject->b_text_changed = true;

        return true;
    }


    bool TextCanvas::createTextLine(short int line)
    {
        return true;
    }


    short int TextCanvas::wrap_text_in_lines(void)
    {
        std::string word = "";
        word[0] = '\0';

        pTextObject->b_text_changed = true;

        short int toReturn = 0;
        bool line2_started = false;

        std::istringstream iss ((const char *) pTextObject->sText.c_str());
        short int string_size  = 0;
        short int string_size2 = 0;

        while (std::getline(iss, word, ' '))
        {
            if (line2_started == false)
            {
                if ((string_size + word.length()) <= (CHARACTERS_PER_LINE_MAX) - 1)
                {
                    strncat(pTextObject->ligne1, &word[0], word.length());
                    string_size += word.length(); // add the word

                    if ((string_size) < (CHARACTERS_PER_LINE_MAX) - 1)
                    {
                        // we can add a separator
                        strncat(pTextObject->ligne1, " "     , 1);
                        string_size += 1;             // + the space character
                    }
                }
                else
                {
                    pTextObject->number_of_lines++;

                    pTextObject->ligne2[0] = '\0';

                    strncat(pTextObject->ligne2, &word[0], word.length());
                    strncat(pTextObject->ligne2, " "     , 1);

                    string_size2 += word.length();
                    string_size2 += 1;

                    line2_started = true;
                }
            }
            else
            {
                if ((string_size2 + word.length()) <= (CHARACTERS_PER_LINE_MAX) - 1)
                {
                    strncat(pTextObject->ligne2, &word[0], word.length());
                    string_size2 += word.length();

                    if ((string_size2) < (CHARACTERS_PER_LINE_MAX - 1))
                    {
                        strncat(pTextObject->ligne2, " "     , 1);
                        string_size2++;
                    }
                }
                else
                    break;
            }
            word = { '\0' };
        }

        if (string_size > toReturn)
            toReturn = string_size;

        if (string_size2 > toReturn)
            toReturn = string_size2;

        return toReturn;
    }

    void TextCanvas::stopStringIncrustation(void)
    {
        init();
        pTextObject->b_displayable = false;
        pTextObject->frameCount = ImGui::GetIO().Framerate * DEFAULT_INCRUSTATION_DURATION;
    }

    void TextCanvas::clearString(void)
    {
        pTextObject->sText = "";

        short int i = 0;
        for (i  = 0 ; i < CHARACTERS_PER_LINE_MAX ; i++)
        {
            pTextObject->ligne1[i] = '\0';
            pTextObject->ligne2[i] = '\0';
        }
    }

    int TextCanvas::insertString(cv::Mat aFrame, md::TEXT_Object * pTextObject)
    {
        if (aFrame.empty())
        {
            std::cout << "No frame ..." << std::endl;
            return -1;
        }

        short int i = 0;

        for (i = 0; i < pTextObject->number_of_lines; i++)
        {
            pTextObject->ft2 = cv::freetype::createFreeType2();
            pTextObject->ft2->loadFontData(pTextObject->annotations_current_font, false);
            pTextObject->textSize = pTextObject->ft2->getTextSize(  pTextObject->ligne1,
                                                                    pTextObject->fontHeight,
                                                                    pTextObject->thickness,
                                                                   &(pTextObject->baseline));
            static int boxWidth  = DEFAULT_FONTSIZE;
            static int boxHeight = DEFAULT_FONTSIZE;

            // avoid using too much cpu
            if (pTextObject->b_text_changed == true)
            {
                boxWidth  = pTextObject->textSize.width  + (2*pTextObject->thickness);
                boxHeight = pTextObject->textSize.height + (2*pTextObject->thickness);

                if (pTextObject->b_text_auto_positioned == true)
                {
                    switch(pTextObject->hPosition)
                    {
                        case H_LEFT:
                            pTextObject->textOrg.x = 1;
                          break;

                        case H_CENTER:
                            pTextObject->textOrg.x = (aFrame.cols - boxWidth)/2;
                          break;

                        case H_RIGHT:
                            pTextObject->textOrg.x = aFrame.cols - boxWidth;

                        default:
                          break;
                    }

                    switch(pTextObject->vPosition)
                    {
                        case V_TOP:
                            pTextObject->textOrg.y = boxHeight;
                          break;

                        case V_MIDDLE:
                            pTextObject->textOrg.y = (aFrame.rows - boxHeight)/2;
                          break;

                        case V_BOTTOM:
                            pTextObject->textOrg.y = aFrame.rows - (GAP*boxHeight);
                        default:
                          break;
                    }
                }
            }

            //draw_everything();

            if (pTextObject->b_draw_outline == true)
            {
                cv::rectangle( aFrame,
                               pTextObject->delta + pTextObject->textOrg,
                               pTextObject->delta + pTextObject->textOrg + cv::Point(boxWidth, pTextObject->baseline + boxHeight + 2*pTextObject->thickness),
                               pTextObject->outlineColor,
                               1,
                               8);
            }

            const cv::Point* ppt[1] = { pTextObject->box_points[0] };
            int box_npt[] = { 4 };

            if (pTextObject->b_fillpoly == true)
            {
                // Create the box vertices : be carefull :INTEGERS ONLY !
                //
                //   P1 (0,baseline)                                     P2 (Size.width, baseline)
                //   ____________________________________________________   <--- baseline
                //   |                                                   |
                //   |                    THE TEXT                       |
                //   |                                                   |
                //   |___________________________________________________|
                //
                //   P4 (0, baseline + Size.height)                      P3 (Size.width, baseline + Size.height)

                if (pTextObject->b_text_changed == true)
                {
                    pTextObject->box_points[0][0] = pTextObject->delta + pTextObject->textOrg + cv::Point(0, pTextObject->baseline + pTextObject->thickness);
                    pTextObject->box_points[0][1] = pTextObject->delta + pTextObject->textOrg + cv::Point(pTextObject->textSize.width, pTextObject->baseline + pTextObject->thickness);
                    pTextObject->box_points[0][2] = pTextObject->delta + pTextObject->textOrg + cv::Point(pTextObject->textSize.width, pTextObject->baseline + pTextObject->fontHeight + 2*pTextObject->thickness);
                    pTextObject->box_points[0][3] = pTextObject->delta + pTextObject->textOrg + cv::Point(0, pTextObject->baseline + pTextObject->fontHeight + 2*pTextObject->thickness);
                }

                // set a colored box containing the text)
                cv::fillPoly( aFrame,
                              ppt,
                              box_npt,
                              1,
                              pTextObject->backgroundColor,
                              pTextObject->lineType );
            }

            pTextObject->ft2->putText(  aFrame,
                                        pTextObject->ligne1,
                                        pTextObject->delta + pTextObject->textOrg,      // to animate, create do textOrg(t)
                                        pTextObject->fontHeight,
                                        pTextObject->fontColor,
                                        -1,                        // pTextObject->thickness : -1 == CV doing the job (>0 values == other rendering)
                                        cv::LINE_AA,                // pTextObject->lineStyle
                                        false );

            if (pTextObject->b_draw_text_baseline == true)
            {
                // ... and the baseline. FIXME : before the font ?
                cv::line(aFrame,
                         pTextObject->delta + pTextObject->textOrg + cv::Point(0, pTextObject->baseline + pTextObject->thickness + pTextObject->textSize.height),
                         pTextObject->delta + pTextObject->textOrg + cv::Point(pTextObject->textSize.width, pTextObject->baseline + pTextObject->thickness + pTextObject->textSize.height),
                         pTextObject->baselineColor);
            }

            // Other tests, inspired from OpenCV documentation
            // pTextObject->ft2->putText(aFrame, "cv::freetype::putText()", cv::Point( 50, 50 ), 20, cv::Scalar( 255, 255, 255 ), -1, cv::LINE_AA, false );
            //pTextObject->ft2->putText(aFrame, (i == 0) ? pTextObject->ligne1 : pTextObject->ligne2, pTextObject->textOrg, pTextObject->fontHeight + i*pTextObject->textSize.height,
            //pTextObject->fontColor, pTextObject->thickness, pTextObject->lineStyle, false );  // BGR + (A=0) !!!

        }

        // calculate only once
        if (pTextObject->b_text_changed == true)
            pTextObject->b_text_changed = false;

        return EXIT_SUCCESS;
    }

    bool TextCanvas::textBoxHovered(cv::Mat aFrame, ImVec2 image_pos)
    {
        bool b_inside_rect = false;

        ImVec2 mouse_pos_in_image = ImVec2(ImGui::GetIO().MousePos.x - image_pos.x, ImGui::GetIO().MousePos.y - image_pos.y);
        ImVec2 topLeft    ((image_size.x*pTextObject->box_points[0][0].x)/aFrame.cols, ((image_size.y-58)*pTextObject->box_points[0][0].y)/aFrame.rows);
        ImVec2 bottomRight((image_size.x*pTextObject->box_points[0][2].x)/aFrame.cols, ((image_size.y-58)*pTextObject->box_points[0][2].y)/aFrame.rows);

        if ((mouse_pos_in_image.x < topLeft.x)||(mouse_pos_in_image.x > bottomRight.x)||
            (mouse_pos_in_image.y < topLeft.y)||(mouse_pos_in_image.y > bottomRight.y)   )
        {
            b_inside_rect = false;
        }
        else
        {
            b_inside_rect = true;
        }
        return b_inside_rect;
    }


    int TextCanvas::move(void)
    {
        pTextObject->delta.x += 2*ImGui::GetIO().MouseDelta.x;
        pTextObject->delta.y += 2*ImGui::GetIO().MouseDelta.y;
        pTextObject->b_text_changed = true;
        return 0;
    }
}
