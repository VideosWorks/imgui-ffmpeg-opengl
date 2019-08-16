/*
 * Fichier d'en tête miniDart_en-US.hpp pour le projet miniDart
 * Auteur : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef miniDart_LOCALIZATION
#define miniDart_LOCALIZATION

#define APPLICATION_VERSION_STRING "miniDart 0.9.6 2019/08/16 Ablette Agile"

#define UNIX_PATH_SEPARATOR                  "/"
#define WINDOWS_PATH_SEPARATOR               "\\"
#define OUTPUT_FILENAME_PREFIX               "MyVideo"

#define APPLICATION_NAME                     "miniDart"
#define TIME_DISPLAY                         "Time : "
#define RECORD_DISPLAY                       "REC"
#define DELAY_DISPLAY                        "Delay : "

#define FILE_OPEN_MENU_ENTRY                 " File"
#define EDITION_MENU_ENTRY                   " Édition"
#define MODE_MENU_ENTRY                      "Mode"
#define UNDO_MENU_ENTRY                      "Undo"
#define REDO_MENU_ENTRY                      "Redo"
#define CUT_MENU_ENTRY                       "Cut"
#define COPY_MENU_ENTRY                      "Copy"
#define PASTE_MENU_ENTRY                     "Paste"


#define UNDO_MENU_ENTRY_SHORTCUT             "Ctrl+z"
#define REDO_MENU_ENTRY_SHORTCUT             "Ctrl+y"
#define CUT_MENU_ENTRY_SHORTCUT              "Ctrl+x"
#define COPY_MENU_ENTRY_SHORTCUT             "Ctrl+c"
#define PASTE_MENU_ENTRY_SHORTCUT            "Ctrl+v"


#define TABBAR_NAME                          "#miniDart tab test"
#define DELAY_TAB_NAME                       " Delay workshop"


#define ZOOM_WINDOW                          " Lense"
#define ZOOM_VALUE                           "Zoom "

#define CLOSE_BUTTON                         "Close"
#define CHRONO_RESET_BUTTON                  "Reset"
#define CHRONO_ACTIVITY_TIME                 "Activity time %2d : %2d"
#define CHRONO_TOTAL_ELLAPSED_TIME           "Total ellapsed time %2d : %2d"

#define RENDERING_TIME_FPS_DISPLAY           "Rendering done in %.3f ms/frame (%.1f FPS)"
#define DISPLAY_PAUSED_CHECKBOX              "  Display paused"
#define RECORD_VIDEO_CHECKBOX                "  Record a video"

#define CURRENT_WORKING_DIRECTORY_BUTTON     "  Current working directory"
#define CURRENT_WORKING_DIRECTORY_POPUP      "Current working directory"

#define HELP_CHOOSE_WORKING_REC_DIRECTORY    "Before to be able to record a video, a directory must be defined, who will \ncontain the recorded movies, and where you can write big size files\n"

#define OK_BUTTON                            "OK"
#define ADVANCED_RECORDING_PARAMETERS_BUTTON "Advanced recording parameters"

#define VIDEO_CONTAINER                      "Container : "

/* NO NEED TO TRANSLATE */

#define CODEC_FPS_SIZE_POPUP                 "##codec_fps_size"
#define MP4_CONTAINER_RADIOBUTTON            ".mp4"
#define AVI_CONTAINER_RADIOBUTTON            ".avi"
#define MKV_CONTAINER_RADIOBUTTON            ".mkv"

#define DIVX_CODEC_RADIOBUTTON               "DIVX"
#define DIV3_CODEC_RADIOBUTTON               "DIV3"
#define MJPG_CODEC_RADIOBUTTON               "MJPG"
#define FMP4_CODEC_RADIOBUTTON               "FMP4"
#define DX50_CODEC_RADIOBUTTON               "DX50"
#define FLV1_CODEC_RADIOBUTTON               "FLV1"
#define U263_CODEC_RADIOBUTTON               "U263"
#define H264_CODEC_RADIOBUTTON               "H264"

/* PART 2 */

#define FRAMES_PER_SECOND_CHOICE             "Frames per second : "

#define RECORDED_FRAMESIZE_BUTTON           "Recorded frames size : "
#define RECORDED_FRAMESIZE_POPUP            "dimensions_out_image"
#define RESTORE_DEFAULT_VALUES_BUTTON       "Restore default values"

#define CLOSE_BUTTON_FRAMESIZE_POPUP        "Close"
#define DELAYED_DISPLAY_CHECKBOX            "  Delayed display"
#define DELAY_IN_SECONDS                    "Delay (in s):"

#define CHRONOMETER_CHECKBOX                "  Chronometer"
#define CHRONOMETER_RECORD_CHECKBOX         "  Record the time      "
#define CHRONOMETER_ACTIVITY_TIME_CHECKBOX  "  Display activity time"
#define CHRONOMETER_TOTAL_TIME_CHECKBOX     "  Display total time   "

#define INPUT_SOURCENAME_CHOICE             "Source : "
#define INPUT_SOURCENAME_USB_WEBCAM         " USB Webcam"
#define INPUT_SOURCENAME_VIDEO              " Video file"
#define INPUT_SOURCENAME_IP_WEBCAM          " Network (IP webcam)"
#define INPUT_SOURCENAME_CELL_PHONE         " Cell phone"

#define INPUT_SOURCENAME_CHOICE             "Source : "
#define INPUT_SOURCENAME_CHOICE_POPUP       "source_choice"

#define SELECTED_WEBCAM_BUTTON              "Selected webcam"
#define WEBCAM1                             " webcam 1"
#define WEBCAM2                             " webcam 2"
#define WEBCAM3                             " webcam 3"

#define WEBCAM_CHOICE_POPUP                 "webcam_choice"

/* part 3 */

#define IMAGE_SIZE_IN_PIXELS_BUTTON          "Image size in pixels : "
#define IMAGE_FORMAT_POPUP                   "format_image"
#define RESET_THE_WEBCAM_BUTTON              "Reset the source"
#define ZOOM_ENABLE_CHECKBOX                 " Enable the lense"
#define SELECTED_AREA_COLOR                  "Selected area color"
#define NETWORK_FLOW                         "#flux_reseau"
#define FLOW_URL                             "URL of the flow : %s"
#define ENTER_URL                            "enter the URL:"
#define OK_BUTTON2                           "Ok"
#define OPEN_A_VIDEO_BUTTON                  "Open a video"
#define CLOSE_THE_VIDEO_BUTTON               "Close the video"
#define CLOSE_THE_FLOW_BUTTON               "Close the rtsp flow"


/* part 4 */ 

#define ANNOTATIONS_CHILD_WINDOW             "Annotations"
#define ANALYSIS_WORKSHOP                    "  Analysis workshop"
#define WORKING_DIRECTORY_BUTTON             "Select a working directory"

#define IMAGE_CURRENTLY_DISPLAYED           "Currently displayed image number"
#define CANVAS_GROUP                        "Canvas"
#define IMAGES_FORMAT_IN_PIXELS         "Images format (in pixels) : "
#define IMAGES_FORMAT_POPUP             "#dimensions"

#define FIGURES_TAB                         "  Figures"


#endif /* miniDart_LOCALIZATION */

#define OPEN_A_CLIP_BUTTON                  "Open a video"
#define CLOSE_THE_CLIP_BUTTON               "CLose the clip"

/*  part 5 */

#define THEME_MENU_ENTRY                   "Theme"

#define TRADITIONAL_GREEN_THEME_MENU_ENTRY "  Traditional green"

#define DARK_THEME_MENU_ENTRY              "  Dark"

#define CLASSIC_THEME_MENU_ENTRY           "  Classic"

#define LIGHT_BLUE_THEME_MENU_ENTRY        "  Light blue"

#define WINDOWS_THEME_MENU_ENTRY           "  Windows"

#define IMAGES_SOURCE_CHOICE_MENU_ENTRY    "Images source"

#define ABOUT_BUTTON                       " About ..."

#define ABOUT_MINIDART                     "About miniDart"

#define VIDEO_SOURCE_SELECTION             "Source selection"

#define PRESENTATION_STRING1               "Video analysis software for collective or individual sports coaches \n(handball e.g.), written in C++"

#define LINUX_PRESENTATION_STRING2         "Version : 0.9.6  (Linux Intel x86_64 )"

#define WINDOWS_PRESENTATION_STRING2       "Version : 0.9.6  (Windows 7+ x86_64 )"

#define LINK_PRESENTATION_STRING3          "Download : https://framagit.org/ericb/miniDart"

#define AUTHOR_PRESENTATION_STRING4        "Author of the software : Eric Bachard © 2016-2019"

#define LICENSE_PRESENTATION_STRING5       "License GPL v2+"

#define OK_BUTTON                          "OK"

#define VIDEO_CODEC_TEXT                   "Video codec : "

#define SEE_RECORDABLE_AUDIO_DEVICES       "See recordable audio devices "

#define SOURCES_OF_IMAGES                  "Source of images :"

#define USE_ANOTHER_IMAGE_SOURCE_BUTTON    "Use another images source"

#define SELECT_ANOTHER_IMAGES_SOURCE       "Select another images source"

#define ACTIVATE_THE_USB_WEBCAM            "Activate the USB webcam"

#define AUTOMATIC_FOCUS                    "Auto focusing "

#define FOCUS_VALUE                        "Focus value"

#define CURRENT_FOCUS_VALUE                "Focus actuel"

#define AUTOMATIC_LIGHT_EXPOSITION         "Automatic light exposure "

#define LIGHT_EXPOSITION_VALUE             "Light exposure"

#define CURRENT_LIGHT_EXPOSITION_VALUE     "Current light exposure "

#define PAN_VALUE                          "Pan value   "

#define CURRENT_PAN_VALUE                  "Current Pan value : "

#define TILT_VALUE                         "Tilt value  "

#define CURRENT_TILT_VALUE                 "Current tilt value  "

#define CURRENT_ZOOM_VALUE                 "Current zoom value : "

#define RESET_PAN_AND_TILT_VALUE_BUTTON    "Reset pan and tilt"

#define AVAILABLE_CODECS_BUTTON            "Available codecs"

#define OTHER_WEBCAM_PARAMETERS_BUTTON     "Other webcam parameters"

#define ENTERED_WEBCAM_IP_VALUE            "Input IP webcam : "

#define START_THE_DIFFUSION_BUTTON         "Start the diffusion"

#define START_BUTTON                       "Start"

#define OUTLINE_THICKNESS_VALUE            "Line thickness"

#define DEFINE_A_RECTANGULAR_AREA_IN_THE_IMAGE "Define a rectangular area in the image"

#define IMAGES_WIDTH_HEIGHT_RATIO          "Images width/height ratio"

#define CHANGE_THE_IMAGES_WIDTH_HEIGHT_RATIO "Change the displayed images width/height ratio    "

#define _16_9_SCREEN                          " 16/9 ratio (e.g. cinéma screen, default) "

#define _16_10_MACBOOK_PRO_EXEMPLE            " 16/10 ratio (example :  macbook pro)"

#define _4_3_CLASSICAL_TV_SCREEN              " 4/3 (classical TV screen) "

#define _5_4_SCREEN                           " 5/4 "

#define CHANGE_IMAGES_ORIENTATION_BUTTON     "Change images orientation    "

#define IMAGES_ROTATION                      "Images rotation"

#define ZERO_ROTATION                          " No images rotation  "

#define CLOCKWISE_90_DEGREES_ROTATION        " 90 degrees, clockwise) "

#define _180_DEGREES_ROTATION                 " 180 degrees "

#define TRIGO_90_DEGREES_ROTATION            " 90 degrees, counter clockwise "

#define HORIZONTAL_FLIP                      "Horizontal flip"

#define APPLIED_BEFORE_THE_ROTATION          "(applied before the rotation)"

#define ANNOTATE_THE_VIDEO                   "Annotate the vidéo"

#define FILLED_RECTANGLE_SOON_INTEGRATED     "Annotation of filled rectangle type will be in a next version"

#define EMPTY_RECTANGLE_SOON_INTEGRATED      "Annotation of empty rectangle type will be in a next version"

#define FILLED_ELLIPSE_SOON_INTEGRATED       "Annotation of filled ellipse type will be in a next version"

#define EMPTY_ELLIPSE_SOON_INTEGRATED        "Annotation of empty ellipse type will be in a next version"

#define RANDOM_LINE_SOON_INTEGRATED          "Annotation of random line type will be in a next version"

#define SIMPLE_ARROW_SOON_INTEGRATED         "Annotation of arrow type will be in a next version"

#define SIMPLE_LINE_SOON_INTEGRATED          "Annotation of simple line type will be in a next version"

#define FRAMES_PER_SECOND                    "frames per second"

#define CANVAS_STRING_LINE01                 "Fonctionnal :"

#define CANVAS_STRING_LINE02                 "Open a clip from 30 s to 1 min"

#define CANVAS_STRING_LINE03                 "Maximum advised : 1 min "

#define CANVAS_STRING_LINE04                 "Read at a speed between"

#define CANVAS_STRING_LINE05                 "5 and 60 frames per second"

#define CANVAS_STRING_LINE06                 "Make an image stop"

#define CANVAS_STRING_LINE07                 "Return to the begining"

#define CANVAS_STRING_LINE08                 "Fast forward (~ 10 images)"

#define CANVAS_STRING_LINE09                 "Fast backward (de 10 images)"

#define CANVAS_STRING_LINE10                 "Implementation in progress :"

#define CANVAS_STRING_LINE11                 "- extract an image"

#define CANVAS_STRING_LINE12                 "- draw in the image"

#define CANVAS_STRING_LINE13                 "- insert text"

#define CANVAS_STRING_LINE14                 "- define a working directory"

#define CANVAS_STRING_LINE15                 "- choose the recording format"

#define CANVAS_STRING_LINE16                 "- record as a \nde video of a given duration"

#define CANVAS_STRING_LINE17                 "- choose the duration"

#define CANVAS_STRING_LINE18                 "- add keywords in an wml file\nallowing to sort and retrieve targeted images"

#define TEAM_A                               " Team A"

#define PLAYERS_LIST                          "Players list"

#define ALL_ACTIONS                           "All actions"

#define GAME_ACTIONS_OF_ALL_PLAYERS           "All match, all players actions"

#define PLAYER_01                             "Player 1"

#define ALL_PLAYER_1_ACTIONS                  "Actions attributed to player 1"

#define PLAYER_02                             "Player 2"

#define ALL_PLAYER_2_ACTIONS                  "Actions attributed to player 2"

#define PLAYER_03                             "Player 3"

#define ALL_PLAYER_3_ACTIONS                  "Actions attributed to player 3"

#define PLAYER_04                             "Player 4"

#define ALL_PLAYER_4_ACTIONS                  "Actions attributed to player 4"

#define PLAYER_05                             "Player 5"

#define ALL_PLAYER_5_ACTIONS                  "Actions attributed to player 5"

#define PLAYER_06                             "Player 6"

#define ALL_PLAYER_6_ACTIONS                  "Actions attributed to player 6"

#define PLAYER_07                             "Player 7"

#define ALL_PLAYER_7_ACTIONS                  "Actions attributed to player 7"

#define PLAYER_08                             "Player 8"

#define ALL_PLAYER_8_ACTIONS                  "Actions attributed to player 8"

#define PLAYER_09                             "Player 9"

#define ALL_PLAYER_9_ACTIONS                  "Actions attributed to player 9"

#define PLAYER_10                             "Player 10"

#define ALL_PLAYER_10_ACTIONS                  "Actions attributed to player 10"

#define PLAYER_11                             "Player 11"

#define ALL_PLAYER_11_ACTIONS                  "Actions attributed to player 11"

#define PLAYER_12                             "Player 12"

#define ALL_PLAYER_12_ACTIONS                  "Actions attributed to player 12"

#define GK_FOLLOW_UP_TAB_ITEM                  "  GK follow up "

#define STATISTICS_AND_GK_FOLLOW_UP            "Statistics and GK follow up"

#define ENTER_THE_NAME                         "Enter the name:"

#define ENTER_THE_CLUB                         "Enter the club:"

#define CHAMPIONSHIP                           "Championship"

#define OPPONENT                               "Opponent : "

#define ENTER_THE_DATE                         "Enter the date :"

#define PRACTICE_RADIOBUTTON                   "Practice"

#define OFFICIAL_MATCH_RADIOBUTTON             "Official match"

#define DELETE_ALL_BUTTON                      "Delete all"

#define ALL_SHOOTS_WILL_BE_CANCELED_STRING     "All shoots will be definitely deleted"

#define CANCEL_BUTTON                          "Cancel"

#define UNDO_BUTTON                            "Undo"

#define FIRST_HALF_TIME                        "  1st half time"

#define SECOND_HALF_TIME                       "  2nd half time"

#define GAME_PERIOD_BUTTON                     "Game périod : "

#define RIGHT_CLICK_TIP_FOR_ARROWS_DRAWING     "Left-click + drag and release to add a shoot,\nRight click to modify the parameters. 7m : click first the 7m button, then add the shoot"

#define ARROWS_PARAMETERS                      "Parameters"

#define PROVIDE_ADDITIONAL_INFORMATION_ABOUT_THE_SHOOT  "Enter additional information about the shoot"

#define DISPLAY_SHOOTERS_NUMBERS               "Display the shooters numbers"

#define ARROWS_POPUP_MENU                      "MenuFleches"

#define DRAW_THE_ARROWS                        "Draw the arrows"

#define ARROWS_LENGTH                          "Arrows length"

#define ARROWS_WIDTH                           "Arrows width"

#define ENTER_A_NUMBER_BETWEEN_1_AND_99        "Enter the player number (from 1 to 99)"

#define SHOOT_CONDITIONS                       "Shoot conditions : "

#define SHOOT_AT_6M                            "Shoot at 6m"

#define WING_SHOOT                             "Wing shoot"

#define FAR_SHOOT                              "Far shoor"

#define PIVOT_SHOOT                            "Pivot shoot"

#define BACK_SHOOT                             "Back shoot"

#define CENTRAL_SHOOT                          "Central shoot"

#define COUNTER_ATTACK_FIGHT                   "Fight on counter attack"

#define BETWEEN_BEANS_SHOOT                    "Shoot between beans"

#define MASKED_SHOOT                           "Masked shoot"

#define BOUNDING_SHOOT                         "Bounding shoot"

#define SIDE_SHOOT                             "Side shoot"

#define IN_THE_RUN_SHOOT                       "Running shoot"

#define OVER_HEAD_SHOOT                        "Lob"

#define COUNTERED_SHOOT                        "Countered shoot"

#define STRESSED_SHOOT                         "Stressed shoot"

#define SHOOT_RESULT                           "Shoot result : "

#define GOAL_RADIOBUTTON                       "   Goal    "

#define GK_STOP_RADIOBUTTON                    "  GK stop  "

#define BORDER_SHOOT_RADIOBUTTON               "   Pole shoot   "

#define OUT_SHOOT_RADIOBUTTON                  "   Outside "

#define _7M_BUTTON                              "    7m    "

#define DRAW_THE_ARROWS                        "Draw the arrows"

#define DISPLAY_THE_SHOOTERS_NUMBER            "Display the shooters numbers"

#define GK_FIGURES_STRING_01                   "GK statistics"

#define GK_FIGURES_STRING_02                   "Number of shoots : %d"

#define GK_FIGURES_STRING_03                   "Number of 7m : %d"

#define GK_FIGURES_STRING_04                   "Number of shoots on target %d"

#define GK_FIGURES_STRING_05                   "Number of goals taken : %d"

#define GK_FIGURES_STRING_06                   "Number of goasl taken over 7m : %d"

#define GK_FIGURES_STRING_07                   "Number of GK stops : %d"

#define GK_FIGURES_STRING_08                   "Number of GK stops over 7m : %d"

#define GK_FIGURES_STRING_09                   "Percentage of GK stops over 7m : %.0f %%"

#define GK_FIGURES_STRING_10                   "Percentage of success of GK on 7m : %.0f %%"

#define GK_FIGURES_STRING_11                   "Percentage of stops of GK : %.0f %%"

#define GK_FIGURES_STRING_12                   "Percentage of success of GK : %.0f %%"

#define EFFICIENCY_AREA                        "Efficiency area"

#define GK_EFFICIENCY_AREA                     "GK efficiency area"

#define IMPACTS_LOCATION                       "Location of impacts"

#define GOAL_FIGURES                           "Characteristics of taken goals"

#define GOALS_FIGURES                          "Location of taken goals"

#define CHARACTERISTICS_OF_THE_SHOOTS_TEXT     "Shoots characteristics (by shooter / increasing decreasing order / by sector )"

#define PREFERENCES_TAB_ITEM                   "  Preferences"

#define PREFERENCES_STRING_01                  " "

#define PREFERENCES_STRING_02                  "Webcam configuration"

#define PREFERENCES_STRING_03                  "Luminance correction"

#define PREFERENCES_STRING_04                  "User interface decoration"

#define PREFERENCES_STRING_05                  "  Traditional green theme"

#define PREFERENCES_STRING_06                  "  Dark theme"

#define PREFERENCES_STRING_07                  "  Classic theme"

#define PREFERENCES_STRING_08                  "  Light blue theme"

#define PREFERENCES_STRING_09                  "  Windows theme"

#define PREFERED_SHORTCUT_KEY_TEXT             "Prefered key for shortcuts : "

#define ALT_SHORTCUT_TEXT                      " ALT  "

#define SHIFT_SHORTCUT_TEXT                    " SHIFT   "

#define HELP_TAB_ITEM                          " Help "

#define THIS_PAGE_AIMS_TO_BETTER_USE_MINIDART  "This tab aims to help better using miniDart."

#define SLIDER_THIKNESS                        "Slider Thickness"

#define CREDITS_TAB_ITEM                       " Credits"

#define AUTHOR_ID_COPYRIGHT_YEARS              "miniDart author : Eric Bachard © 2016-2019"

#define DISPLAY_NUMBER_ID_CLUB_RESULT_IMPACT_CONDITIONS  "Display : Number / Id / Club / Result / Impact / Conditions "

#define USED_FONT                              "Font in use : "

//#define TRUETYPE_FONT_CURRENTLY_IN_USE

#define FONTSIZE                               "Size : "

#define UNDEFINED_DURATION_NOTATION            "Undefined duration annotation"

#define ANNOTATION_DURATION_IN_S               "Video incrustation duration (in s)"

#define DRAW_A_BACKGROUND_UNDER_THE_TEXT_Q     "Draw a background under the text?"

#define AUTOMATIC_POSITIONING_OF_THE_TEXT_Q    "Automatic location of the text ?"

#define HORIZONTAL_POSITIONING                 "Horiz. positioning of the text"

#define VERTICAL_POSITIONING                   "Vert. positioning of the text"

#define H_POSITION                             "H Position"

#define ON_THE_LEFT                            "To the left"

#define CENTERED                               "Centered"

#define ON_THE_RIGHT                           "To the right"

#define V_POSITION                             "V Position"

#define ON_TOP                                 "On top"

#define IN_THE_MIDDLE                          "In the middle"

#define AT_THE_BOTTOM                          "At the bottom" 

#define USE_RIGHT_CLICK_TO_TYPE_A_TEXT_OR_CHANGE_IT  "Use right click to type a new text or change it"

#define COMMENT                                "comment"

#define VALIDATE_BUTTON                        "Validate"

#define THE_MESSAGE_CONTAINS                   "The message contains : "

#define SUPPRESS_THE_TEXT                      "Remove the video incrustation"

#define INSERT                                 "Insert"

#define ADVANCED_PARAMETERS                    "Advanced parameters"

#define ADDITIONAL_PARAMETERS                  "Additional Parameters"

#define FONT_COLOR                             "Font color"

#define RED_FONT_COMP                          "Comp. R"

#define GREEN_FONT_COMP                        "Comp. G"

#define BLUE_FONT_COMP                         "Comp. B"

#define FONT_BASELINE                          "Font baseline "

#define DRAW_THE_BASELINE_Q                    "Draw the text baseline ?"

#define BOUNDING_BOX_COLOR                     "Frame color"

#define BB_RED_COMP_COLOR                      "Comp. R   "

#define BB_GREEN_COMP_COLOR                    "Comp. G   "

#define BB_BLUE_COMP_COLOR                     "Comp. B_  "

#define BACKGROUND_INCRUSTATION_COLOR          "Background annotation color"

#define BACKGROUND_RED_COMP_COLOR              "Comp. R  "

#define BACKGROUND_GREEN_COMP_COLOR            "Comp. G  "

#define BACKGROUND_BLUE_COMP_COLOR             "Comp. B  "

#define DRAW_THE_FRAME_BORDER_Q                "Draw the frame border?"

#define OUTLINE_COLOR                          "Frame border color"

#define OUTLINE_RED_COMP_COLOR                 "Outline R comp."

#define OUTLINE_GREEN_COMP_COLOR               "Outline G comp."

#define OUTLINE_BLUE_COMP_COLOR                "Outline B comp."

#define BACK_TO_DEFAULT_VALUES                 "Back to default values"

#define ANNOTATIONS_OK_BUTTON                  "OK"

#define TEXT_AND_STRING                        "Text : %s"
// PA means Players Actions

#define PA_GOAL_SCORED                                  "Goal scored"

#define PA_ATTACK                                       "Attack"

#define PA_DEFENSE                                      "Defense"

#define PA_COUNTER_ATTACK                              "Counter attack"

#define PA_BALL_RISE                                    "Ball rise"

#define PA_DEFENSIVE_WITHDRAWAL                        "Defensive withdrawal"

#define PA_BEHAVIOR                                     "Behavior"

#define PA_CLEAN_ALL_Q                                  "Clean all ?"

#define PA_ALL_ACTIONS_WILL_BE_CANCELED_STRING          "Everything will be definitely deleted. "

#define DRAWN_OBJECT_BACKGROUND_COLOR                   "Background color"

#define DRAWN_OBJECT_COLOR                   "Color"

#define DRAWN_OBJECT_OUTLINE_COLOR                 "Outline color"

#define DRAWN_OBJECT_LINE_THICKNESS                 "Thickness"

#define RIGHT_CLICK_FOR_INDIVIDUAL_COLOR_WIDGET_OPTIONS  "Right click on the individual color widget to show options"

#define HALF_ALPHA_PREVIEW  "With half alpha preview"

#define WITH_DRAG_AND_DROP  "With drag and drop"

#define WITH_OPTIONS_MENU  "With Options menu"

#define ALL_ANNOTATIONS_WILL_BE_CANCELED_Q  "All drawn objects will be deleted."

#define CONFIRM_BUTTON                              "Confirm"

#define MINIDART_VERSION_NUMBER                     "0.9.6"
