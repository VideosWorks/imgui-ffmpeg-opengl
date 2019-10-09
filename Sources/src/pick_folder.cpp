/*
 * File pick_folder.cpp, belongs to miniDart project
 * Copyright : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TEST_NFD_EXT
#include "nfd.hpp"
#include <iostream>
#else
#include "nfd.h"
#endif

#if defined( NATIVE_BUILD)
//#include <iostream>
#endif

#ifdef _WIN32
#include "Windows/file_dialog.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#endif

int pick_folder(char * currentPath)
{
#ifdef TEST_NFD_EXT
    NFD_Init();
    nfdchar_t *outPath;

    nfdresult_t result = NFD_PickFolder(&outPath, NULL);

    if ( result == NFD_OKAY )
    {
        puts("Success!");
        puts(outPath);
        strcpy(currentPath, outPath);
        NFD_FreePath(outPath);
    }
#else
    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);

    if ( result == NFD_OKAY )
    {
        puts("Success!");
        puts(outPath);
        strcpy(currentPath, outPath);
        free(outPath);
    }
#endif
    else if ( result == NFD_CANCEL )
        puts("User pressed cancel.");

    else
        fprintf(stderr, "Error: %s\n", NFD_GetError() );

#ifdef TEST_NFD_EXT
    NFD_Quit();
#endif
    return 0;
}
