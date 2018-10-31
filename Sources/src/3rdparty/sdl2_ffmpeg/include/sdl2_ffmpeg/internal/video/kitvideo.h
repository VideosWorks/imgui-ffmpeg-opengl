#ifndef KITVIDEO_H
#define KITVIDEO_H

#include <SDL_render.h>

#include "sdl2_ffmpeg/kitconfig.h"
#include "sdl2_ffmpeg/kitsource.h"
#include "sdl2_ffmpeg/kitplayer.h"
#include "sdl2_ffmpeg/internal/kitdecoder.h"

KIT_LOCAL Kit_Decoder* Kit_CreateVideoDecoder(const Kit_Source *src, int stream_index, Kit_RendererType rType);
KIT_LOCAL int Kit_GetVideoDecoderSDLData(Kit_Decoder *dec, SDL_Texture *texture);
KIT_LOCAL int Kit_GetVideoDecoderOpenGLData(Kit_Decoder *dec, unsigned int gltexture);
KIT_LOCAL int Kit_GetVideoDecoderOpenCVData(Kit_Decoder *dec, uint8_t * pData);
KIT_LOCAL int Kit_GetVideoDecoderOpenCVData2(Kit_Decoder *dec, uint8_t * pYUVData);
KIT_LOCAL int Kit_GetVideoDecoderYUV2OpenGLData(Kit_Decoder *dec, uint8_t * pData);
KIT_LOCAL double Kit_GetVideoDecoderPTS(Kit_Decoder *dec);

#endif // KITVIDEO_H
