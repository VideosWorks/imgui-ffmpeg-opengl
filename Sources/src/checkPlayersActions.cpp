/*
 * File checkPlayersActions.cpp, belongs to miniDart project
 * Copyright : Eric Bachard  / 2019 april 26th, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <imgui.h>
#include <application.hpp>

#ifdef BUILDING_FRENCH
#include "miniDart_fr.hpp"
#else
#include "miniDart_en-US.hpp"
#endif

extern "C" {
#include <statistics.hpp>
}

#ifdef TESTING_COLORS
extern int current_theme;
#endif

void checkPlayersActions(void)
{
    static int selected_event = -1;

    ImGui::BeginGroup();
    ImGui::PushItemWidth(100);
    ImVec2 size = ImGui::GetItemRectSize();

    ImGui::NewLine();
    ImGui::Dummy(ImVec2(0.0f, 40.0f));

    for (int n = 0; n < EVENTS_NUMBER ; n++)
    {
        ImGui::PushID(events_array[n]);
        ImGui::PushItemWidth(100);

#ifdef TESTING_COLORS
                    if (current_theme == DARK_THEME)
                    {
//                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3/7.0f, 1.0f, 1.0f));
//                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3/7.0f, 0.5f, 0.5f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(n/7.0f, 1.0f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(n/7.0f, 0.5f, 0.5f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
                    }
#endif
                    ImGui::SameLine(20);

                    if (ImGui::Button(events_array[n],ImVec2((size.x - 5.0) * 0.8f, 50.0f)))
                        selected_event = n;

                    ImGui::Dummy(ImVec2(0.0f, 10.0f));

                    if ( n < (EVENTS_NUMBER - 1))
                    ImGui::NewLine();

#ifdef TESTING_COLORS
                    if (current_theme == DARK_THEME)
                        ImGui::PopStyleColor(3);
#endif
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndGroup();


//                static int buts_pour = 0;
//                static int buts_contre = 0;


                switch (selected_event)
                {
                    case EVENT_GOAL:
                        ImGui::OpenPopup(PA_GOAL_SCORED);
                        selected_event = -1;
                    break;

                    case EVENT_ATTACK:
                        ImGui::OpenPopup(PA_ATTACK);
                        selected_event = -1;
                    break;

                    case EVENT_DEFENSE:
                        ImGui::OpenPopup(PA_DEFENSE);
                        selected_event = -1;
                    break;

                    case EVENT_COUNTER_ATTACK:
                        ImGui::OpenPopup(PA_COUNTER_ATTACK);
                        selected_event = -1;
                    break;

                    case EVENT_PROGRESS :
                        ImGui::OpenPopup(PA_BALL_RISE);
                        selected_event = -1;
                    break;

                    case EVENT_DEFENSIVE_RETURN :
                        ImGui::OpenPopup(PA_DEFENSIVE_WITHDRAWAL);
                        selected_event = -1;
                    break;

                    case EVENT_BEHAVIOR:
                        ImGui::OpenPopup(PA_BEHAVIOR);
                        selected_event = -1;
                    break;

                    case EVENT_UNDO:
                    break;

                    case EVENT_CLEAN_ALL:
                        ImGui::OpenPopup(PA_CLEAN_ALL_Q);
                        selected_event = -1;
                    break;

                    default:
                    break;
                }

                if (ImGui::BeginPopupModal(PA_GOAL_SCORED, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int goals_pro = 0;
                    static int goals_7m = 0;
                    static int clicked = -1;
                    // static int goals_against = 0;
                    static int i = 0;

                    ImGui::NewLine();

                    for (i = 0; i < GOAL_EVENTS_NUMBER; i++)
                    {
                        if (ImGui::Button(goal_events[i],ImVec2(300,0)))
                            clicked = i;

                        ImGui::NewLine();
                    }

                    switch(clicked)
                    {
                        case GOAL_ON_SHOOT:
                            goals_pro += 1;
                            ImGui::CloseCurrentPopup();
                          break;

                        case GOAL_AT_7M:
                            goals_7m += 1;
                            ImGui::CloseCurrentPopup();
                          break;

                        case GOAL_AGAINST:
                        case GOAL_AGAINST_AT_7M:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_GOAL:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }

                    // reset the value
                    clicked = -1;

                    ImGui::EndPopup();
                } // BeginPopupModal()

                if (ImGui::BeginPopupModal(PA_ATTACK, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int attack_clicked = -1;
                    static int j = 0;
                    ImGui::NewLine();

                    for (j = 0; j < ATTACK_EVENTS_NUMBER; j++)
                    {
                        if (ImGui::Button(attack_events[j],ImVec2(300,0)))
                            attack_clicked = j;

                        ImGui::NewLine();
                    }

                    switch(attack_clicked)
                    {
                        case DUEL_OFFENSIF_GAGNE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case BONNE_COURSE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case RECUPERE_LE_BALLON:
                        case PROVOQUE_UN_7M:
                        case PRISE_INTERVALLE:
                        case MAUVAISE_PASSE:
                        case MARCHER:
                        case PERTE_DE_BALLE:
                        case MAUVAIS_BLOC:
                        case PASSAGE_EN_FORCE:
                        case REPRISE_DE_DRIBBLE:
                        case EMPIETEMENT:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_ATTACK:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    attack_clicked = -1;

                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal(PA_DEFENSE, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int defense_clicked = -1;
                    static int k = 0;
                    ImGui::NewLine();

                    for (k = 0; k < DEFENSE_EVENTS_NUMBER; k++)
                    {
                        if (ImGui::Button(defense_events[k],ImVec2(300,0)))
                            defense_clicked = k;

                        ImGui::NewLine();
                    }

                    switch(defense_clicked)
                    {
                        case DUEL_DEFENSIF_GAGNE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CAUSE_UNE_PERTE_DE_BALLE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case INTERCEPTION:
                        case BONNE_ENTRAIDE:
                        case DISSUASION_EFFICACE:
                        case CONTRE_AYANT_AIDE_LE_GB:
                        case BIEN_SORTI:
                        case BIEN_GERE_LA_DISTANCE_DE_COMBAT:
                        case PROVOQUE_UN_PASSAGE_EN_FORCE:
                        case DUEL_DEFENSIF_PERDU:
                        case CONCEDE_UN_7M:
                        case MAUVAIS_CONTRE:
                        case DEFENSE_EN_ZONE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_DEFENSE:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    defense_clicked = -1;

                    ImGui::EndPopup();
                }


                if (ImGui::BeginPopupModal(PA_COUNTER_ATTACK, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int counter_attack_clicked = -1;
                    static int L = 0;
                    ImGui::NewLine();

                    for (L = 0; L < COUNTER_ATTACK_EVENTS_NUMBER; L++)
                    {
                        if (ImGui::Button(counter_attack_events[L],ImVec2(300,0)))
                            counter_attack_clicked = L;

                        ImGui::NewLine();
                    }

                    switch(counter_attack_clicked)
                    {
                        case BONNE_ANTICIPATION:
                            ImGui::CloseCurrentPopup();
                          break;

                        case BONNE_RELANCE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case BONNE_PASSE:
                        case MAUVAISE_ANTICIPATION:
                        case PERTE_DU_BALLON:
                        case MAUVAISE_RELANCE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_COUNTER_ATTACK:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    counter_attack_clicked = -1;

                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal(PA_BALL_RISE, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int ball_progression_clicked = -1;
                    static int M = 0;
                    ImGui::NewLine();

                    for (M = 0; M < COUNTER_ATTACK_EVENTS_NUMBER; M++)
                    {
                        if (ImGui::Button(counter_attack_events[M],ImVec2(300,0)))
                            ball_progression_clicked = M;

                        ImGui::NewLine();
                    }

                    switch(ball_progression_clicked)
                    {
                        case BON_ETAGEMENT:
                            ImGui::CloseCurrentPopup();
                          break;

                        case BON_ECARTEMENT:
                            ImGui::CloseCurrentPopup();
                          break;

                        case PASSE_DANS_LA_COURSE  :
                        case BONNE_DISPONIBILITE   :
                        case PERTE_DU_BALLON_SUR_MB:
                        case FAUTE_D_ATTAQUANT     :
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_BALL_PROGRESSION:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    ball_progression_clicked = -1;

                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal(PA_DEFENSIVE_WITHDRAWAL, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int defensive_return_clicked = -1;
                    static int MM = 0;
                    ImGui::NewLine();

                    for (MM = 0; MM < DEFENSIVE_RETURN_EVENTS_NUMBER; MM++)
                    {
                        if (ImGui::Button(defensive_return_events[MM],ImVec2(300,0)))
                            defensive_return_clicked = MM;

                        ImGui::NewLine();
                    }

                    switch(defensive_return_clicked)
                    {
                        case BON_HARCELEMENT:
                            ImGui::CloseCurrentPopup();
                          break;

                        case RECUPERATION_SUR_RD:
                            ImGui::CloseCurrentPopup();
                          break;

                        case BON_ETAGEMENT_RD  :
                        case PAS_REVENU_DEFENDRE   :
                        case PB_ORIENTATION:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_DEFENSIVE_RETURN:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    defensive_return_clicked = -1;

                    ImGui::EndPopup();
                }


                if (ImGui::BeginPopupModal(PA_BEHAVIOR, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int behavior_clicked = -1;
                    static int N = 0;
                    ImGui::NewLine();

                    for (N = 0; N < BEHAVIOR_EVENTS_NUMBER; N++)
                    {
                        if (ImGui::Button(behavior_events[N],ImVec2(300,0)))
                            behavior_clicked = N;

                        ImGui::NewLine();
                    }

                    switch(behavior_clicked)
                    {
                        case AVERTISSEMENT:
                            ImGui::CloseCurrentPopup();
                          break;

                        case _2_MIN:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CARTON_ROUGE:
                        case RAPPORT:
                        case BLESSURE:
                            ImGui::CloseCurrentPopup();
                          break;

                        case CANCEL_BEHAVIOR:
                            ImGui::CloseCurrentPopup();
                          break;

                        default:
                          break;
                    }
                    // reset the value
                    behavior_clicked = -1;

                    ImGui::EndPopup();
                }

                if (ImGui::BeginPopupModal(PA_CLEAN_ALL_Q, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text(PA_ALL_ACTIONS_WILL_BE_CANCELED_STRING);

                    if (ImGui::Button(OK_BUTTON, ImVec2(120,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::SameLine();

                    if (ImGui::Button(CANCEL_BUTTON, ImVec2(120,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                }
                // end checkPlayersActions()
}

