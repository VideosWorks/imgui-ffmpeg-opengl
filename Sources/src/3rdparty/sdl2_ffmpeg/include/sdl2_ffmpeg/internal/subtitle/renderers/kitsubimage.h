#ifndef KITSUBIMAGE_H
#define KITSUBIMAGE_H

#include "sdl2_ffmpeg/kitconfig.h"
#include "sdl2_ffmpeg/internal/kitdecoder.h"
#include "sdl2_ffmpeg/internal/subtitle/renderers/kitsubrenderer.h"

KIT_LOCAL Kit_SubtitleRenderer* Kit_CreateImageSubtitleRenderer(
    Kit_Decoder *dec, int video_w, int video_h, int screen_w, int screen_h);

#endif // KITSUBIMAGE_H
