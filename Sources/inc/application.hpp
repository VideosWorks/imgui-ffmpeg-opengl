/*
 * Fichier d'en tête application.hpp pour le projet miniDart
 * Auteur : Eric Bachard  / lundi 3 octobre 2016, 14:19:58 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <cstdio>

#include "helpers.h"
#include "timer.hpp"
#include "imgui.h"
#include "imgui_impl_sdl.h"
//#define IMGUI_VERSION_160_OR_MINUS

//#define MINIDART_CURRENT_VERSION  "miniDart 0.9.5 2019/07/09 Little Poisson Rouge"


// max value ?
//#define DELAY_MAX 40

// FIXME : the UI size is fixed, while the window size is modified => bad
// TODO : resize the ImGUI windows (but the font size) when resizing the SDL Window

// default values for the window
//#define WINDOW_WIDTH  1280
//#define WINDOW_HEIGHT 720
// testing values
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT  780
#endif

#ifndef CAPTURED_FRAME_WIDTH
#define CAPTURED_FRAME_WIDTH 1280
#define CAPTURED_FRAME_HEIGHT 720
//#define CAPTURED_FRAME_WIDTH 4096
//#define CAPTURED_FRAME_HEIGHT 2160
//#define CAPTURED_FRAME_WIDTH 2560
//#define CAPTURED_FRAME_HEIGHT 1440
#endif

#ifndef WEBCAM_PARAMETERS_AREA_WIDTH
#define WEBCAM_PARAMETERS_AREA_WIDTH 335.0f
#endif

// was a test. The result is : proc == 100%
//#define CAPTURED_FRAME_WIDTH 1980 + #define CAPTURED_FRAME_HEIGHT 1080

// 24 fps on Linux seems to be correct, both for displaying the frames and recording them
// though 30 fps on Linux works well already, but depends on the machine load
// 24 fps : usual machine (i3 / i5 dual + 6 GB of RAM + SSD ) : load ~ 52 %
// 30 fps : powerfull machine (i7 / quad / 12 GB of RAM + fast SSD and so on) : load ~ 30 %
// 60 fps : powerfull machine (i7 / quad / 12 GB of RAM + fast SSD and so on) : load ~ 68 %
// => testing 30 fps (yet on Linux)

#define LINUX_DEFAULT_FRAMERATE   24.0
//#define LINUX_DEFAULT_FRAMERATE   30.0

// As usual Window sucks : video writer is ok, not the webcam flow (15 fps only, instead of 24 or 30 expected)
// thanks to Logitech for the parameters obfuscation
// testing 20 ... needs a Windows for checking whether it works
#define WINDOWS_DEFAULT_FRAMERATE   30.0

// values start at 0

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif

#define DEFAULT_DEVICE_NUMBER  0

#define DEFAULT_APPLICATION_FONT "./fonts/DroidSans.ttf"

#define DEFAULT_FRAMERATE LINUX_DEFAULT_FRAMERATE
#define DEFAULT_APPLICATION_FONT_SIZE  19.0f

#define IMAGE_FORMAT_640x360        0
#define IMAGE_FORMAT_800x600        1
#define IMAGE_FORMAT_960x720        2
#define IMAGE_FORMAT_720P           3
#define IMAGE_FORMAT_1360x1768      4
#define IMAGE_FORMAT_1440x1080      5
#define IMAGE_FORMAT_1080P          6
#define IMAGE_FORMAT_2K             7
#define IMAGE_FORMAT_4K             8

#define DEFAULT_IMAGE_FORMAT IMAGE_FORMAT_720P

#define  ZOOM_WIDTH_MIN  270.0f
#define  ZOOM_HEIGHT_MIN ZOOM_WIDTH_MIN

#define LEFT_IMAGE_BORDER 1.0f
#define TOP_IMAGE_BORDER  1.0f
#define BOTTOM_IMAGE_BORDER (w*ratio + (2*LEFT_IMAGE_BORDER))
#define RIGHT_IMAGE_BORDER w -26.0f

// 40s should be sufficient in most cases
#define DELAY_MAX 40
// Minimal value
#define DELAY_MIN 1

// default value is 8GB of RAM (modern machines)

// 2 * 1073741824 bytes
#define RAM_LIMIT_2GB     2147483648
#define RAM_LIMIT_4GB     4294967296
#define RAM_LIMIT_8GB     8589934592
#define RAM_LIMIT_16GB   17179869184
#define RAM_LIMIT_32GB   34359738368

#define RAM_LIMIT_DEFAULT  REAM_LIMIT_8GB

enum RotateFlags {
    NO_ROTATION = 1,
    ROTATE_90_CLOCKWISE = 2, //Rotate 90 degrees clockwise
    ROTATE_180 = 3, //Rotate 180 degrees clockwise
    ROTATE_90_COUNTERCLOCKWISE = 4, //Rotate 270 degrees clockwise
    HORIZONTAL_FLIP = 5,
    FLIPPED_AND_ROTATE_90_CLOCKWISE = 6,
    FLIPPED_AND_ROTATE_90_COUNTER_CLOCKWISE = 8
};


#define RATIO(x,y)  (x)/(y)
#define RATIO_1280_720   1280.0f/720.0f
#define RATIO_16_10       16.0f/10.0f
#define RATIO_16_9       16.0f/9.0f
#define RATIO_4_3       4.0f/3.0f


enum ShootType
{
    BUT = 0,
    ARRET_GB = 1,
    POTEAU = 2,
    HORS_CADRE = 3,
    _7M = 4
};

// video flow
#define     USB_WEBCAM       0
#define     FULL_VIDEO       1
#define     IP_WEBCAM        2
#define     CELL_PHONE       3
#define     NETWORK          4
#define     VIDEO_CLIP       5
#define     NOT_A_VIDEO_FLOW 100


enum ModifierName
{
//  conflict between quit and fill a form (same letter cannot do both)
//    MOD_CTRL_KEY  = 0,
    MOD_ALT_KEY   = 0,
    MOD_SHIFT_KEY = 1
};

enum SelectableTab
{
    DELAY_TAB,
    ANALYSIS_TAB,
    STATS_TAB,
    GK_STATS_TAB,
    PREFERENCES_TAB,
    HELP_TAB,
    CREDITS_TAB
};

typedef enum aTheme
{
    LIGHT_GREEN_THEME          = 0,
    CLASSIC_THEME              = 1,
    DARK_THEME                 = 2,
    LIGHT_BLUE_THEME           = 3,
    WINDOWS_THEME              = 4
} THEME;

#define DEFAULT_THEME LIGHT_GREEN_THEME

/*  Better ?
struct applicationState {

}
*/

extern bool b_lostWebcam;

class Application
{
  public:
    Application();
    ~Application();

    short int          aValue;
    unsigned short int prefered_modifier;
    unsigned int       myFourcc;
    std::string        myContainer;
    cv::Size           outFrameSize;
    double             outFPS;
    double             out_oldFPS;
    double             video_duration;
    double             origin;

    float              ratio;
    float              scale;
    float              spot_radius;
    float              position;

    SDL_Event          aDummyEvent;

    bool               b_paused;
    bool               b_audio_paused;
    bool               b_playing_at_fps;
    bool               b_spot_active;
    bool               b_flip_image;
    bool               b_inserting_text;
    bool               b_video_running;
    bool               b_video_refresh;

    void               do_seek(double, int);
    //bool               b_lostWebcam;
    int                image_rotation_value;
    int                image_format;
    int                old_image_format;
    int                current_manual_exposure;
    int                old_manual_exposure;
    int                current_source;
    int                old_source;
    int                currentWidth;
    int                currentHeight;
    int                current_tab;
    int                outWidth;
    int                outHeight;
    int                out_image_format;
    int                out_old_framesize;
    int                out_new_framesize;

    int                delay;
    int                fdelay;
    int                currentFrameNumber;
    int                lastFrameNumber;

    // engine ?
    //int                currentWidth;
    //int                currentHeight;
    //cv::Mat            playerImage[];
    float              value2;
    //ClipReader *       pClipReader;
    bool               b_use_USB_webcam;
#ifdef FPS_TEST
    int                fps_test;
#endif
    //VideoState *       is;
    //SDL_Renderer *     renderer
    bool               Init();
    void               Run();
    void               toggleWindowFullscreen();
    bool               getIsWindowFullscreen();
    bool               setStyle();
    bool               initialize_recordableAudioSources();
    short int          loadFonts();
    SDL_Window*        getWindow();

  private:
    SDL_Window*   window;
    SDL_Texture*  texture;
    // Timing
    unsigned int  lastTick, fpsTick, fps, frameCount;

    // Test
    float         testx, testy;

    void          Clean();
    void          Update(float delta);
    void          Render(float delta);

    void          NewApplication();
    void          ResetCaptureDev();
    bool          mbIsFullscreen;
};

void getInfo(void);

#endif /* __APPLICATION_H */

