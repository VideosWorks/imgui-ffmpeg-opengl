#ifndef KITSUBTITLE_H
#define KITSUBTITLE_H

#include <SDL_render.h>

#include "sdl2_ffmpeg/kitconfig.h"
#include "sdl2_ffmpeg/kitsource.h"
#include "sdl2_ffmpeg/internal/kitdecoder.h"

KIT_LOCAL Kit_Decoder* Kit_CreateSubtitleDecoder(
    const Kit_Source *src, int stream_index, int video_w, int video_h, int screen_w, int screen_h);
KIT_LOCAL void Kit_GetSubtitleDecoderTexture(Kit_Decoder *dec, SDL_Texture *texture);
KIT_LOCAL void Kit_SetSubtitleDecoderSize(Kit_Decoder *dec, int w, int h);
KIT_LOCAL int Kit_GetSubtitleDecoderInfo(
    Kit_Decoder *dec, SDL_Texture *texture, SDL_Rect *sources, SDL_Rect *targets, int limit);

#endif // KITSUBTITLE_H
