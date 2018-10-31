/* Copyright : Eric Bachard  2018 August 5th */
/* this file is under MIT License */

#include <sdl2_ffmpeg/kitchensink.h>

#ifdef Linux
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1

#include <GL/glew.h>
#include <GL/gl.h>

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
*     DO NOT USE THIS EXAMPLE : IT DOES NOT WORK.
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
    unsigned int gltexture;
    SDL_Window *window = NULL;
    SDL_GLContext glcontext;
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetSwapInterval(1);

    // Create a resizable window.
    window = SDL_CreateWindow("Example Simple OpenGL Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN);
    if(window == NULL) {
        fprintf(stderr, "Unable to create a new window!\n");
        return 1;
    }

    glcontext = SDL_GL_CreateContext(window);
    if(glcontext == NULL) {
        fprintf(stderr, "Unable to create a new OpenGL context!\n");
        return 1;
    }

#ifdef Linux
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "glewInit() failed");
        return 1;
    }
#endif

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glEnable(GL_BLEND) problem is : %d\n", err);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) problem is : %d\n", err);

    glEnable(GL_DEPTH_TEST);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glEnable(GL_DEPTH_TEST) problem is : %d\n", err);

    glEnable(GL_TEXTURE_2D);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glEnable(GL_TEXTURE_2D) problem is : %d\n", err);

//    glEnable(GL_LIGHTING);
//    glDisable(GL_CULL_FACE);
//    glDisable(GL_SCISSOR_TEST);

    fprintf(stdout, "OpenGL version: %s\n", (char *)glGetString(GL_VERSION));
    fprintf(stdout, "GLSL version  : %s\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    fprintf(stdout, "Vendor        : %s\n", (char *)glGetString(GL_VENDOR));
    fprintf(stdout, "Renderer      : %s\n", (char *)glGetString(GL_RENDERER));

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
        KIT_OPENGL_RENDERER,
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

    // Initialize video texture. This will probably end up as YV12 most of the time.
    // FIXME : usefull ?
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    // OpenGL texture creation. This is the new part.

    ////////////// COMPLETE ME

    // This is the subtitle texture atlas. This contains all the subtitle image fragments.

    /*
    TODO : FIXME !!

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // Always nearest for atlas operations
    SDL_Texture *subtitle_tex = SDL_CreateTexture(
        renderer,
        pinfo.subtitle.output.format,
        SDL_TEXTUREACCESS_STATIC,
        ATLAS_WIDTH, ATLAS_HEIGHT);
    if(subtitle_tex == NULL) {
        fprintf(stderr, "Error while attempting to create a subtitle texture atlas\n");
        return 1;
    }

    // Make sure subtitle texture is in correct blendmode
    SDL_SetTextureBlendMode(subtitle_tex, SDL_BLENDMODE_BLEND);

    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    */

    int w = pinfo.video.output.width;//frame->width; //dec->codec_ctx->width;
    int h = pinfo.video.output.height;//frame->height; //dec->codec_ctx->height;

    glViewport(0, 0, pinfo.video.output.width, pinfo.video.output.height);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glViewPort problem is : %d\n", err);

    // If gltexture works, it means we see a yellow screen
    glClearColor(255, 255, 0, 255);
    err = glGetError();
    if ( GL_NO_ERROR != err )
        fprintf(stdout, "glClearColor problem is : %d\n", err);
    else
        fprintf(stdout, "glClearColor done \n");

    // Create the OpenGL GL_TEXTURE_2D
    // cf http://docs.gl/gl2/glTexImage2D
    // We know that glTexImage2D is slow, but we'll use glTexSubImage2D() in the loop for the gltexture update

    // Start playback
    Kit_PlayerPlay(player);

    // Playback temporary data buffers
    char audiobuf[AUDIOBUFFER_SIZE];

    // FIXME : usefull ?
    //    SDL_Rect sources[ATLAS_MAX];
    //    SDL_Rect targets[ATLAS_MAX];

    // FIXME broken ? usefull ?
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);


    while(run) {
        if(Kit_GetPlayerState(player) == KIT_STOPPED) {
            run = false;
            continue;
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    run = false;
                    break;

                default:
                    break;
            }
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

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        //    glGenTextures(1, &gltexture);
        //    glBindTexture(GL_TEXTURE_2D, gltexture);

        // Refresh videotexture and render it
        // OpenGL test
        Kit_GetPlayerVideoOpenGLData(player, gltexture);

        // Refresh subtitle texture atlas and render subtitle frames from it
        // For subtitles, use screen size instead of video size for best quality
        // FIXME !!!
        //        int got = Kit_GetPlayerSubtitleData(player, subtitle_tex, sources, targets, ATLAS_MAX);
        //        for(int i = 0; i < got; i++) {
        //            SDL_RenderCopy(renderer, subtitle_tex, &sources[i], &targets[i]);
        //        }

        SDL_GL_SwapWindow(window);

        // avoid leaks
        if (gltexture != 0)
            glDeleteTextures(1, &gltexture);

    }  /* end main loop */


    Kit_ClosePlayer(player);
    Kit_CloseSource(src);
    Kit_Quit();

    SDL_CloseAudioDevice(audio_dev);
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(glcontext);
    window = NULL;
    SDL_Quit();
    return 0;
}
