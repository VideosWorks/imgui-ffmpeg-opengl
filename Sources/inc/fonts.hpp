/* fonts.hpp
 * Copyright Eric Bachard  2019 July 30th / Licence GPL v2
 * This file is under GPL v2 license
 */

#ifndef __FONTS_HPP_
#define __FONTS_HPP_

/* Unix fonts */

#define UNIX_TTF_FONT_DROID_SANS              "./fonts/DroidSans.ttf"
#define UNIX_TTF_FONT_UBUNTU_REGULAR         "./fonts/Ubuntu-R.ttf"
#define UNIX_TTF_FONT_UBUNTU_BOLD            "./fonts/Ubuntu-B.ttf"

// LINUX ONLY : install MS fonts with msttcorefonts is mandatory
#define UNIX_TTF_FONT_COMIC_SANS_MS          "/usr/share/fonts/truetype/msttcorefonts/comic.ttf"
#define UNIX_TTF_FONT_COMIC_SANS_MS_BOLD     "/usr/share/fonts/truetype/msttcorefonts/comicbd.ttf"
#define UNIX_TTF_FONT_ARIAL                  "/usr/share/fonts/truetype/msttcorefonts/arial.ttf"
#define UNIX_TTF_FONT_ARIAL_BLACK            "/usr/share/fonts/truetype/msttcorefonts/ariblk.ttf"
#define UNIX_TTF_FONT_VERDANA                "/usr/share/fonts/truetype/msttcorefonts/verdana.ttf"
#define UNIX_TTF_FONT_TIMES_NEW_ROMAN        "/usr/share/fonts/truetype/msttcorefonts/times.ttf"
#define UNIX_TTF_FONT_IMPACT                 "/usr/share/fonts/truetype/msttcorefonts/impact.ttf"
//#define UNIX_TTF_FONT_WEBDINGS               "/usr/share/fonts/truetype/msttcorefonts/webdings.ttf"

#define UNIX_FONT_MAX      10

const char * SystemFontFile[UNIX_FONT_MAX] =
{
    UNIX_TTF_FONT_DROID_SANS             ,
    UNIX_TTF_FONT_UBUNTU_REGULAR        ,
    UNIX_TTF_FONT_UBUNTU_BOLD           ,
    UNIX_TTF_FONT_COMIC_SANS_MS         ,
    UNIX_TTF_FONT_COMIC_SANS_MS_BOLD    ,
    UNIX_TTF_FONT_ARIAL                 ,
    UNIX_TTF_FONT_ARIAL_BLACK           ,
    UNIX_TTF_FONT_VERDANA               ,
    UNIX_TTF_FONT_TIMES_NEW_ROMAN       ,
    UNIX_TTF_FONT_IMPACT
};

const char* font_items[] = { "DroidSans"      , "Ubuntu-Regular", "Ubuntu-Bold", "Comic Sans",
                             "Comic Sans bold", "Arial"         , "Arial black", "Verdana"   ,
                             "Times New Roman", "Impact"
                           };


#endif /*  __FONTS_HPP_  */

