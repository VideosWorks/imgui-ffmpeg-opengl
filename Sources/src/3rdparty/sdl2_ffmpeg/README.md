# SDL2_ffmpeg (cpp SDL2 ffmpeg OpenCV OpenGL)

FFmpeg and SDL2 based library for audio and video playback, written in C99. This repository is a fork of **Tuomas Virtanen** code (https://github.com/katajakasa/SDL_kitchensink). 

To retrieve all the changes, please check all the diffs at : https://github.com/ebachard/SDL_kitchensink

**The main idea is to reuse this nice library without using the SDL2 renderer with Dear ImGui (https://github.com/ocornut/imgui) in miniDart (https://framagit.org/ericb/miniDart).**

**More precisely, SDL_Renderer does not work well with native OpenGL 3.x contexts, and it is difficult to mix them. Thus, this fork.**

**And if your need is not to use OpenCV at the end, PLEASE prefer use the original code instead.**

**ericb / 2018 August 1st.** 


In this repository, several new examples are proposed :

- FFMPEG + OpenCV implementation, using cv::Mat + cv::imshow a cv::namedWindow // WORKS well, but software convertion needs 32% of one core.
- FFPMEG + OpenGL texture2D implementation // currently broken  
- FFMPEG + SDL2 YUV frames -> yuv2rgb conversion using glsl -> OpenCV cv::Mat  //  WORKS, but colors are not correct 

Only ffmpeg + OpenCV implementation is currently working well (needs maybe some optimization, WIP). YUV2OpenCV works somewhat (you can open a video, and hear the sound, but the colors are uggly)

See example_simple_OpenCV.c for further information. For testing, you need to compile everything (OpenCV 3.3+ is mandatory).


Linux instructions only (Windows needs are similar. FYI, I'm simply cross compiling from Linux to Windows) :

install OpenCV, SDL2, OpenGL maybe
````
git clone https://framagit.org/ericb/sdl2_ffmpeg.git
cd sdl2_ffmpeg
mkdir build
cd build
cmake ..
make
````

**If everything is fine, you should obtain a simple_OpenCV binary and you just have to run ./simple_OpenCV somefile.mp4 to see video + sound synchronized.**

TIP : if the video is not fluent, replace cv::waitKey(30) with cv::waitKey(20) (should be sufficient).

Thanks in advance for any feedback !!

    FFMPEG + OpenGL. Currently broken. [needs help]
    FFMPEG + YUV2RGB // WIP, works
    FFMPEG + YUV2RGB (version2) // WIP, works, but colors are wrong

Last: the build should be warnings free. Please file an issue if ever there some ...

Original documentation is available at http://katajakasa.github.io/SDL_kitchensink/

Features:

    Decoding video, audio and subtitles via FFmpeg
    Dumping video and subtitle data on SDL_textures
    Dumping audio data in the usual mono/stereo interleaved formats
    Automatic audio and video conversion to SDL2 friendly formats
    Synchronizing video & audio to clock
    Seeking forwards and backwards
    Bitmap, text and SSA/ASS subtitle support
