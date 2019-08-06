/*
  File statistics.hpp, belongs to miniDart project
  Copyright : Eric Bachard, the file has been created 2017 december 19th
  This file is under GPL v2 license
  See :  http://www.gnu.org/licenses/gpl-2.0.html
*/

#ifndef __STATISTICS_H_
#define __STATISTICS_H_

#define EVENTS_NUMBER                                 9
#define GOAL_EVENTS_NUMBER                            5
#define ATTACK_EVENTS_NUMBER                         13
#define DEFENSE_EVENTS_NUMBER                        14
#define DEFENSIVE_RETURN_EVENTS_NUMBER                6
#define COUNTER_ATTACK_EVENTS_NUMBER                  7
#define BALL_PROGRESSION_EVENTS_NUMBER                8
#define BEHAVIOR_EVENTS_NUMBER                        6
#define DEFENSIVE_RETURN_EVENTS_NUMBER                6

#define EVENT_GOAL              0
#define EVENT_ATTACK            1
#define EVENT_DEFENSE           2
#define EVENT_COUNTER_ATTACK    3
#define EVENT_PROGRESS          4
#define EVENT_DEFENSIVE_RETURN  5
#define EVENT_BEHAVIOR          6
#define EVENT_UNDO              7
#define EVENT_CLEAN_ALL         8

//const char * events_array[EVENTS_NUMBER];

#define  CANCEL_EVENT  1000

enum Goal_Event
{
   GOAL_ON_SHOOT      = 0,
   GOAL_AT_7M         = 1,
   GOAL_AGAINST       = 2,
   GOAL_AGAINST_AT_7M = 3,
   CANCEL_GOAL        = 4
};


enum Defense_Event
{
    DUEL_DEFENSIF_GAGNE             =  0,
    CAUSE_UNE_PERTE_DE_BALLE        =  1,
    INTERCEPTION                    =  2,
    BONNE_ENTRAIDE                  =  3,
    DISSUASION_EFFICACE             =  4,
    CONTRE_AYANT_AIDE_LE_GB         =  5,
    BIEN_SORTI                      =  6,
    BIEN_GERE_LA_DISTANCE_DE_COMBAT =  7,
    PROVOQUE_UN_PASSAGE_EN_FORCE    =  8,
    DUEL_DEFENSIF_PERDU             =  9,
    CONCEDE_UN_7M                   = 10,
    MAUVAIS_CONTRE                  = 11,
    DEFENSE_EN_ZONE                 = 12,
    CANCEL_DEFENSE                  = 13
};

enum Attack_Event
{
    DUEL_OFFENSIF_GAGNE         =  0,
    BONNE_COURSE                =  1,
    RECUPERE_LE_BALLON          =  2,
    PROVOQUE_UN_7M              =  3,
    PRISE_INTERVALLE            =  4,
    MAUVAISE_PASSE              =  5,
    MARCHER                     =  6,
    PERTE_DE_BALLE              =  7,
    MAUVAIS_BLOC                =  8,
    PASSAGE_EN_FORCE            =  9,
    REPRISE_DE_DRIBBLE          = 10,
    EMPIETEMENT                 = 11,
    CANCEL_ATTACK               = 12
};

enum Counter_Attack_Event
{
    BONNE_ANTICIPATION    = 0,
    BONNE_RELANCE         = 1,
    BONNE_PASSE           = 2,
    MAUVAISE_ANTICIPATION = 3,
    PERTE_DU_BALLON       = 4,
    MAUVAISE_RELANCE      = 5,
    CANCEL_COUNTER_ATTACK = 6
};

enum Ball_Progression_Event
{
    BON_ETAGEMENT            = 0,
    BON_ECARTEMENT           = 1,
    PASSE_DANS_LA_COURSE     = 2,
    BONNE_DISPONIBILITE      = 3,
    PERTE_DU_BALLON_SUR_MB   = 4,
    FAUTE_D_ATTAQUANT        = 5,
    CANCEL_BALL_PROGRESSION  = 6
};


enum Defensive_Return_Event
{
    BON_HARCELEMENT          = 0,
    RECUPERATION_SUR_RD      = 1,
    BON_ETAGEMENT_RD         = 2,
    PAS_REVENU_DEFENDRE      = 3,
    PB_ORIENTATION           = 4,
    CANCEL_DEFENSIVE_RETURN  = 5
};


enum Behavior_Event
{
    AVERTISSEMENT   = 0,
    _2_MIN           = 1,
    CARTON_ROUGE    = 2,
    RAPPORT         = 3,
    BLESSURE        = 4,
    CANCEL_BEHAVIOR = 5
};




const char * events_array[EVENTS_NUMBER] =
{
    "BUT",
    "ATTAQUE",
    "DÉFENSE",
    "CONTRE-ATTAQUE",
    "MONTÉE DE BALLE",
    "REPLI DÉFENSIF",
    "COMPORTEMENT",
    "DÉFAIRE",
    "TOUT EFFACER"
};

const char * goal_events[GOAL_EVENTS_NUMBER] =
{
    "+ BUT MARQUÉ",
    "+ BUT MARQUÉ SUR 7m",
    "- BUT ENCAISSÉ",
    "- BUT ENCAISSÉ SUR 7m",
    "  ANNULER"
/*
    "+ But marqué",
    "+ But marqué sur 7m",
    "- But encaissé",
    "- But encaissé sur 7m",
    "  VALIDER",
    "  ANNULER"
*/
};




const char * attack_events[ATTACK_EVENTS_NUMBER] =
{
    "+  DUEL OFFENSIF GAGNÉ",
    "+  BONNE COURSE",
    "+  PROVOQUE UNE PERTE DE BALLE",
    "+  PROVOQUE UN 7M",
    "+  PRISE D'INTERVALLE",
    "-  MAUVAISE PASSE",
    "-  MARCHER",
    "-  PERTE DE BALLE",
    "-  MAUVAIS BLOC",
    "-  PASSAGE EN FORCE",
    "-  REPRISE DE DRIBBLE",
    "-  ZONE",
    "   ANNULER"
};

const char * defense_events[DEFENSE_EVENTS_NUMBER] =
{
    "+  DUEL DEFENSIF GAGNÉ",
    "+  PROVOQUE UNE PERTE DE BALLE",
    "+  INTERCEPTION",
    "+  BONNE ENTRAIDE",
    "+  DISSUASION EFFICACE",
    "+  CONTRE AYANT AIDÉ LE GB",
    "+  BIEN SORTI",
    "+  BIEN GÉRÉ LA DISTANCE DE COMBAT",
    "+  PROVOQUE UN PASSAGE EN FORCE",
    "-  DUEL DEFENSIF PERDU",
    "-  CONCÈDE UN 7m",
    "-  MAUVAIS CONTRE",
    "-  DÉFENSE EN ZONE",
    "   ANNULER"
};


const char * counter_attack_events[COUNTER_ATTACK_EVENTS_NUMBER] =
{
    "+  BONNE ANTICIPATION",
    "+  BONNE RELANCE",
    "+  BONNE PASSE",
    "-  MAUVAISE ANTICIPATION",
    "-  PERTE_DU_BALLON",
    "-  MAUVAISE RELANCE",
    "   ANNULER"
};



const char * ball_progression_events[BALL_PROGRESSION_EVENTS_NUMBER] =
{
    "+ BON ÉTAGEMENT",
    "+ BON ÉCARTEMENT",
    "+ PASSE DANS LA COURSE",
    "+ BONNE DISPONIBILITÉ",
    "- PERTE DE BALLE",
    "- REPRISE DE DRIBBLE",
    "- FAUTE D'ATTAQUANT",
    "  ANNULER"
};



const char *  defensive_return_events[DEFENSIVE_RETURN_EVENTS_NUMBER]
{
    "+ BON HARCÈLEMENT",
    "+ RÉCUPÉRATION SUR REPLI DÉFENSIF",
    "+ BON ÉTAGEMENT",
    "- PAS REVENU DÉFENDRE",
    "- PROBLÈME D'ORIENTATION",
    "  ANNULER"
};

/*
en-US
const char *  defensive_return_events[DEFENSIVE_RETURN_EVENTS_NUMBER]
{
    "+ GOOD HASSLING",
    "+ RECUPERATION ON DEFENSIVE WITHRAWAL",
    "+ GOOD STAGING",
    "- DID NOT RETURN TO DEFEND",
    "- ORIENTATION ISSUE",
    "  CANCEL"
};

*/


const char * behavior_events[BEHAVIOR_EVENTS_NUMBER] =
{
    "- AVERTISSEMENT",
    "- 2 MIN",
    "- CARTON ROUGE",
    "- RAPPORT",
    "- BLESSURE",
    "  ANNULER"
};

/* 

en-US
const char * behavior_events[BEHAVIOR_EVENTS_NUMBER] =
{
    "- WARNING",
    "- 2 MIN",
    "- RED CARD",
    "- REPORT",
    "- INJURY",
    "  CANCEL"
};
*/


#endif /* __STATISTICS_H_ */
