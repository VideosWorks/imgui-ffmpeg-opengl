#ifndef KITAUDIO_H
#define KITAUDIO_H

#include "sdl2_ffmpeg/kitconfig.h"
#include "sdl2_ffmpeg/kitsource.h"
#include "sdl2_ffmpeg/internal/kitdecoder.h"

KIT_LOCAL Kit_Decoder* Kit_CreateAudioDecoder(const Kit_Source *src, int stream_index);
KIT_LOCAL int Kit_GetAudioDecoderData(Kit_Decoder *dec, unsigned char *buf, int len);
KIT_LOCAL double Kit_GetAudioDecoderPTS(Kit_Decoder *dec);

#endif // KITAUDIO_H
