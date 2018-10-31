/* Copyright : Eric Bachard  2018 October 28th */
/* this file is under MIT License */

#include <sdl2_ffmpeg/kitchensink.h>

#ifdef Linux
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1

#include <SDL2/SDL.h>

#include <opencv2/opencv.hpp>

#endif
#else
#include <SDL.h>
#endif

#include <stdio.h>
#include <stdbool.h>

/*
*
*
*
*     WORK IN PROGRESS
*
*     IF YOU USE IT, IT'S AT YOUR OWN RISKS AND YOU CAN LOSE YOUR DATA !
*
*
*
*/

#define AUDIOBUFFER_SIZE (1024 * 64)
#define ATLAS_WIDTH 4096
#define ATLAS_HEIGHT 4096
#define ATLAS_MAX 1024

int main(int argc, char *argv[]) {
    int err = 0, ret = 0;
    const char* filename = NULL;
    bool run = true;
    Kit_Source *src = NULL;
    Kit_Player *player = NULL;
    SDL_AudioSpec wanted_spec, audio_spec;
    SDL_AudioDeviceID audio_dev;

    // Get filename to open
    if(argc != 2) {
        fprintf(stderr, "Usage: simple <filename>\n");
        return 0;
    }
    filename = argv[1];

    // Init SDL
    err = SDL_Init(SDL_INIT_EVERYTHING);
    if(err != 0) {
        fprintf(stderr, "Unable to initialize SDL2!\n");
        return 1;
    }

    // Initialize Kitchensink with network and libass support.
    err = Kit_Init(KIT_INIT_NETWORK|KIT_INIT_ASS);
    if(err != 0) {
        fprintf(stderr, "Unable to initialize Kitchensink: %s", Kit_GetError());
        return 1;
    }

    src = Kit_CreateSourceFromUrl(filename);
    if(src == NULL) {
        fprintf(stderr, "Unable to load file '%s': %s\n", filename, Kit_GetError());
        return 1;
    }

    player = Kit_CreatePlayer(
        src,
        Kit_GetBestSourceStream(src, KIT_STREAMTYPE_VIDEO),
        KIT_OPENCV_RENDERER,
        Kit_GetBestSourceStream(src, KIT_STREAMTYPE_AUDIO),
        Kit_GetBestSourceStream(src, KIT_STREAMTYPE_SUBTITLE),
        1280, 720);

    if(player == NULL) {
        fprintf(stderr, "Unable to create player: %s\n", Kit_GetError());
        return 1;
    }

    Kit_PlayerInfo pinfo;
    Kit_GetPlayerInfo(player, &pinfo);

    // Make sure there is video in the file to play first.
    if(Kit_GetPlayerVideoStream(player) == -1) {
        fprintf(stderr, "File contains no video!\n");
        return 1;
    }

    // Init audio
    SDL_memset(&wanted_spec, 0, sizeof(wanted_spec));
    wanted_spec.freq = pinfo.audio.output.samplerate;
    wanted_spec.format = pinfo.audio.output.format;
    wanted_spec.channels = pinfo.audio.output.channels;
    audio_dev = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &audio_spec, 0);
    SDL_PauseAudioDevice(audio_dev, 0);

    // OPENCV : convert FFMPEG frame into cv::Mat
    // Link : https://stackoverflow.com/questions/29263090/ffmpeg-avframe-to-opencv-mat-conversion
    int w = pinfo.video.output.width;
    int h = pinfo.video.output.height;

//    cv::Mat mYUV = cv::Mat(h + h/2, w, CV_8UC1);
//    cv::Mat mYUV = cv::Mat(h + h/2, w, CV_8UC1, cv::Scalar(0,0,255));
    cv::Mat mYUV = cv::Mat(h, w, CV_8UC3);
    cv::Mat const & rYUV = mYUV;
    uint8_t * pYUVData = (uint8_t *)rYUV.data;

    // Start playback
    Kit_PlayerPlay(player);

    // Playback temporary data buffers
    char audiobuf[AUDIOBUFFER_SIZE];

    // costly. Causing around 10% of proc load !
    cv::namedWindow("MyVideo2", CV_WINDOW_NORMAL);
    cv::resizeWindow("MyVideo2", 1280, 720);

    while(run) {
        if(Kit_GetPlayerState(player) == KIT_STOPPED) {
            run = false;
            continue;
        }

        // Refresh audio
        int queued = SDL_GetQueuedAudioSize(audio_dev);
        if(queued < AUDIOBUFFER_SIZE) {
            int need = AUDIOBUFFER_SIZE - queued;

            while(need > 0) {
                ret = Kit_GetPlayerAudioData(
                    player,
                    (unsigned char*)audiobuf,
                    AUDIOBUFFER_SIZE);
                need -= ret;
                if(ret > 0) {
                    SDL_QueueAudio(audio_dev, audiobuf, ret);
                } else {
                    break;
                }
            }
            // If we now have data, start playback (again)
            if(SDL_GetQueuedAudioSize(audio_dev) > 0) {
                SDL_PauseAudioDevice(audio_dev, 0);
            }
        }

        // Refresh videotexture and render it
        // OpenCV test
        Kit_GetPlayerVideoOpenCVData2(player, pYUVData);

        // Refresh subtitle texture atlas and render subtitle frames from it
        // For subtitles, use screen size instead of video size for best quality
        // FIXME !!!
        //        int got = Kit_GetPlayerSubtitleData(player, subtitle_tex, sources, targets, ATLAS_MAX);
        //        for(int i = 0; i < got; i++) {
        //            SDL_RenderCopy(renderer, subtitle_tex, &sources[i], &targets[i]);
        //        }

        cv::imshow("MyVideo2", mYUV);

        char key = (char)cv::waitKey(30);

        if ((key == 27) || (key == 'q') || (key == 'Q' ))
            run = false;

        if(key == ' ') {
            if (Kit_GetPlayerState(player) == KIT_PAUSED) {
                Kit_PlayerPlay(player);
            } else {
                Kit_PlayerPause(player);
            }
        }
    }  /* end main loop */

    mYUV.release();
    cv::destroyWindow("MyVideo2");

    Kit_ClosePlayer(player);
    Kit_CloseSource(src);
    Kit_Quit();

    SDL_CloseAudioDevice(audio_dev);
    SDL_Quit();
    return 0;
}
