/*
 * Fichier d'en tête miniDart_fr.hpp pour le projet miniDart
 * Auteur : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */


#ifndef miniDart_LOCALIZATION
#define miniDart_LOCALIZATION

#define APPLICATION_VERSION_STRING "miniDart 0.9.6.3 2020/03/20 Petite Truite"

#define UNIX_PATH_SEPARATOR                  "/"
#define WINDOWS_PATH_SEPARATOR               "\\"
#define OUTPUT_FILENAME_PREFIX               "MyVideo"

#define APPLICATION_NAME                     "miniDart 0.9.6.3"
#define TIME_DISPLAY                         "Temps : "
#define RECORD_DISPLAY                       "REC"
#define DELAY_DISPLAY                        "Retard : "

#define FILE_OPEN_MENU_ENTRY                 " Fichier"
#define EDITION_MENU_ENTRY                   " Édition"
#define MODE_MENU_ENTRY                      "Mode"
#define UNDO_MENU_ENTRY                      "Annuler"
#define REDO_MENU_ENTRY                      "Refaire"
#define CUT_MENU_ENTRY                       "Couper"
#define COPY_MENU_ENTRY                      "Copier"
#define PASTE_MENU_ENTRY                     "Coller"


#define UNDO_MENU_ENTRY_SHORTCUT             "Ctrl+z"
#define REDO_MENU_ENTRY_SHORTCUT             "Ctrl+y"
#define CUT_MENU_ENTRY_SHORTCUT              "Ctrl+x"
#define COPY_MENU_ENTRY_SHORTCUT             "Ctrl+c"
#define PASTE_MENU_ENTRY_SHORTCUT            "Ctrl+v"


#define TABBAR_NAME                          "#miniDart tab test"
#define DELAY_TAB_NAME                       " Atelier retard"


#define ZOOM_WINDOW                          " Loupe"
#define ZOOM_VALUE                           "Zoom "

#define CLOSE_BUTTON                         "Fermer"
#define CHRONO_RESET_BUTTON                  "Remise à zéro"
#define CHRONO_ACTIVITY_TIME                 "Temps d'activité %2d : %2d"
#define CHRONO_TOTAL_ELLAPSED_TIME           "Temps total écoulé %2d : %2d"

#define RENDERING_TIME_FPS_DISPLAY           "Rendu exécuté en %.3f ms/frame (%.1f FPS)"
#define DISPLAY_PAUSED_CHECKBOX              "  Affichage en pause"
#define RECORD_VIDEO_CHECKBOX                "  Enregistrer les images"

#define CURRENT_WORKING_DIRECTORY_BUTTON     "  Répertoire de travail"
#define CURRENT_WORKING_DIRECTORY_POPUP      "Répertoire de travail"

#define HELP_CHOOSE_WORKING_REC_DIRECTORY    "Avant de pouvoir enregistrer une vidéo, il faut définir un répertoire qui \ncontiendra les images enregistrées, et dans lequel vous avez le droit de \ncréér des fichiers de taille importante\n"

#define OK_BUTTON                            "OK"
#define ADVANCED_RECORDING_PARAMETERS_BUTTON "Paramètres avancés d'enregistrement"

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

#define FRAMES_PER_SECOND_CHOICE             "Images par seconde : "

#define RECORDED_FRAMESIZE_BUTTON           "Dimensions images_enregistrées : "
#define RECORDED_FRAMESIZE_POPUP            "dimensions_out_image"
#define RESTORE_DEFAULT_VALUES_BUTTON       "Remettre les réglages par défaut"

#define CLOSE_BUTTON_FRAMESIZE_POPUP        "Fermer"
#define DELAYED_DISPLAY_CHECKBOX            "  Affichage retardé"
#define DELAY_IN_SECONDS                    "Retard (en s):"

#define CHRONOMETER_CHECKBOX                "  Chronomètre"
#define CHRONOMETER_RECORD_CHECKBOX         "  Enregistrer le temps        "
#define CHRONOMETER_ACTIVITY_TIME_CHECKBOX  "  Afficher le temps d'activité"
#define CHRONOMETER_TOTAL_TIME_CHECKBOX     "  Afficher le temps total     "

#define INPUT_SOURCENAME_CHOICE             "Source : "
#define INPUT_SOURCENAME_USB_WEBCAM         " Webcam USB"
#define INPUT_SOURCENAME_VIDEO              " Fichier vidéo"
#define INPUT_SOURCENAME_IP_WEBCAM          " Réseau (webcam IP)"
#define INPUT_SOURCENAME_CELL_PHONE              " Téléphone portable"

#define INPUT_SOURCENAME_CHOICE             "Source : "
#define INPUT_SOURCENAME_CHOICE_POPUP       "choix_source"

#define SELECTED_WEBCAM_BUTTON              "Webcam sélectionnée"
#define WEBCAM1                             " webcam 1"
#define WEBCAM2                             " webcam 2"
#define WEBCAM3                             " webcam 3"

#define WEBCAM_CHOICE_POPUP                 "choix_webcam"

/* part 3 */

#define IMAGE_SIZE_IN_PIXELS_BUTTON          "Taille de l'image en pixels : "
#define IMAGE_FORMAT_POPUP                   "format_image"
#define RESET_THE_WEBCAM_BUTTON              "Réinitialiser la source"
#define ZOOM_ENABLE_CHECKBOX                 " Activer la loupe"
#define SELECTED_AREA_COLOR                  "Couleur de la sélection"
#define NETWORK_FLOW                         "#flux_reseau"
#define FLOW_URL                             "URL du flux : %s"
#define ENTER_URL                            "Saisir l'URL:"
#define OK_BUTTON2                           "Ok"
#define OPEN_A_VIDEO_BUTTON                  "Ouvrir une vidéo"
#define CLOSE_THE_VIDEO_BUTTON               "Fermer la vidéo"
#define CLOSE_THE_FLOW_BUTTON               "Fermer le flux rtsp"

/* part 4 */ 

#define ANNOTATIONS_CHILD_WINDOW             "Annotations"
#define ANALYSIS_WORKSHOP                    "  Atelier analyse"
#define WORKING_DIRECTORY_BUTTON             "Sélectionner un répertoire de travail"

#define IMAGE_CURRENTLY_DISPLAYED           "Image en cours de visualisation"
#define CANVAS_GROUP                        "Canvas"
#define IMAGES_FORMAT_IN_PIXELS         "Dimension des images (en pixels) : "
#define IMAGES_FORMAT_POPUP             "#dimensions"

#define FIGURES_TAB                         "  Statistiques"

#define OPEN_A_CLIP_BUTTON                  "Ouvrir un clip"
#define CLOSE_THE_CLIP_BUTTON               "Fermer le clip"

/*  part 5 */

#define THEME_MENU_ENTRY                   "Theme"

#define TRADITIONAL_GREEN_THEME_MENU_ENTRY "  Traditional green"

#define DARK_THEME_MENU_ENTRY              "  Dark"

#define CLASSIC_THEME_MENU_ENTRY           "  Classic"

#define LIGHT_BLUE_THEME_MENU_ENTRY        "  Light blue"

#define WINDOWS_THEME_MENU_ENTRY           "  Windows"

#define IMAGES_SOURCE_CHOICE_MENU_ENTRY    "Choix Source"

#define ABOUT_BUTTON                       " À propos ..."

#define ABOUT_MINIDART                     "À propos de miniDart"

#define VIDEO_SOURCE_SELECTION             "Choix Source"

#define PRESENTATION_STRING1               "Logiciel d'analyse vidéo pour coachs de sports collectifs \n(handball ou autre sport collectif), écrit en C++"

#define LINUX_PRESENTATION_STRING2         "Version : 0.9.6  (Linux Intel x86_64 )"

#define WINDOWS_PRESENTATION_STRING2       "Version : 0.9.6  (Windows 7+ x86_64 )"

#define LINK_PRESENTATION_STRING3          "Téléchargement : https://framagit.org/ericb/miniDart"

#define AUTHOR_PRESENTATION_STRING4        "Auteur du logiciel : Eric Bachard © 2016-2020"

#define LICENSE_PRESENTATION_STRING5       "Licence GPL v2+"

#define OK_BUTTON                          "OK"

#define VIDEO_CODEC_TEXT                   "Codec vidéo : "

#define SEE_RECORDABLE_AUDIO_DEVICES       "Voir les sources audio enregistrables "

#define SOURCES_OF_IMAGES                  "Source d'images :"

#define USE_ANOTHER_IMAGE_SOURCE_BUTTON    "Utiliser une autre source d'images"

#define SELECT_ANOTHER_IMAGES_SOURCE       "Choisir une autre source d'images"

#define ACTIVATE_THE_USB_WEBCAM            "Activer la webcam USB"

#define AUTOMATIC_FOCUS                    "Focus auto "

#define FOCUS_VALUE                        "Focus"

#define CURRENT_FOCUS_VALUE                "Focus actuel"

#define AUTOMATIC_LIGHT_EXPOSITION         "Exposition automatique "

#define LIGHT_EXPOSITION_VALUE             "Exposition"

#define CURRENT_LIGHT_EXPOSITION_VALUE     "Exposition actuelle"

#define PAN_VALUE                          "Pan   "

#define CURRENT_PAN_VALUE                  "Pan actuel : "

#define TILT_VALUE                         "Tilt   "

#define CURRENT_TILT_VALUE                 "Tilt actuel "

#define CURRENT_ZOOM_VALUE                 "Zoom actuel : "

#define RESET_PAN_AND_TILT_VALUE_BUTTON    "Reset pan et tilt"

#define AVAILABLE_CODECS_BUTTON            "Codecs disponibles"

#define OTHER_WEBCAM_PARAMETERS_BUTTON     "Autres Réglages Webcam"

#define ENTERED_WEBCAM_IP_VALUE            "IP webcam entrée : "

#define START_THE_DIFFUSION_BUTTON         "Commencer la diffusion"

#define START_BUTTON                       "Commencer"

#define OUTLINE_THICKNESS_VALUE            "Épaisseur du trait"

#define DEFINE_A_RECTANGULAR_AREA_IN_THE_IMAGE "Définir une zone rectangulaire dans l'image"

#define IMAGES_WIDTH_HEIGHT_RATIO          "Rapport largeur/hauteur des images"

#define CHANGE_THE_IMAGES_WIDTH_HEIGHT_RATIO "Modifier le rapport largeur/hauteur des images    "

#define _16_9_SCREEN                          " 16/9  (écran cinéma, defaut) "

#define _16_10_MACBOOK_PRO_EXEMPLE            " 16/10 (exemple :  macbook pro)"

#define _4_3_CLASSICAL_TV_SCREEN              " 4/3 (écran TV classique) "

#define _5_4_SCREEN                           " 5/4 "

#define CHANGE_IMAGES_ORIENTATION_BUTTON     "Changer l'orientation des images    "

#define IMAGES_ROTATION                      "Rotation_images"

#define ZERO_ROTATION                        " Pas de rotation  "

#define CLOCKWISE_90_DEGREES_ROTATION        " 90 degrés sens horaire) "

#define _180_DEGREES_ROTATION                 " 180 degrés "

#define TRIGO_90_DEGREES_ROTATION            " 90 degrés, sens trigo "

#define HORIZONTAL_FLIP                      "Flip horizontal"

#define APPLIED_BEFORE_THE_ROTATION          "(appliqué avant la rotation)"

#define ANNOTATE_THE_VIDEO                   "Annoter la vidéo"

#define FILLED_RECTANGLE_SOON_INTEGRATED     "L'annotation type dessin d'un rectangle plein sera dans une prochaine version"

#define EMPTY_RECTANGLE_SOON_INTEGRATED      "L'annotation type dessin d'un rectangle vide sera dans une prochaine version"

#define FILLED_ELLIPSE_SOON_INTEGRATED       "L'annotation type dessin d'une ellipse pleine est  en cours d'intégration"

#define EMPTY_ELLIPSE_SOON_INTEGRATED        "L'annotation type dessin d'une ellipse vide sera dans une prochaine version"

#define RANDOM_LINE_SOON_INTEGRATED          "L'annotation type tracé à la main sera dans une prochaine version"

#define SIMPLE_ARROW_SOON_INTEGRATED         "L'annotation type tracé d'une flèche droite sera dans une prochaine version"

#define SIMPLE_LINE_SOON_INTEGRATED          "L'annotation type tracé d'une ligne simple sera dans une prochaine version"

#define FRAMES_PER_SECOND                    "Images par seconde"

#define CANVAS_STRING_LINE01                 "Fonctionnel :"

#define CANVAS_STRING_LINE02                 "Ouvir un clip de 30 s à 1 min"

#define CANVAS_STRING_LINE03                 "Maxi conseillé : 1 min "

#define CANVAS_STRING_LINE04                 "Lire à une vitesse entre"

#define CANVAS_STRING_LINE05                 "5 et 60 images par seconde"

#define CANVAS_STRING_LINE06                 "Faire un arrêt sur image"

#define CANVAS_STRING_LINE07                 "Revenir au début"

#define CANVAS_STRING_LINE08                 "Avancer rapidement (de 10 images)"

#define CANVAS_STRING_LINE09                 "Revenir rapidement (de 10 images)"

#define CANVAS_STRING_LINE10                 "En cours d'intégration :"

#define CANVAS_STRING_LINE11                 "- extraire une image"

#define CANVAS_STRING_LINE12                 "- dessiner dans l'image"

#define CANVAS_STRING_LINE13                 "- insérer du texte"

#define CANVAS_STRING_LINE14                 "- définir un répertoire de travail"

#define CANVAS_STRING_LINE15                 "- choisir le format d'enregistrement"

#define CANVAS_STRING_LINE16                 "- enregistrer sous forme \nde vidéo d'une durée donnée"

#define CANVAS_STRING_LINE17                 "- choisir la durée"

#define CANVAS_STRING_LINE18                 "- ajout de mots clés dans un fichier xml\npermettant de trier et retrouver les images"

#define TEAM_A                               "Équipe A"

#define PLAYERS_LIST                          "Liste des joueurs"

#define ALL_ACTIONS                           "Toutes les actions"

#define GAME_ACTIONS_OF_ALL_PLAYERS           "Actions de jeu de toute l'équipe"

#define PLAYER_01                             "Joueur 1"

#define ALL_PLAYER_1_ACTIONS                  "Actions attribuées au joueur 1"

#define PLAYER_02                             "Joueur 2"

#define ALL_PLAYER_2_ACTIONS                  "Actions attribuées au joueur 2"

#define PLAYER_03                             "Joueur 3"

#define ALL_PLAYER_3_ACTIONS                  "Actions attribuées au joueur 3"

#define PLAYER_04                             "Joueur 4"

#define ALL_PLAYER_4_ACTIONS                  "Actions attribuées au joueur 4"

#define PLAYER_05                             "Joueur 5"

#define ALL_PLAYER_5_ACTIONS                  "Actions attribuées au joueur 5"

#define PLAYER_06                             "Joueur 6"

#define ALL_PLAYER_6_ACTIONS                  "Actions attribuées au joueur 6"

#define PLAYER_07                             "Joueur 7"

#define ALL_PLAYER_7_ACTIONS                  "Actions attribuées au joueur 7"

#define PLAYER_08                             "Joueur 8"

#define ALL_PLAYER_8_ACTIONS                  "Actions attribuées au joueur 8"

#define PLAYER_09                             "Joueur 9"

#define ALL_PLAYER_9_ACTIONS                  "Actions attribuées au joueur 9"

#define PLAYER_10                             "Joueur 10"

#define ALL_PLAYER_10_ACTIONS                  "Actions attribuées au joueur 10"

#define PLAYER_11                             "Joueur 11"

#define ALL_PLAYER_11_ACTIONS                  "Actions attribuées au joueur 11"

#define PLAYER_12                             "Joueur 12"

#define ALL_PLAYER_12_ACTIONS                  "Actions attribuées au joueur 12"

#define GK_FOLLOW_UP_TAB_ITEM                  "  Suivi GB "

#define STATISTICS_AND_GK_FOLLOW_UP            "Statistiques et suivi GB"

#define ENTER_THE_NAME                         "Entrer le nom:"

#define ENTER_THE_CLUB                         "Entrer le club:"

#define CHAMPIONSHIP                           "Championnat"

#define OPPONENT                               "Équipe adverse : "

#define ENTER_THE_DATE                         "Entrer la date :"

#define PRACTICE_RADIOBUTTON                   "Entraînement"

#define OFFICIAL_MATCH_RADIOBUTTON             "Match officiel"

#define DELETE_ALL_BUTTON                      "Tout effacer"

#define ALL_SHOOTS_WILL_BE_CANCELED_STRING     "Tous les tirs vont être effacés. L'effacement est définitif"

#define CANCEL_BUTTON                          "Annuler"

#define UNDO_BUTTON                            "Défaire"

#define FIRST_HALF_TIME                        "  1ère mi-temps"

#define SECOND_HALF_TIME                       "  2ème mi-temps"

#define GAME_PERIOD_BUTTON                     "Période de jeu : "

#define RIGHT_CLICK_TIP_FOR_ARROWS_DRAWING     "Clic gauche + glisser et relâcher pour ajouter un trait,\nclic droit pour modifier les paramètres. 7m : cliquer d'abord sur 7m puis ajouter le tir"

#define ARROWS_PARAMETERS                      "Paramètres"

#define PROVIDE_ADDITIONAL_INFORMATION_ABOUT_THE_SHOOT  "Saisie d'informations supplémentaires sur le tir"

#define DISPLAY_SHOOTERS_NUMBERS               "Afficher numéros des tireurs"

#define DRAW_THE_ARROWS                     "Dessiner les flèches"

#define ARROWS_POPUP_MENU                   "MenuFleches"

#define ARROWS_LENGTH                          "Longueur des flèches"

#define ARROWS_WIDTH                           "Largeur des flèches"

#define ENTER_A_NUMBER_BETWEEN_1_AND_99        "Entrer le numéro (de 1 à 99)"

#define SHOOT_CONDITIONS                       "Conditions du tir : "

#define SHOOT_AT_6M                            "Tir à 6m"

#define WING_SHOOT                             "Tir à l'aile"

#define FAR_SHOOT                              "Tir de loin"

#define PIVOT_SHOOT                            "Tir de pivot"

#define BACK_SHOOT                             "Tir d'arrière"

#define CENTRAL_SHOOT                          "Tir central 9m et plus"

#define COUNTER_ATTACK_FIGHT                   "Duel sur CA"

#define BETWEEN_BEANS_SHOOT                    "Tir entre les jambes"

#define MASKED_SHOOT                           "Tir masqué"

#define BOUNDING_SHOOT                         "Tir à rebond"

#define SIDE_SHOOT                             "Tir à la hanche"

#define IN_THE_RUN_SHOOT                       "Tir dans la course"

#define OVER_HEAD_SHOOT                        "Lob"

#define COUNTERED_SHOOT                        "Tir contré"

#define STRESSED_SHOOT                         "Tir stressé"

#define SHOOT_RESULT                           "Résultat du tir : "

#define GOAL_RADIOBUTTON                       "    But    "

#define GK_STOP_RADIOBUTTON                    "  Arrêt GB  "

#define BORDER_SHOOT_RADIOBUTTON               "   Poteau   "

#define OUT_SHOOT_RADIOBUTTON                  " Hors cadre "

#define _7M_BUTTON                              "    7m    "

#define DRAW_THE_ARROWS                        "Dessiner les flèches"

#define DISPLAY_THE_SHOOTERS_NUMBER            "Afficher numéros des tireurs"

#define GK_FIGURES_STRING_01                   "Statistiques GB"

#define GK_FIGURES_STRING_02                   "Nombre de tirs : %d"

#define GK_FIGURES_STRING_03                   "Nombre de 7m : %d"

#define GK_FIGURES_STRING_04                   "Nombre de tirs cadrés %d"

#define GK_FIGURES_STRING_05                   "Nombre de buts pris : %d"

#define GK_FIGURES_STRING_06                   "Nombre de buts pris sur 7m : %d"

#define GK_FIGURES_STRING_07                   "Nombre d'arrêts du GB : %d"

#define GK_FIGURES_STRING_08                   "Nombre d'arrêts du GB sur 7m : %d"

#define GK_FIGURES_STRING_09                   "Pourcentage d'arrêts du GB sur 7m : %.0f %%"

#define GK_FIGURES_STRING_10                   "Pourcentage de réussite du GB sur 7m : %.0f %%"

#define GK_FIGURES_STRING_11                   "Pourcentage d'arrêts du GB : %.0f %%"

#define GK_FIGURES_STRING_12                   "Pourcentage de réussite du GB : %.0f %%"

#define EFFICIENCY_AREA                        "Zone efficacité"

#define GK_EFFICIENCY_AREA                     "Zone d'efficacité du GB"

#define IMPACTS_LOCATION                   "Localisation des impacts"

#define GOAL_FIGURES                           "Caractéristiques buts pris"

#define GOALS_FIGURES                          "Caractéristiques des buts encaissés"

#define CHARACTERISTICS_OF_THE_SHOOTS_TEXT     "Caractéristiques et résultats des tirs (par tireur / ordre croissant / décroissant / secteur )"

#define PREFERENCES_TAB_ITEM                   "  Préférences"

#define PREFERENCES_STRING_01                  " "

#define PREFERENCES_STRING_02                  "Configuration de la webcam"

#define PREFERENCES_STRING_03                  "Correction de la luminance"

#define PREFERENCES_STRING_04                  "Décoration de l'interface utilisateur"

#define PREFERENCES_STRING_05                  "  Theme traditional green"

#define PREFERENCES_STRING_06                  "  Theme dark"

#define PREFERENCES_STRING_07                  "  Theme classic"

#define PREFERENCES_STRING_08                  "  Theme light blue"

#define PREFERENCES_STRING_09                  "  Theme Windows"

#define PREFERENCES_STRING_10                  "  Taille police de caractères##Selector"


#define PREFERED_SHORTCUT_KEY_TEXT             "Touche à utiliser pour les raccourcis claviers : "

#define ALT_SHORTCUT_TEXT                      " ALT  "

#define SHIFT_SHORTCUT_TEXT                    " SHIFT   "

#define HELP_TAB_ITEM                          " Aide "

#define THIS_PAGE_AIMS_TO_BETTER_USE_MINIDART  "Cette page permet de mieux utiliser miniDart."

#define SLIDER_THIKNESS                        "Slider Thickness"

#define CREDITS_TAB_ITEM                       " Crédits"

#define AUTHOR_ID_COPYRIGHT_YEARS              "Auteur du logiciel miniDart : Eric Bachard © 2016-2020"

#define DISPLAY_NUMBER_ID_CLUB_RESULT_IMPACT_CONDITIONS  "Afficher : Numéro / Idendité / Club / Résultat / Impact / Conditions "

#define USED_FONT                              "Police utilisée : "

//#define TRUETYPE_FONT_CURRENTLY_IN_USE

#define FONTSIZE                               "Taille : "

#define UNDEFINED_DURATION_NOTATION            "Annotation de durée indéfinie"

#define ANNOTATION_DURATION_IN_S               "Durée de l'incrustation vidéo (en s)"

#define DRAW_A_BACKGROUND_UNDER_THE_TEXT_Q     "Dessiner un fond sous le texte ?"

#define AUTOMATIC_POSITIONING_OF_THE_TEXT_Q    "Positionnement automatique du texte ?"

#define HORIZONTAL_POSITIONING                 "Positionnnement horizontal du texte"

#define VERTICAL_POSITIONING                   "Positionnnement vertical du texte"

#define H_POSITION                             "Position horizontale"

#define ON_THE_LEFT                            "À gauche"

#define CENTERED                               "Au centre"

#define ON_THE_RIGHT                           "À droite"

#define V_POSITION                             "Position verticale"

#define ON_TOP                                 "En haut"

#define IN_THE_MIDDLE                          "Au milieu"

#define AT_THE_BOTTOM                          "En bas" 

#define USE_RIGHT_CLICK_TO_TYPE_A_TEXT_OR_CHANGE_IT  "Utiliser le clic droit pour saisir un nouveau texte ou le modifier"

#define COMMENT                                "commentaire"

#define VALIDATE_BUTTON                        "Valider"

#define THE_MESSAGE_CONTAINS                   "Le message contient : "

#define SUPPRESS_THE_TEXT                      "Supprimer l'incrustation"

#define INSERT                                 "Insérer"

#define ADVANCED_PARAMETERS                    "Paramètres avancés"

#define ADDITIONAL_PARAMETERS                  "Paramètres supplémentaires"

#define FONT_COLOR                             "Couleur de la police de caractères"

#define RED_FONT_COMP                          "Composante R"

#define GREEN_FONT_COMP                        "Composante G"

#define BLUE_FONT_COMP                         "Composante B"

#define FONT_BASELINE                          "Ligne de base de la police de caractères "

#define DRAW_THE_BASELINE_Q                    "Dessiner la ligne de base du texte ?"

#define BOUNDING_BOX_COLOR                     "Couleur du cadre"

#define BB_RED_COMP_COLOR                      "Composante R   "

#define BB_GREEN_COMP_COLOR                    "Composante G   "

#define BB_BLUE_COMP_COLOR                     "Composante B_  "

#define BACKGROUND_INCRUSTATION_COLOR          "Couleur du fond pour l'incrustation"

#define BACKGROUND_RED_COMP_COLOR              "Composante R  "

#define BACKGROUND_GREEN_COMP_COLOR            "Composante G  "

#define BACKGROUND_BLUE_COMP_COLOR             "Composante B  "

#define DRAW_THE_FRAME_BORDER_Q                "Dessiner le bord du cadre ?"

#define OUTLINE_COLOR                          "Couleur du cadre"

#define OUTLINE_RED_COMP_COLOR                 "Composante R "

#define OUTLINE_GREEN_COMP_COLOR               "Composante G "

#define OUTLINE_BLUE_COMP_COLOR                "Composante B "

#define BACK_TO_DEFAULT_VALUES                 "Revenir aux valeurs par défaut"

#define ANNOTATIONS_OK_BUTTON                  "OK"

#define TEXT_AND_STRING                        "Texte : %s"

// PA means Players Actions

#define PA_GOAL_SCORED                                  "But marqué"

#define PA_ATTACK                                       "Attaque"

#define PA_DEFENSE                                      "Défense"

#define PA_COUNTER_ATTACK                              "Contre attaque"

#define PA_BALL_RISE                                    "Montée de balle"

#define PA_DEFENSIVE_WITHDRAWAL                        "Repli défensif"

#define PA_BEHAVIOR                                     "Comportement"

#define PA_CLEAN_ALL_Q                                  "Tout effacer ?"

#define PA_ALL_ACTIONS_WILL_BE_CANCELED_STRING          "Tous ce qui a été saisi va être effacé. L'effacement est définitif"

#define DRAWN_OBJECT_BACKGROUND_COLOR                   "Couleur du fond"

#define DRAWN_OBJECT_COLOR                   "Couleur"

#define DRAWN_OBJECT_OUTLINE_COLOR                 "Couleur du contour"

#define DRAWN_OBJECT_LINE_THICKNESS                "Épaisseur du trait"

#define RIGHT_CLICK_FOR_INDIVIDUAL_COLOR_WIDGET_OPTIONS  "Clic droit sur le sélecteur de couleurs pour voir les options"

#define HALF_ALPHA_PREVIEW  "Avec prévisualisation du canal alpha"

#define WITH_DRAG_AND_DROP  "Avec glisser-déposer"

#define WITH_OPTIONS_MENU  "Avec menu d'options"

#define ALL_ANNOTATIONS_WILL_BE_CANCELED_Q  "Tout ce qui a été dessiné va être effacé."

#define CONFIRM_BUTTON                      "Confirmer"

#define MINIDART_VERSION_NUMBER             "0.9.6"

#define MAGNIFIER_WINDOW_SIZE               "Taille fenêtre"
#endif /* miniDart_LOCALIZATION  */
