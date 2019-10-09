/*
 * File open_file_dialog.cpp, belongs to miniDart project
 * Copyright : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "open_file_dialog.h"

#include <string.h>

#if defined( NATIVE_BUILD)
#include <iostream>
#include "nfd.h"
    #ifdef TEST_NFD_EXT
    #include "nfd.hpp"
    #endif
#endif

#ifdef _WIN32
#include "Windows/file_dialog.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#endif


/* Returns 0 on success, 1 on error (or user hit cancel) */
int openFileDialog(char *filename)
{
#ifdef NATIVE_BUILD
    #ifdef TEST_NFD_EXT
        // initialize NFD
        NFD::Guard nfdGuard;

        // auto-freeing memory
        NFD::UniquePath outPath;

        // define filters for the dialog
        nfdfilteritem_t filterItem[1] = { { "Video files", "avi,mp4,mpg,ts,MTS,wmv" } };

        nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);
    #else
        nfdchar_t *outPath = NULL;
        // Filters means extensions (at least on Linux)
        nfdresult_t result = NFD_OpenDialog( "avi,mp4,mpg,ts,MTS,wmv", NULL, &outPath );
    #endif

    if ( result == NFD_OKAY )
    {
        #ifdef TEST_NFD_EXT
            std::cout << "Success!" << std::endl << outPath.get() << std::endl;
            strcpy(filename, outPath.get());
        #else
            puts("Success!");
            puts(outPath);
            strcpy(filename, outPath);
            free(outPath);
        #endif
        return 0;
    }
    else if ( result == NFD_CANCEL )
        puts("User pressed cancel.");
    else
    #ifdef TEST_NFD_EXT
        std::cout << "Error: " << NFD::GetError() << std::endl;
    #else
        fprintf(stderr,"Error: %s\n", NFD_GetError() );
    #endif

    return 0;
#else
    /* this is Windows File Picker */
    // https://stackoverflow.com/questions/1524356/c-deprecated-conversion-from-string-constant-to-char
    return openFileDialog(filename, (char *)"All\0All\0*.avi\0*.avi\0*.ts\0*.ts\0*.MTS\0*.MTS\0*.mp4\0*.mp4\0*.mpg\0*.mpg\0*.wmv\0*.wmv\0*.*\0");
#endif
}

#ifdef _WIN32
int openFileDialog(char *filename, char *filters) {
    /* open file dialogue */
    char cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);

    OPENFILENAME aVideo;

    char szFile[MAX_PATH];

    /* open a file name */
    ZeroMemory( &aVideo , sizeof( aVideo));
    aVideo.lStructSize = sizeof ( aVideo );
    aVideo.hwndOwner = NULL  ;
    aVideo.lpstrFile = szFile ;
    aVideo.lpstrFile[0] = '\0';
    aVideo.nMaxFile = sizeof( szFile );
    // Looks like the same filter entry must be entered twice, else, nothing works
    aVideo.lpstrFilter = "All\0*.*\0*.avi\0*.avi\0*.ts\0*.ts\0*.MTS\0*.MTS\0*.mp4\0*.mp4\0*.mpg\0*.mpg\0*.wmv\0*.wmv\0";
    aVideo.nFilterIndex =1;
    aVideo.lpstrFileTitle = NULL ;
    aVideo.nMaxFileTitle = 0 ;
    aVideo.lpstrInitialDir = NULL ;
    aVideo.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;

    /* change current working directory back to location of executable */
    SetCurrentDirectory(cwd);

    if (!GetOpenFileName( &aVideo)) {
        /* user hit cancel */
        return 1;
    }

    strcpy(filename, szFile);
    return 0;
}

#endif /* _WIN32 */
