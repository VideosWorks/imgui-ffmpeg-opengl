#include <stdlib.h>
#include "sdl2_ffmpeg/internal/kitlibstate.h"

static Kit_LibraryState _librarystate = {0, 1, 0, 3, 64, 64, NULL, NULL};

Kit_LibraryState* Kit_GetLibraryState() {
    return &_librarystate;
}
