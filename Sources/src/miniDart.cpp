/*  miniDart.cpp Copyright Eric Bachard 2016-2019 */
/*  Licence GPL v2. See : http://www.gnu.org/licenses/gpl-2.0.html  */

#include "SDL2/SDL.h"                  // We use SDL2 !
#include <opencv2/opencv.hpp>          // the main entry for OpenCV 3.x
#include "opencv2/highgui.hpp"         // the GUI entry
#include <opencv2/videoio/videoio_c.h> // contains all the video capture, writing features entries
#include <iostream>
#include <cstdlib>
#include <list>
#include <queue>                       // for the FIFO stack

#include "application.hpp"             // WINDOW_WIDTH, WINDOW_HEIGHT, Application::getInfo()
#include "resetImageSource.hpp"
#include "colors.hpp"
#include "setlabel.h"                  // Factorized methode
#include "capturedev.hpp"              // Defines the full menu
#include "timer.hpp"

#include "checkPlayersActions.hpp"     // Check players actions during the match
#include "text_incrustation.hpp"       // text_in_video()
#include "draw_text.hpp"               // text Canvas for incrustation in videos
#include "audiodevices.hpp"            // getSDLRecordableAudioDeviceNames() + AudioDevices class

#include "imgui.h"                     //  immediate mode GUI
#include "imgui_themes.h"

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl.h>                     // OpenGL

#include "open_file_dialog.h"
#include "clipreader.hpp"
#include "sdl_utils.hpp"
#include "pick_folder.h"
#include "imgui_helpers.h"
#include "FontAwesome.h"
#include "md_definitions.h"
#include "shoots.hpp"

#include "ffmpeg_player.hpp"
#include "engine.hpp"
#include "cv_helpers.hpp"
#include "canvas_objects.hpp"

#ifdef BUILDING_FRENCH
#include "miniDart_fr.hpp"
#else
#include "miniDart_en-US.hpp"
#endif

//#define BACKGROUND_IMAGE "./images/test5.png"

// TEST
#define BACKGROUND_IMAGE "./images/test6.png"

#define HANDBALL_PLAYGROUND_IMAGE "./images/analyse_GB/secteurs_tirs.png"

/* video player */
#define AUDIOBUFFER_SIZE 192000
//#define AUDIOBUFFER_SIZE (1024 * 64)
#define ATLAS_WIDTH 4096
#define ATLAS_HEIGHT 4096
#define ATLAS_MAX 1024

// seen as external by Videoplayer
cv::Mat videoMat;
uint8_t * pData = 0;


// seen as external by insert_text_in_video
bool b_inserting_text = false; // default value

//int queued;

static int video_init(char *);

#ifndef HIGH_DPI_SCREEN
#define HIGH_DPI_SCREEN
#endif

// other possibilities are : KMOD_CTRL, KMOD_SHIFT
// User preferences. Either in a configuration file, or in a live structure ? (both, loaded at launch time) ?
static unsigned short int prefered_modifier = KMOD_ALT;

// FIXME : choose the start tab ? Use the last one (means : save the last used one when quit ?)
int current_tab = DELAY_TAB;

// FIXME
//static unsigned int compteur = 0;

static short int aValue = 1;

// OUTPUT DEVICE class Recorder
// Recorded images parameters (output) => should be a class (recorder ?)
//static unsigned int myFourcc = CV_FOURCC('M','J','P','G');
static unsigned int myFourcc = CV_FOURCC('D','I','V','3');
static std::string myContainer = ".avi";
static int outWidth = 1280;
static int outHeight = 720;
static cv::Size outFrameSize(static_cast<int>(outWidth), static_cast<int>(outHeight));
static double outFPS = 24;

#ifdef FPS_FEATURE_ENABLED
static int out_image_format = DEFAULT_IMAGE_FORMAT; // IMAGE_FORMAT_720P
static int out_old_framesize = outWidth * outHeight;
static int out_new_framesize = out_old_framesize;
//static double out_oldFPS = outFPS;
//static double out_newFPS = out_oldFPS;
#endif

static SDL_Event aDummyEvent;
static bool b_audio_paused = false;


static void do_seek(double incr, long int pos);

static void ToggleButton(const char* str_id, bool* v);

// GUI
// Application background when no capture device is active
const cv::Mat backgroundImage = cv::imread( BACKGROUND_IMAGE );
const cv::Mat playgroundImage = cv::imread( HANDBALL_PLAYGROUND_IMAGE );

// GUI class / private
// (TODO) rather use " ImVector <GLuint> backgroundTextureID "
GLuint backgroundTextureId = 0;
GLuint backgroundTextureId2 = 0;
GLuint backgroundTextureId3 = 0;
GLuint backgroundTextureId4 = 0;

// Application::images_source

// Choix source d'enregistrement, valeur par défaut
static int current_source = USB_WEBCAM;

static int old_source = current_source;

// extern => should become Application::b_lostWebcam
bool b_never_initialized_webcam = true;
bool b_lostWebcam = true;

// Application::b_flipped_flag
static bool b_flip_image = false;

static float ratio = RATIO(720.0f,1280.0f);

static float frame_width = 1280.0f;
static float frame_height = frame_width * ratio;

#define TEST_SCALE
#ifdef TEST_SCALE
static float scale = 1.0f;
#endif

// Application::b_image_rotation_value
static int image_rotation_value = NO_ROTATION;

// Application::b_spot_active
static bool b_spot_active = false;
static bool b_record_rectangle= false;
//static float spot_radius = 60.0f;

// CaptureDev parameters (input)
static int image_format = DEFAULT_IMAGE_FORMAT; // IMAGE_FORMAT_720P
static int old_image_format = image_format;
static char defaultPath[PATH_MAX];

#define EXPOSURE_FEATURE

#ifdef EXPOSURE_FEATURE
#define MINIDART_V4L2_EXPOSURE_AUTO              0
#define MINIDART_V4L2_EXPOSURE_MANUAL            1
#define MINIDART_V4L2_EXPOSURE_SHUTTER_PRIORITY  2
#define MINIDART_V4L2_EXPOSURE_APERTURE_PRIORITY 3

int current_manual_exposure = 312;
int old_manual_exposure = current_manual_exposure;

#endif /* EXPOSURE_FEATURE */

    // FIXME : hide me
int currentWidth  = WINDOW_WIDTH;
int currentHeight = WINDOW_HEIGHT;

// Gui::current_theme
static int current_theme = DEFAULT_THEME; // system default is LIGHT_GREEN_THEME

// Gui::set_theme()

// declaration forward
static int set_theme(int);

// implementation
static int set_theme(int aTheme)
{
    switch(aTheme)
    {
        case LIGHT_GREEN_THEME: ImGui::StyleColorsLightGreen(); break;
        case DARK_THEME: ImGui::StyleColorsDark(); break;
        case CLASSIC_THEME: ImGui::StyleColorsClassic(); break;
        case LIGHT_BLUE_THEME: ImGui::StyleColorsLight(); break;
        case WINDOWS_THEME: ImGui::StyleColorsWindows(); break;
        default: break;
    }
    current_theme = aTheme;
    return 0;
}
// End Gui::change_theme()

// just a value for the slider on the tabs
static float value2 = 0.0f;

static void inc_color(void)
{
    aValue++;

    if (aValue == 9)
        aValue = 1;
}

// Application::Application()  // CTor
static ClipReader * pClipReader = nullptr;
////VideoPlayer * pVideoPlayer = nullptr;

// Idea : no longer depend of the webcam when launching the application
static bool b_use_USB_webcam = false;

#ifdef FPS_TEST
static int fps_test = 24;
#endif


static bool b_video_running = false;
static bool b_playing_at_fps = false;

VideoState * is = 0;
AVPacket  flush_pkt;
SDL_AudioDeviceID audio_dev;
double incr = 0.0f;
double video_duration = 0.0f;
static double origin = 0.0f;
static double max_position = origin + video_duration;


// declaration forward
static float initialize_position(void);

static float initialize_position(void)
{
#ifdef MASTER_CLOCK
   origin = get_master_clock(is);
#else
   origin = get_audio_clock(is);
#endif
   max_position = origin + video_duration;

   std::cout << "Initialization : origin         =  " << origin                << std::endl;
   std::cout << "                 max_position   =  " << max_position          << std::endl;
   std::cout << "                 video_duration =  " << max_position - origin << std::endl;

   return (max_position - origin);
}

static float position = 0.0f;

// reader::do_seek(double incr, int pos);
static void do_seek(double incr, long int pos)
{
    std::cout << "Call values ...  incr : " << incr  << " pos : " << pos  << std::endl;

    if (pos == 0)
    {
        incr = 0.0f;
    }
    else
    {
#ifdef MASTER_CLOCK
        pos = get_master_clock(is);
        std::cout << "get_audio_clock(is) : " << get_master_clock(is) << std::endl;
#else
        pos = get_audio_clock(is);
        std::cout << "get_audio_clock(is) : " << get_audio_clock(is) << std::endl;
#endif
        std::cout << " incr : " << incr  << " pos : " << pos  << std::endl;
        pos += incr;

        if (pos < 0)
            pos = 0.0f;
    }
    std::cout << "stream_seek prepared :  incr = " << incr  << "  pos =  " << pos  << std::endl;
    stream_seek(is, (int64_t)(pos * AV_TIME_BASE), (int64_t)(incr * AV_TIME_BASE), 0);
}


// Application::b_open_a_file = false
static bool b_open_a_file = false;

// Tools::LoadFile(NULL);
static int LoadFile(const char * filename, int video_type)
{

    if (filename)
    {
        switch(video_type)
        {
            case CELL_PHONE:
            case IP_WEBCAM:
            case FULL_VIDEO:
                b_audio_paused = false;
                video_duration = 0.0f;
                video_init((char *)filename);
                position = initialize_position();
            break;

            case VIDEO_CLIP:
                pClipReader->setFile((char *)filename);
                pClipReader->initClip();
            break;

            default:
            break;
        }
    }
    else
    {
        /* load the file from GUI */
        char new_filename[PATH_MAX];
        openFileDialog(new_filename) ? fprintf(stderr, "User aborted the open file dialog.\n") : LoadFile(new_filename, video_type);
    }
    return 0;
}

// FilesTools::selectFolder()
static int selectFolder(const char * currentPath)
{
    if (currentPath)
    {
        // do nothing
        std::cout << "currentPath contient : " << currentPath << std::endl;
        // strcpy(defaultPath, currentPath);
        std::cout << "defaultPath contient : " << defaultPath << std::endl;
    }
    else
    {
        std::cout << "No filename, please choose one." << std::endl;
        /* load the file from GUI */
        //char new_path[PATH_MAX];
        pick_folder(defaultPath) ? fprintf(stderr, "User aborted the open file dialog.\n") : selectFolder(defaultPath);
    }
    return 0;
}


static void ShowMenuFile();
static char * currentPath;

// Application::onEvent(SDL_Event *)
//void onEvent(SDL_Event *);

// Application::update(SDL_Event *)
void update(const SDL_Event *);

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

std::vector<std::string> recordableAudioDeviceNames;

const std::string canvasObjectImagePath[CANVAS_OBJECTS_TYPES_MAX] =
{
#define TESTING_DARK_THEME
#ifdef TESTING_DARK_THEME
    TEXT_OBJECT_IMAGE_DARK_PATH,
    FILLED_RECTANGLE_IMAGE_DARK_PATH,
    EMPTY_RECTANGLE_IMAGE_DARK_PATH,
    FILLED_ELLIPSE_IMAGE_DARK_PATH,
    EMPTY_ELLIPSE_IMAGE_DARK_PATH,
    RANDOM_LINE_IMAGE_DARK_PATH,
    RANDOM_ARROW_IMAGE_DARK_PATH,
    SIMPLE_ARROW_IMAGE_DARK_PATH,
    SIMPLE_LINE_IMAGE_DARK_PATH
#else
    TEXT_OBJECT_IMAGE_PATH,
    FILLED_RECTANGLE_IMAGE_PATH,
    EMPTY_RECTANGLE_IMAGE_PATH,
    FILLED_ELLIPSE_IMAGE_PATH,
    EMPTY_ELLIPSE_IMAGE_PATH,
    RANDOM_LINE_IMAGE_PATH,
    RANDOM_ARROW_IMAGE_PATH,
    SIMPLE_ARROW_IMAGE_PATH,
    SIMPLE_LINE_IMAGE_PATH
#endif

};

GLuint canvasObjectImageTexId[CANVAS_OBJECTS_TYPES_MAX];
cv::Mat canvasObjectImage[CANVAS_OBJECTS_TYPES_MAX];

static void loadCanvasObjectsIcons(void)
{
    for (short i = 0 ; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        canvasObjectImage[i] = cv::imread(canvasObjectImagePath[i]);
    }
}

static void createCanvasObjectsImagesTexIds(void)
{
    short int i = 0;
    for (i = 0; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        canvasObjectImageTexId[i] = glConvertMatToTexture(canvasObjectImage[i]);
    }
}

static void cleanCanvasObjectsImagesTexIds(void)
{
    short int i = 0;
    for (i = 0; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
    {
        if (canvasObjectImageTexId[i] != 0)
            glDeleteTextures(1, &canvasObjectImageTexId[i]);
    }
}



int main(int argc, char * argv[])
{
    // Engine();
    using std::cout;
    using std::endl;

    // VideoWriterg
    cv::VideoWriter oVideoWriter;//create videoWriter object, not initialized yet
    ClipReader aClipReader; // create a ClipReader object
    pClipReader = &aClipReader;

    // to be added to out filenames, whn recording
    int inc = 0;

    // class CaptureDev. Private
    bool b_Once = false;

    // class GUI. Private members
    bool b_display_delayed = false;

    // LATER. Needed ?
    // Class GUI
    bool b_is_fullscreen = false;

    // class CaptureDev. Public
    bool b_recording_video = false;

    bool b_full_video_selected = false;
    bool b_cell_phone_selected = false;

    // class GUI
    bool b_recording_checked = false;
    bool b_startNewRecording = false;
    bool b_draw_recording_info    = false;
    //bool b_draw_help         = false;
    bool b_chrono            = false;
    bool b_record_chrono    = false;


    // class GUI
    bool b_display_activity_time = false;
    bool b_display_total_time = false;
    bool b_zoom_available = false;
    loadCanvasObjectsIcons();

    int lastFrameNumber = pClipReader->getMaxFrame();

    // pClipReader->setCurrentFrameNumber(1);
    // + eventualy :
    // int currentFrameNumber = pClipReader->getCurrentFrameNumber();
    int currentFrameNumber = 1;

    // ******************************************************
    // CaptureDev::init();
    // ******************************************************

    // TESTING
    CaptureDev captureDev;
    CaptureDev & pCaptureDev = captureDev;
    cv::VideoCapture cap2 = pCaptureDev.maVideoCap;
    cv::VideoCapture * pCap2 = &cap2;

    // classe CaptureDev. Private
    int delay = captureDev.get_delay(); // with CaptureDev Ctor, default value is 1s

    // FIXME : really usefull ?
    int fdelay = 1.0f;

    double fps = captureDev.getCaptureFPS();
    std::cout << "fps : " << fps << std::endl;

    if (fps < 5.0) /* Houston, we have a problem */
        fps = DEFAULT_FRAMERATE;

    // delay::stack_size
    short int stack_size = DELAY_MIN * DEFAULT_FRAMERATE;

    // keep a trace of the previous delay
    // delay::delay_ref

    short int delay_ref = delay;
    double dWidth = cap2.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap2.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    if (dWidth * dHeight == 0)
    {
        dWidth  = CAPTURED_FRAME_WIDTH;
        dHeight = CAPTURED_FRAME_HEIGHT;
    }

    cv::Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));


    // SDL engine + OpenGL, basicaly needed by Dear ImGui
    ////Engine  engine;

    putenv((char *)"SDL_AUDIODRIVER=alsa");

    // about issues between OpenGL 3.3. and OpenGL 3.0 (only Linux concerned)
    // https://discourse.libsdl.org/t/confused-about-what-opengl-context-is-being-used-with-sdl/22860
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // removed for TESTS, and no apparent change at least on Linux ...
    // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // avoid using compatiblity profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // 3.3 core profile should be sufficient (true on Windows)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_DisplayMode  current;
    SDL_GetCurrentDisplayMode(0, &current);

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );


    // 1 == enable VSync ; 0 == disable VSync
    #define USE_VSYNC
    #ifdef USE_VSYNC
    SDL_GL_SetSwapInterval(1);
    #else
    SDL_GL_SetSwapInterval(0);
    #endif

    SDL_Window * window = nullptr;

    window = SDL_CreateWindow("miniDart",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

    // check whether the SDL2 window exists
    if (window == nullptr)
        sdl_application_abort("Problem creating the SDL window.\n");
    else
        std::cout << "SDL2 Window created " << std::endl;

    std::cout <<  "SDL_VERSION_ATLEAST(2,0,9) " << SDL_VERSION_ATLEAST(2,0,9) <<  std::endl;
    std::cout <<  "SDL_VERSION_ATLEAST(2,0,8) " << SDL_VERSION_ATLEAST(2,0,8) <<  std::endl;
    std::cout <<  "SDL_VERSION_ATLEAST(2,0,7) " << SDL_VERSION_ATLEAST(2,0,7) <<  std::endl;
    std::cout << "SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE    SDL_VERSION_ATLEAST(2,0,4) = " << SDL_VERSION_ATLEAST(2,0,4) << std::endl;
    std::cout << "SDL_HAS_WINDOW_ALPHA                SDL_VERSION_ATLEAST(2,0,5) = " << SDL_VERSION_ATLEAST(2,0,5) << std::endl;
    std::cout << "SDL_HAS_ALWAYS_ON_TOP               SDL_VERSION_ATLEAST(2,0,5) = " << SDL_VERSION_ATLEAST(2,0,5) << std::endl;
    std::cout << "SDL_HAS_USABLE_DISPLAY_BOUNDS       SDL_VERSION_ATLEAST(2,0,5) = " << SDL_VERSION_ATLEAST(2,0,5) << std::endl;
    std::cout << "SDL_HAS_PER_MONITOR_DPI             SDL_VERSION_ATLEAST(2,0,4) = " << SDL_VERSION_ATLEAST(2,0,4) << std::endl;
    std::cout << "SDL_HAS_VULKAN                      SDL_VERSION_ATLEAST(2,0,6) = " << SDL_VERSION_ATLEAST(2,0,6) << std::endl;
    std::cout << "SDL_HAS_MOUSE_FOCUS_CLICKTHROUGH    SDL_VERSION_ATLEAST(2,0,5) = " << SDL_VERSION_ATLEAST(2,0,5) << std::endl;

    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
    //  seems to work very well. Just an issue : how to handle the window and move it ? (ALT + is suboptimal)
    //    SDL_SetWindowBordered(window, SDL_FALSE);

    const SDL_GLContext & glcontext = SDL_GL_CreateContext(window);

    if (glcontext == nullptr)
        sdl_application_abort("Problem creating GL context.\n");

    if (gl3wInit() != 0)
    {
        SDL_GL_DeleteContext(glcontext);
        SDL_Quit();
        throw std::runtime_error("Failed to initialize OpenGL\n");
    }
    getInfo();

    // usefull ?
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEPTH_TEST);

    int drawable_width, drawable_height;
    SDL_GL_GetDrawableSize(window, &drawable_width, &drawable_height);

    //// SDL_Audio
    int i = 0;
    int count = 8; //SDL_GetNumAudioDevices(2);
    for (i = 0; i < count; ++i) {
        fprintf(stdout, "Audio device (no recording capability)   %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
        fprintf(stdout, "Audio device (with recording capability) %d: %s\n", i, SDL_GetAudioDeviceName(i, 1));
    }

    std::cout << "Found " << md::getSDLRecordableAudioDeviceNames() << " recordable audio devices." << std::endl;

    //Get capture device count
    int nbrRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

    fprintf(stdout, "Number of Audio devices (with recording capability) %d \n",nbrRecordingDeviceCount);

    const char* driver_name = SDL_GetCurrentAudioDriver();

    if (driver_name) {
        printf("Audio subsystem initialized; driver = %s.\n", driver_name);
    } else {
        printf("Audio subsystem not initialized.\n");
    }

    //  GUI::Init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    std::cout << "io =  " << &io << std::endl;

    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Multi-ViewPorts
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-ViewPorts

    ImGuiStyle& style = ImGui::GetStyle();

    ImGui::StyleColorsLightGreen();  // back to this theme as default

    std::cout << "style =  " << &style << std::endl;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        std::cout << "Viewport enabled " << std::endl;
        style.WindowRounding = 0.0f;
    }

    //ImGui_ImplSDL2_InitForOpenGL(engine.window, engine.glcontext);
    ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
    ImGui_ImplOpenGL3_Init();

    Timer aTimer;

    //Application::LoadFonts(). Loads Fonts. See extra_fonts/README.txt for more details)
    //    io.Fonts->AddFontDefault();

    // load only the font we need (add other fonts later ?)
    io.Fonts->AddFontFromFileTTF(DEFAULT_APPLICATION_FONT, DEFAULT_APPLICATION_FONT_SIZE);

    ImFontConfig config;
    config.MergeMode = true;

    // merge in icons from Font Awesome
    static const ImWchar fa_icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    static const ImWchar md_icons_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
    config.PixelSnapH = true;

    io.Fonts->AddFontFromFileTTF("./fonts/Icons/FontAwesome/FontAwesome.ttf", 19.0f, &config, fa_icons_ranges);
    io.Fonts->AddFontFromFileTTF("./fonts/Icons/MaterialDesign/font.ttf", 38.0f, &config, md_icons_ranges);

    ImVec4 clear_color = ImColor(114, 144, 154);

    // CaptureDev::b_paused
    bool b_paused = false;

    // Application
    bool quit = false;

    // TODO : implement the eventManager class
    // Application::sdl_event
    SDL_Event event;

    // FIFO stack. And yes, this is a lot of RAM !
    std::queue<cv::Mat> cb;

    // cv::frame *  pCaptureDev->frame ...  etc
    cv::Mat frame;
    cv::Mat frame2;

    // Application::displayed_frame : stands in main loop, as cv::frame (the one to be drawn)
    cv::Mat displayed_frame;

    // Application::init()
    md::TEXT_Object  aTextObject;
    md::TEXT_Object* pTextObject = &aTextObject;
    md::TextCanvas   aTextCanvas(pTextObject);

    md::TextCanvas * pTextCanvas = &aTextCanvas;

    DrawnObject * p_aDrawnObject;
    std::vector <DrawnObject> * p_delayTabDrawnObjects;

    // Application::Loop()
    while (!quit)
    {
        // Application::onEvent
////////////////////////////////////////////////
        int fpsCapStartTicks = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            // ImGui keyboard events
            ImGui_ImplSDL2_ProcessEvent(&event);

            // Application::onEvent()
            if ((b_video_running)&&(!b_audio_paused))
            {
                SDL_PumpEvents();
                double remaining_time = 0.0f; 
                video_refresh(is, &remaining_time);
            }
            switch (event.type) 
            {
                case SDL_WINDOWEVENT:
                {
                    switch(event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:

                            // faster than SDL_GetWindowSize(window, &currentWidth, &currentHeight);
                            currentWidth = event.window.data1;
                            currentHeight = event.window.data2;
                        break;

                        default:
                        break;
                    }
                }
                    break;

                case SDL_DROPFILE:
                break;

                default:
                break;
            }

            switch (event.key.type)
            {
                case SDL_QUIT:
                  quit = true;
                    break;

                // first : detect SDL_KEYDOWN *then* switch(event.key.keysym.sym)
                // without that, nothing would occur
                case SDL_KEYDOWN:

                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    // will be used for menu navigation
                    break;

                    // kept as example
                    case SDLK_SPACE:
                    {
                        if (!b_inserting_text)
                        {
                            switch (current_tab)
                            {
                                case ANALYSIS_TAB:
                                    b_playing_at_fps = !b_playing_at_fps;
                                break;

                                case DELAY_TAB:
//8 mai
                                    if ((b_video_running) && ((position - origin) < (video_duration - 0.2f)))
                                    {
                                        b_audio_paused = !b_audio_paused;
                                        SDL_PauseAudioDevice(audio_dev, b_audio_paused);

                                        std::cout << "b_audio_paused =  " << b_audio_paused << std::endl;
                                        std::cout << "position -origin : " << position - origin << std::endl;
                                        std::cout << "video_duration - 0.2f : " << video_duration - 0.2f << std::endl;
                                }
                                break;

                                default:
                                    break;
                            }
                        }
                    }
                    break;

                    case SDLK_UP:
                    case SDLK_RIGHT:
                    {
                        if (event.key.keysym.mod & prefered_modifier)
                        {
                            switch (current_tab)
                            {
                                case DELAY_TAB:
                                {
                                    if ((delay < DELAY_MAX) && (!b_recording_video))
                                    {
                                        delay++;
                                        fdelay = delay;
                                    }
                                }
                                break;

                                default:
                                    break;
                            }
                        }
                        else
                        {
                            switch (current_tab)
                            {

                                case ANALYSIS_TAB:
                                    if ((currentFrameNumber < lastFrameNumber-1))
                                        currentFrameNumber++;
                                  break;

                                default:
                                    break;
                            }
                        }
                    }
                    break;

                    case SDLK_DOWN:
                    case SDLK_LEFT:
                    {
                        if (event.key.keysym.mod & prefered_modifier)
                        {
                            switch (current_tab)
                            {
                                case DELAY_TAB:
                                {
                                    if ((delay > 1) && (!b_recording_video))
                                    {
                                        delay--;
                                        fdelay = delay;
                                    }
                                }
                                  break;

                                default:
                                    break;
                            }
                        }
                        else
                        {
                            switch (current_tab)
                            {
                                case ANALYSIS_TAB:
                                    if ((currentFrameNumber > 1))
                                        currentFrameNumber--;
                                  break;

                                default:
                                    break;
                            }
                        }
                    }
                    break;

                    case SDLK_d:
                    {
                        // shortcut = MODIFIER + d to avoid clash when entering URL or any string
                        if ((event.key.keysym.mod & prefered_modifier) && (!b_inserting_text))
                        {
                            switch (current_tab)
                            {
                                case DELAY_TAB:
                                {
                                    b_display_delayed = !b_display_delayed;

                                    if (b_display_delayed == false)
                                        delay = fdelay = 1;
                                }
                                break;

                                default:
                                  break;
                            }
                        }
                    }
                    break;

                    // TRUE for all modes ?
                    case SDLK_f:
                    {
                        if ((event.key.keysym.mod & prefered_modifier)&&(!b_inserting_text))
                        {
                            //DISABLED : will be back once the FilePicker issue will progress
                            // IDEA : display the image in fullscreen ?
                            b_is_fullscreen = !b_is_fullscreen;
                            short int aDesktop_mode = (b_is_fullscreen == true) ? SDL_WINDOW_FULLSCREEN_DESKTOP : false;
//                            SDL_SetWindowFullscreen(engine.window, aDesktop_mode);
                            SDL_SetWindowFullscreen(window, aDesktop_mode);
                            // to avoid unstabilities when switching fullscreen <==> windowed mode
                            SDL_Delay(300);
                        }
                    }
                    break;

                    case SDLK_p:
                    {
                        switch(current_tab)
                        {
                            case DELAY_TAB:
                            {
                                if ((!b_recording_video) && (!b_video_running) && (!b_inserting_text) )
                                    b_paused = !b_paused;
                            }
                              break;

                            default:
                              break;
                        }
                    }
                    break;

                    case SDLK_q:
                        if ((current_tab == DELAY_TAB)&&(b_video_running)&&(!pClipReader->getCurrentClipFile()))
                        {
                            if ((event.key.keysym.mod) && (!b_inserting_text))
                            {
                                cout<<"do exit"<<endl;
                                b_video_running = false;
                                do_exit(is);
                            }
                        }

                        if (event.key.keysym.mod & KMOD_CTRL)
                            quit = true;
                    break;

                    case SDLK_r:
                    {
                        if ((event.key.keysym.mod & prefered_modifier))
                        {
                            switch(current_tab)
                            {
                                case DELAY_TAB:
                                {
                                    if (!b_paused)
                                        b_recording_checked = !b_recording_checked;
                                }
                                  break;

                                default:
                                  break;
                            }
                        }
                    }
                    break;

                    case SDLK_s:// Very soon, we'll save an image using that (+ prefered_modifier)
                    default:
                    break;
                }
            }
        }

///////////////////////////////////////////////////////////////////
/*
    // recorded objects

    1. create the video recorder
    2. select the frame to be displayed, its source, plus the eventual delay
    3. draw recorded ext
    4. draw recorded rectagle
    5. draw recorded chrono
    6. draw recorded panel
    7. add the frame to the vidéo

    // not recorded (only displayed on overlay or whatever)
    8. draw "REC"  when recording
    9. draw the delay value (only when not recording)
   10. draw the magnifier rectangle (if defined)
*/
///////////////////////////////////////////////////////////////////

            // CREATE THE VIDEO RECORDER, WHEN NEEDED
        static ImVec2 topLeft = ImVec2( 0.0f, 0.0f);
        static ImVec2 bottomRight = ImVec2( 0.0f, 0.0f);
        static ImVec2 * p_topLeft = &topLeft;
        static ImVec2 * p_bottomRight = &bottomRight;

        // if (! Application::images_paused)
        if (!b_paused)
        {
            // SELECT THE IMAGES SOURCE FIRST

            // should be the only case, no ?
            //if ((current_source == FULL_VIDEO) || current_source == NETWORK )
            if ( !(current_source == USB_WEBCAM))
            {
                // FULL_VIDEO, IP_WEBCAM, CELL_PHONE provide ffmpeg images == videoMat cv::Mat
                if ((!videoMat.empty())&&(b_video_running))
                    frame2 = videoMat.clone();
            }
            else
            {
                // FIXME : broken
#ifdef FPS_TEST
//                getUSBFrame(captureDev, frame2);
                static unsigned int old_read_frame_time = 0;
                static unsigned int current_read_frame_time = 0;
#endif
                if (b_use_USB_webcam)
                {
                    if (b_never_initialized_webcam)
                    {
                        resetImageSource(pCap2, USB_WEBCAM, NULL, DEFAULT_DEVICE_NUMBER, CAPTURED_FRAME_WIDTH, CAPTURED_FRAME_HEIGHT, DEFAULT_FRAMERATE);
                        b_never_initialized_webcam = false;
                    }

                    if (pCap2->isOpened())
                    {
                        static bool bSuccess2 = false;
#ifdef FPS_TEST
                        current_read_frame_time = SDL_GetTicks();
                        //if (pCaptureDev.canReadFrame(fps_test, current_read_frame_time, old_read_frame_time))
                        if ( (current_read_frame_time - old_read_frame_time) >=  (unsigned int)(1000 / fps_test))
                        {
                            bSuccess2 = pCap2->read(pCaptureDev.USBFrame);
                            old_read_frame_time = current_read_frame_time;

                            frame2 = pCaptureDev.USBFrame.clone();
                        }
#else
                        bSuccess2 = pCap2->read(pCaptureDev.USBFrame);
                        frame2 = pCaptureDev.USBFrame.clone();
#endif
                        if (!bSuccess2)
                            std::cerr << "Pb with pCap2->read(pCaptureDev.USBFrame)" << std::endl;
                    }
                }
            }

            // IS THE FRAME AVAILABLE ? normal case : yes it is
            bool b_phone_waiting_frames = false;

            // CaptureDev::fillBuffer() : fill the buffer ?
            if ((!b_Once) && (b_display_delayed))
            {
                if (b_cell_phone_selected)
                {
                    if (!frame2.empty())
                        b_phone_waiting_frames = false;
                    else
                        b_phone_waiting_frames = true;
                }

                if (!b_phone_waiting_frames)
                {
                    short int index = 0;

                    for (index=0 ; index < stack_size ; index++)
                    {
                        while(frame2.empty())
                        {
                            std::cout << "waiting for frame ..." << std::endl;
                            index++; // avoids a dead lock, and allows missing frames

                            if (index > 300)
                                break;
                        }
                        cb.push (frame2);
                    }
                    b_Once = true;
                }
            }

// DISPLAY WITH A DELAY

            // the buffer is now full, so let's copy the frames on the screen
            if ((b_display_delayed)&&(!b_phone_waiting_frames))
            {
                if (!cb.empty ())
                    frame = cb.front();

                cb.pop();
                cb.push (frame2);
            }
            else
                frame = frame2;

#ifndef FPS_TEST
            if (!frame2.empty())
                frame2.release();
#endif
            // Application::Update()
            if (!frame.empty())
            {
                if (b_flip_image)
                    flip(frame, frame, 1);

                switch (image_rotation_value)
                {
                    // default. Should be the faster path
                    case NO_ROTATION:
                        scale = 1.0f;
                        ratio = frame_height/frame_width;
                    break;

                    case ROTATE_90_CLOCKWISE:
                        ratio = frame_width/frame_height;
#ifdef TEST_SCALE
                       scale = (frame_height/frame_width)*(frame_height/frame_width);
#else
                       scale = 1.0f;
#endif
                        transpose(frame, frame);
                        flip(frame, frame,1); //transpose+flip(1)=CW
                    break;

                    case ROTATE_90_COUNTERCLOCKWISE:
                        ratio = frame_width/frame_height;
#ifdef TEST_SCALE
                       scale = (frame_height/frame_width)*(frame_height/frame_width);
//                       scale = 0.31f;
#else
                       scale = 1.0f;
#endif
                        transpose(frame, frame);
                        flip(frame, frame, 0); //transpose+flip(0)=CCW
                    break;

                    case ROTATE_180:
                        ratio = frame_height / frame_width;
//                        ratio = 720.0f / 1280.0f;
                        scale = 1.0f;
                        flip(frame, frame,-1);  //flip(-1)=180
                    break;

                    default:
                    break;
                }
                displayed_frame = frame.clone();
                frame.release();
            }
// END DISPLAY WITH A DELAY


// DRAW RECORDED TEXT

            // TODO : draw a vector of textObjets

            // add freetype magic // video incrustation of text
            if ( (pTextCanvas->pTextObject->b_displayable == true) && (!displayed_frame.empty()))
            {
                pTextCanvas->insertString(displayed_frame, pTextCanvas->pTextObject);

                if ((pTextCanvas->pTextObject->frameCount >= 0) && (!pTextCanvas->pTextObject->b_unlimited_annotation))
                    pTextCanvas->pTextObject->frameCount--;
#if defined( DEBUG_TEXT_CANVAS )
                fprintf( stdout, " pTextCanvas->pTextObject->frameCount = %d \n", pTextCanvas->pTextObject->frameCount);
#endif
                if (pTextCanvas->pTextObject->frameCount < 0)
                {
                    pTextCanvas->pTextObject->b_displayable = false;
                    pTextCanvas->stopStringIncrustation();
                }

                //  DRAG the TEXT !!
                if ((pTextCanvas->textBoxHovered(displayed_frame, ImVec2(pTextCanvas->image_pos.x, pTextCanvas->image_pos.y)) == true) && (ImGui::IsMouseDragging(0, 0.0f)))
                    pTextCanvas->move();
            }

// END DRAW TEXT


// DRAW RECORDED RECTANGLE
            if ((b_record_rectangle) && (!displayed_frame.empty()))
            {
                // we need to define a minimal area, to avoid displaying " ghost rectangles"
                if ((abs(p_bottomRight->x - p_topLeft->x) > 10) && (abs(p_bottomRight->y - p_topLeft->y) > 10))
                {
                    // 0.82f, 45.0f and 28.0f are "magics" due to a crappy layout.  FIXME
                    static int rBase   = (0.82f * currentWidth) - WEBCAM_PARAMETERS_AREA_WIDTH - 45.0f;
                    static int rWidth  = rBase - 28.0f;
                    static int rHeight = rBase * ratio;
                    cv::rectangle(  displayed_frame,
                                    cv::Point((p_topLeft->x/rWidth)*displayed_frame.cols,(p_topLeft->y/rHeight)*displayed_frame.rows),
                                    cv::Point((p_bottomRight->x/rWidth)*displayed_frame.cols, (p_bottomRight->y/rHeight)*displayed_frame.rows),
                                    cv::Scalar(255, 255, 255, 255),
                                    2,
                                    8);
                }
            }
// END DRAW RECORDED RECTANGLE

// DRAW RECORDED CHRONO
            if (b_record_chrono && b_chrono)
                putText(displayed_frame,TIME_DISPLAY+floatToString((unsigned long int)aTimer.getActivityTime()/1000.0f)+" s",cv::Point(60,WINDOW_HEIGHT - 70), 2 /*FONT_HERSHEY_COMPLEX ou 1 */,/*1.5*/1.0,ANNOTATIONS_COLOR_YELLOW, 2);
// END DRAW RECORDED CHRONO

// DRAW RECORDED PANEL

            // TODO : IMPLEMENT ME !
            //     _________________
            //     |  PSG     12   |
            //     |---------------|
            //     |  MHBA    11   |
            //     |---------------|
            //     |               |
            //     |  MT1 : 12:45  |
            //     |_______________|
            //
            // Draw : a retangle + ghost window to draw/drop
            // Select colors
            //
            // default horizontal/vertical values : x0 = 5, y0 = 5
            // hovering + right click = open parameters
            // PARAMETERS :
            // move <--> or |
            // Enter teams names
            // Select colors
            // Background
            // Font and font color for chronometer
            // Font for teams and score
            // Color for club (rectangle
            // HALF TIME
            // time in progress + pause/play
            // Add chronometer 
            // be able to change the time + be able to select the half time

// END DRAW RECORDED PANEL


///////////////////  RECORD VIDEO ////////////////////////////////////////////

            static unsigned int recorded_old_time = 0;
            static unsigned int recorded_current_time = 0;

// CREATE THE NEW VIDEO
            // create a new VideoWriter + a new video name ?

            // 

            if ((b_startNewRecording == true) && (strlen(defaultPath) !=  0))
            {
                oVideoWriter  = cv::VideoWriter((std::string)defaultPath+UNIX_PATH_SEPARATOR+OUTPUT_FILENAME_PREFIX+intToString(inc)+myContainer,
                                             // For example : CV_FOURCC('M', 'J', 'P', 'G'),
                                             myFourcc,
                                             outFPS, ///*LINUX_DEFAULT_FRAMERATE*//* 24 fps */,
                                             outFrameSize,
                                             true
                                           );

                if ( !oVideoWriter.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
                    cout << "ERROR: Failed to initialize video writing" << endl;
                else
                {
                    std::cout << "New Recording Started at " <<  outFPS << " images / second" << std::endl;
                    b_startNewRecording = false;
                    cout<<"New video file created MyVideo"+intToString(inc)+myContainer<<endl;
                    inc++;
                }
            }

// VIDEO CREATED (or failed)

// RECORD THE RIGHT FRAME : if we're in recording mode, we write to file here
            if ((b_recording_video) && (!b_audio_paused))
            {
                recorded_current_time = SDL_GetTicks();

                if (recordAtGivenFPS(outFPS, recorded_current_time, recorded_old_time))
                {
                    oVideoWriter.write(displayed_frame);
                    recorded_old_time = recorded_current_time;
                }
            }
// DISPLAYED FRAME RECORDED

///////////////////  END RECORD VIDEO ////////////////////////////////////////////


//////////////////  WHAT FOLLOWS WILL NOT BE RECORDED, JUST DRAWN  ///////////////////////////

            // DELAY MODIFIED ?
            if (delay_ref != delay)
            {
                //the user modified the delay, so the stack has to be cleaned up
                delay_ref = delay;
                stack_size = delay*fps;

                while (!cb.empty())
                {
                    cb.pop();
                }
                b_Once = false;
            }

            /**************  do the OpenCV drawing job here ************/

            // Application::Render()
            // First draw a grey-white background
            //SDL_SetRenderDrawColor(renderer, 214, 214, 214, 255);

            // Now draw figures (lines, polygons, text the user drawn on the saved image appears on the screen)
            //if (b_draw_vector_of_annotations){ // IMPLEMENT ME : render_annotations_vector() }

            // REC appears on top left of the screen when recording a video
            if (b_draw_recording_info)
            {
                cv::Scalar aColor = selectColor(aValue);
                putText(displayed_frame,RECORD_DISPLAY,cv::Point(40,60),2,2,aColor,2);
            }

            // TODO : use a timer, and display "delayed  x seconds" only 2 seconds, to avoid noise with the recorded video
            // e.g. :  start record set the delay, stop record reset it, and the message will be displayed 1 or two seconds only
            // the goal is to display this message only : when not recording, nor paused or only the two fisrt seconds when recording a delayed video

            if((b_display_delayed) && (!b_draw_recording_info) && (!b_recording_video))
                putText(displayed_frame,DELAY_DISPLAY+intToString(delay)+"s",cv::Point(WINDOW_WIDTH - 430,WINDOW_HEIGHT - 70),2,1.0/*1.5*/,ANNOTATIONS_COLOR_YELLOW, 2);

        }   /* if (!b_paused) */

//////////////////  WHAT FOLLOWS WILL NOT BE RECORDED, BUT JUST DRAWN  ///////////////////////////


        // AT THE END, fill OpenGL buffers

        if (!displayed_frame.empty())
        {
            // Application::convertMatToTexture()
            if ( ((b_lostWebcam) || (b_paused)) && (!b_full_video_selected) )
                backgroundTextureId = glConvertMatToTexture(backgroundImage);
            else
                backgroundTextureId = glConvertMatToTexture(displayed_frame);

            if (!displayed_frame.empty())
                displayed_frame.release();
        }
        else
            backgroundTextureId = glConvertMatToTexture(backgroundImage);

        /// GUI::Draw();
        ImGui_ImplOpenGL3_NewFrame();
        // ImGui_ImplSDL2_NewFrame(engine.window);
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        /// GUI::background();
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

        static bool open = true;
        ImGuiWindowFlags mD_window_flags = 0;

        // The points for the canvas
        static ImVector<ImVec2> points;

        // for testing purpose : will be simplified soon
        static bool no_titlebar = true;
        static bool no_resize = true;
        static bool no_move = true;
        static bool no_scrollbar = false;
        static bool no_collapse = true;
        static bool menu = false;

        if (no_titlebar)  mD_window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (no_resize)    mD_window_flags |= ImGuiWindowFlags_NoResize;
        if (no_move)      mD_window_flags |= ImGuiWindowFlags_NoMove;
        if (no_scrollbar) mD_window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (no_collapse)  mD_window_flags |= ImGuiWindowFlags_NoCollapse;
        if (menu)         mD_window_flags |= ImGuiWindowFlags_MenuBar;

        // some geometry
        ImVec2 windowSize(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 23.0f);
        ImGui::SetNextWindowSize(windowSize);

        ImVec2 main_viewport_pos = ImGui::GetMainViewport()->Pos;
        ImGui::SetNextWindowPos(ImVec2(main_viewport_pos.x,main_viewport_pos.y + 23.0f));

        if (ImGui::Begin(APPLICATION_VERSION_STRING, &open, mD_window_flags))
        {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu(ICON_FA_FOLDER_OPEN_O FILE_OPEN_MENU_ENTRY))
            {
                ShowMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(EDITION_MENU_ENTRY))
            {
                if (ImGui::MenuItem(UNDO_MENU_ENTRY, UNDO_MENU_ENTRY_SHORTCUT)) {}
                if (ImGui::MenuItem(REDO_MENU_ENTRY, REDO_MENU_ENTRY_SHORTCUT, false, false)) {}  // Disabled item
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(MODE_MENU_ENTRY))
            {
                ImGui::Separator();
                if (ImGui::MenuItem(CUT_MENU_ENTRY,   CUT_MENU_ENTRY_SHORTCUT)) {}
                if (ImGui::MenuItem(COPY_MENU_ENTRY,  COPY_MENU_ENTRY_SHORTCUT)) {}
                if (ImGui::MenuItem(PASTE_MENU_ENTRY, PASTE_MENU_ENTRY_SHORTCUT)) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(THEME_MENU_ENTRY))
            {
                // initialization
                int selected_menutheme = current_theme;

                if (ImGui::Selectable(TRADITIONAL_GREEN_THEME_MENU_ENTRY))
                    selected_menutheme = LIGHT_GREEN_THEME;

                if (ImGui::Selectable(DARK_THEME_MENU_ENTRY))
                    selected_menutheme = DARK_THEME;

                if (ImGui::Selectable(CLASSIC_THEME_MENU_ENTRY))
                    selected_menutheme = CLASSIC_THEME;

                if (ImGui::Selectable(LIGHT_BLUE_THEME_MENU_ENTRY))
                    selected_menutheme = LIGHT_BLUE_THEME;

                if (ImGui::Selectable(WINDOWS_THEME_MENU_ENTRY))
                    selected_menutheme = WINDOWS_THEME;

                if (selected_menutheme != current_theme)
                    set_theme(selected_menutheme);

                ImGui::EndMenu();
            }


            // Gui::addImagesSourceMenuEntry();
            if ((current_tab == DELAY_TAB) && (!b_video_running))
            {
                if (ImGui::BeginMenu(IMAGES_SOURCE_CHOICE_MENU_ENTRY))
                {
                    // initialization
                    static int selected_source = current_source;
                    //if (b_use_USB_webcam)
                    {
                        if (ImGui::Selectable("  " ICON_FA_VIDEO_CAMERA "  " INPUT_SOURCENAME_USB_WEBCAM ))
                        {
                            selected_source = USB_WEBCAM;
                            b_use_USB_webcam = true;
                        }
                    }

                    if (ImGui::Selectable("  " ICON_FA_FILM "  " INPUT_SOURCENAME_VIDEO              ))
                        selected_source = FULL_VIDEO;

                    if (ImGui::Selectable("  " ICON_FA_WIFI " " INPUT_SOURCENAME_IP_WEBCAM          ))
                        selected_source = IP_WEBCAM;

                    if (ImGui::Selectable("  " ICON_FA_PHONE_SQUARE "  " INPUT_SOURCENAME_CELL_PHONE ))
                        selected_source = CELL_PHONE;

                    if (current_source != selected_source)
                        current_source = selected_source;

                    ImGui::EndMenu();
                }
            }

//            ImGui::Dummy(ImVec2(40.0f,0.0f)); ImGui::SameLine();

            if (ImGui::Button(ABOUT_BUTTON))
                ImGui::OpenPopup(ABOUT_MINIDART);

            if (ImGui::BeginPopupModal(ABOUT_MINIDART, NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::SetItemDefaultFocus();
                ImGui::Text(PRESENTATION_STRING1);
                ImGui::Text(LINUX_PRESENTATION_STRING2);
                ImGui::Text(LINK_PRESENTATION_STRING3);
                ImGui::NewLine();
                ImGui::Text(AUTHOR_PRESENTATION_STRING4);
                ImGui::Text(LICENSE_PRESENTATION_STRING5);
                ImGui::NewLine();

                ImGui::Dummy(ImVec2(100.0f,0.0f)); ImGui::SameLine();

                if (ImGui::Button(OK_BUTTON, ImVec2(200,0)))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }
            ImGui::EndMainMenuBar();
        }

///////////////////////////////////////////////////////////////////////////////////////////////
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None|ImGuiTabBarFlags_Reorderable;

        if(ImGui::BeginTabBar(TABBAR_NAME,tab_bar_flags))
        {

        if (ImGui::BeginTabItem( ICON_FA_VIDEO_CAMERA " " ICON_FA_SLIDERS  DELAY_TAB_NAME))
        {
            // avoid modifying the delay in another tab
            current_tab = DELAY_TAB;

            // Visible from DELAY_TAB ONLY
            static int current_delayTab_drawing_task = DRAWING_NOTHING;
            static int selectedObject = NOT_A_DRAWN_OBJECT; // default

            // with that, we can display every frame at the right time
            if ((b_video_running)&&(!b_audio_paused))
            {
                aDummyEvent.type = FF_REFRESH_EVENT;
                SDL_PushEvent(&aDummyEvent);
            }

            // TODO FIXME UGGLY (and probably PLAIN WRONG)
            float w = 0.82*currentWidth - WEBCAM_PARAMETERS_AREA_WIDTH - 45.0f;//- 50.0f /* be sure it will fit */;
            float w2 = w + WEBCAM_PARAMETERS_AREA_WIDTH + 9.0f;
#ifdef TEST_SCALE
            if ( (image_rotation_value %2) == 0 )
                w = scale * w;
#endif
            float h = w * ratio + 58.0f/* don't forget the chrono */;
            float h2 = currentHeight - h - 80.0f;// was -85.0f

            static ImVec4 col = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
            ImU32 color = ImColor(col);
            static float outline_thickness = 2.5f;

            pTextCanvas->image_size.x = w;
            pTextCanvas->image_size.y = h;

            ImGui::BeginGroup();  // DEBUT1
            {
                ImGui::Dummy (ImVec2(80.0f, 0.0f)); ImGui::SameLine();
                //------------------------------------------------
                ImGui::BeginChild("child1", ImVec2(w + 40.0f, h), true);
// CHILD1 : DRAW THE FRAME displayed_image OR DRAW DEFAULT  BACKGROUND 

                ImDrawList * draw_list = ImGui::GetWindowDrawList();
                pTextCanvas->image_pos = ImGui::GetCursorScreenPos();
                ImVec2 subview_size = ImGui::GetContentRegionAvail();
                //ImVec2 subview_size = pTextCanvas->image_size;
                static bool adding_rect = false;
                //static bool adding_rect2 = false;

#define CLASSICAL_DRAW_IMAGE
#ifdef  CLASSICAL_DRAW_IMAGE
                ImGui::Image(reinterpret_cast<void * >(backgroundTextureId), ImVec2(w - 28.0f, w*ratio), ImVec2(0,0), ImVec2(1,1),
                           ImColor(255,255,255,255), ImColor(255,255,255,255));
#else
                // Kept because interesting (saves time in runtime, bcause rendering once). Does the same, but there is no white boreder drawn.
                // Indeed, the second color when using ImGui::Image instead, means a white frame is drawn around the image)
                draw_list->AddImage(reinterpret_cast<void * >(backgroundTextureId), pTextCanvas->image_pos, pTextCanvas->image_pos + ImVec2(w - 28.0f, w*ratio), ImVec2(0.0f,0.0f), ImVec2(1.0f,1.0f),
                             IM_COL32(255,255,255,255));
#endif
                ImGui::EndChild();

// END CHILD1 : DRAW THE FRAME displayed_image OR DRAW DEFAULT  BACKGROUND 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

// CHILD11 : NOW DRAW OVER THE FRAME

                //------------------------------------------------
                ImGui::BeginChild("child1");

                ImVec2 mouse_pos_in_image = ImVec2(ImGui::GetIO().MousePos.x - pTextCanvas->image_pos.x,  (ImGui::GetIO().MousePos.y - pTextCanvas->image_pos.y));

                static bool adding_preview1 = false;
                static bool adding_rect2 = false;
                static bool adding_preview2 = false;

                static ImVector <ImVec2> arrow_points;

                static DrawnObject aDrawnObject;
                p_aDrawnObject = & aDrawnObject;

                static std::vector <DrawnObject> delayTabDrawnObjects;
                p_delayTabDrawnObjects = &delayTabDrawnObjects;

                if (current_delayTab_drawing_task == DRAWING_ZOOMED_AREA)
                    aDrawnObject.anObjectType = NOT_A_DRAWN_OBJECT;
                else
                    aDrawnObject.anObjectType = selectedObject;

                float P1P4 = 0.1f;
                float arrowLength = 5 * aDrawnObject.thickness;
                float arrowWidth = 3 * aDrawnObject.thickness;

                ///////////////////////////////////////////////////////////////////////////////////////////////
                //                               CATCH THE POINTS TO BE DRAWN                                //
                ///////////////////////////////////////////////////////////////////////////////////////////////

                if ( mouse_pos_in_image.x < 0 )
                    mouse_pos_in_image.x = LEFT_IMAGE_BORDER;

                if (( mouse_pos_in_image.y < 0 ))
                    mouse_pos_in_image.y = TOP_IMAGE_BORDER;

                if ( mouse_pos_in_image.x > (RIGHT_IMAGE_BORDER) )
                    mouse_pos_in_image.x = RIGHT_IMAGE_BORDER;

                if (( mouse_pos_in_image.y > BOTTOM_IMAGE_BORDER ))
                    mouse_pos_in_image.y = BOTTOM_IMAGE_BORDER;

                switch(aDrawnObject.anObjectType)
                {
                    case EMPTY_RECTANGLE:
                    case EMPTY_ELLIPSE:
                    case FILLED_RECTANGLE:
                    case FILLED_ELLIPSE:
                    case SIMPLE_LINE:
                    case SIMPLE_ARROW:

                    if (adding_rect2)
                    {
                        adding_preview2 = true;
                        aDrawnObject.objectPoints.push_back(mouse_pos_in_image);

                        if (!aDrawnObject.objectPoints.empty())
                        {
                            switch(aDrawnObject.anObjectType)
                            {
                                case EMPTY_RECTANGLE:
                                    ImGui::GetOverlayDrawList()->AddRect(pTextCanvas->image_pos + aDrawnObject.objectPoints[0],
                                                                            pTextCanvas->image_pos + aDrawnObject.objectPoints[1],
                                                                            aDrawnObject.objBackgroundColor, 0.0f, ~0 , aDrawnObject.thickness);
                                break;

                                case EMPTY_ELLIPSE:
                                    P1P4 = sqrtf( (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x) + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y) );
                                    ImGui::GetOverlayDrawList()->AddCircle(ImVec2(pTextCanvas->image_pos.x + aDrawnObject.objectPoints[0].x, pTextCanvas->image_pos.y + aDrawnObject.objectPoints[1].y),
                                                                           P1P4,
                                                                           aDrawnObject.objBackgroundColor, 32, aDrawnObject.thickness);
                                break;

                                case FILLED_RECTANGLE:
                                    ImGui::GetOverlayDrawList()->AddRectFilled(pTextCanvas->image_pos + aDrawnObject.objectPoints[0], pTextCanvas->image_pos + aDrawnObject.objectPoints[1], aDrawnObject.objBackgroundColor);
                                break;

                                case FILLED_ELLIPSE:
                                    P1P4 = sqrtf( (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x) + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y) );
                                    ImGui::GetOverlayDrawList()->AddCircleFilled(pTextCanvas->image_pos + aDrawnObject.objectPoints[0], P1P4, aDrawnObject.objBackgroundColor, 32);
                                break;

                                case SIMPLE_LINE:
                                    ImGui::GetOverlayDrawList()->AddLine(pTextCanvas->image_pos + aDrawnObject.objectPoints[0], pTextCanvas->image_pos + aDrawnObject.objectPoints[1], aDrawnObject.objBackgroundColor, aDrawnObject.thickness);
                                break;

                                case SIMPLE_ARROW:
                                    ImGui::GetOverlayDrawList()->AddLine(pTextCanvas->image_pos + aDrawnObject.objectPoints[0], pTextCanvas->image_pos + aDrawnObject.objectPoints[1], aDrawnObject.objBackgroundColor, aDrawnObject.thickness);
                                    P1P4 = sqrtf( (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x)*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x) + (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y)*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y) );

                                    if (P1P4 > 1.5f * arrowLength)
                                    {
                                        ImVec2 pointC(  aDrawnObject.objectPoints[1].x - (arrowLength * (aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/P1P4,
                                                        aDrawnObject.objectPoints[1].y - (arrowLength * (aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/P1P4);
                                        ImVec2 pointD(  pointC.x + (arrowWidth*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/P1P4,
                                                        pointC.y - (arrowWidth*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/P1P4);
                                        ImVec2 pointE(  pointC.x - (arrowWidth*(aDrawnObject.objectPoints[1].y - aDrawnObject.objectPoints[0].y))/P1P4,
                                                        pointC.y + (arrowWidth*(aDrawnObject.objectPoints[1].x - aDrawnObject.objectPoints[0].x))/P1P4);

                                        ImGui::GetOverlayDrawList()->PathClear();
                                        ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(pointD.x + pTextCanvas->image_pos.x, pointD.y + pTextCanvas->image_pos.y));
                                        ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(aDrawnObject.objectPoints[1].x + pTextCanvas->image_pos.x, aDrawnObject.objectPoints[1].y + pTextCanvas->image_pos.y));
                                        ImGui::GetOverlayDrawList()->PathLineTo(ImVec2(pointE.x + pTextCanvas->image_pos.x, pointE.y + pTextCanvas->image_pos.y));
                                        ImGui::GetOverlayDrawList()->PathStroke(aDrawnObject.objBackgroundColor, false, aDrawnObject.thickness);
                                    }
                                break;

                                default:
                                break;
                            }
                        }

                        if (!ImGui::GetIO().MouseDown[0])
                        {
                            adding_rect2    = false;
                            adding_preview2 = false;
                            delayTabDrawnObjects.push_back(aDrawnObject);
                            aDrawnObject.objectPoints.clear();
                        }
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if ( (ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging() )
                            adding_rect2 = adding_preview2 = false;

                        if ( (!adding_rect2 && ImGui::IsMouseClicked(0)) )
                        {
                            aDrawnObject.objectPoints.push_back(mouse_pos_in_image);
                            adding_rect2 = true;
                        }
                    }

                    if ((adding_preview2) && !(aDrawnObject.anObjectType == RANDOM_LINE))
                        aDrawnObject.objectPoints.pop_back();

                    break;

                    case RANDOM_LINE:

                    static bool adding_circle   = false;

                    if (adding_circle)
                    {
                        aDrawnObject.anObjectType = selectedObject;
                        aDrawnObject.objectPoints.push_back(mouse_pos_in_image);

                        for (int i = 0 ; i < aDrawnObject.objectPoints.size(); i++)
                        {
                            ImGui::GetOverlayDrawList()->AddCircleFilled(pTextCanvas->image_pos + aDrawnObject.objectPoints[i], aDrawnObject.thickness, aDrawnObject.objBackgroundColor, 8);
                        }

                        if (!ImGui::GetIO().MouseDown[0])
                        {
                            adding_circle = false;
                            delayTabDrawnObjects.push_back(aDrawnObject);
                            aDrawnObject.objectPoints.clear();
                        }
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if ((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging())
                            adding_circle = false;

                        if ((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && ImGui::IsMouseDragging())
                            adding_circle = true;

                        if ( (!adding_circle && ImGui::IsMouseClicked(0)) )
                        {
                            aDrawnObject.objectPoints.push_back(mouse_pos_in_image);
                            adding_circle = true;
                        }
                    }
                    break;

                    case RANDOM_ARROW:
                    static bool adding_circle2   = false;

                    if (adding_circle2)
                    {
                        aDrawnObject.anObjectType = selectedObject;
                        arrow_points.push_back(mouse_pos_in_image);

                        for (int i = 0 ; i < arrow_points.size(); i++)
                        {
                            ImGui::GetOverlayDrawList()->AddCircleFilled(pTextCanvas->image_pos + arrow_points[i], aDrawnObject.thickness, aDrawnObject.objBackgroundColor, 8);
                        }

                        if (!ImGui::GetIO().MouseDown[0])
                        {
                            adding_circle2 = false;
                            aDrawnObject.objectPoints.push_back(arrow_points[0]);
                            aDrawnObject.objectPoints.push_back(arrow_points  [(int)(arrow_points.size()/3.0f)]);
                            aDrawnObject.objectPoints.push_back(arrow_points[(int)((2*arrow_points.size())/3.0f)]);
                            aDrawnObject.objectPoints.push_back(arrow_points[arrow_points.size()-1]);
                            delayTabDrawnObjects.push_back(aDrawnObject);
                            arrow_points.clear();
                            aDrawnObject.objectPoints.clear();
                        }
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if ( (ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && !ImGui::IsMouseDragging() )
                        {
                            adding_circle2 = false;
                        }

                        if ((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1)) && ImGui::IsMouseDragging())
                            adding_circle2 = true;

                        if ( (!adding_circle2 && ImGui::IsMouseClicked(0)) )
                        {
                            arrow_points.push_back(mouse_pos_in_image);
                            adding_circle2 = true;
                        }
                    }
                    break;

                    case NOT_A_DRAWN_OBJECT:

                    if (adding_rect)
                    {
                        adding_preview1 = true;
                        points.push_back(mouse_pos_in_image);

                        if (!ImGui::GetIO().MouseDown[0])
                            adding_rect = adding_preview1 = false;
                    }

                    if (ImGui::IsItemHovered())
                    {
                        if ( (((ImGui::IsMouseClicked(0)||ImGui::IsMouseClicked(1) )  && (!points.empty()))) && !ImGui::IsMouseDragging() )
                        {
                            adding_rect = false;
                            adding_preview1 = false;
                            points.pop_back();
                            points.pop_back();
                        }
                        if ( (!adding_rect && ImGui::IsMouseClicked(0)) )
                        {
                            points.push_back(mouse_pos_in_image);
                            adding_rect = true;
                        }
                    }
                    // clip lines and objects within the canvas (if we resize it, etc.)
                    draw_list->PushClipRect(ImVec2(0.0f, 0.0f), pTextCanvas->image_pos + subview_size);

                    for (int i = 0; i < points.Size - 1; i += 2)
                    {
                        // be sure the area is enough big to be drawn
                        if ((abs(points[i].x-points[i+1].x) > 2) && (abs(points[i].y-points[i+1].y)> 2))
                        {
                            draw_list->AddRect(ImVec2(pTextCanvas->image_pos.x + points[i].x, pTextCanvas->image_pos.y + points[i].y),
                                               ImVec2(pTextCanvas->image_pos.x + points[i+1].x, pTextCanvas->image_pos.y + points[i+1].y), color, 0.0f, ~0 ,outline_thickness);
                            topLeft     = points[i];
                            bottomRight = points[i+1];
                        }
                        else
                        {
                            // in this case, draw nothing in the lense
                            topLeft     = ImVec2(0.0f,0.0f);
                            bottomRight = ImVec2(ZOOM_WIDTH_MIN, ZOOM_HEIGHT_MIN);
                        }
                    }
                    draw_list->PopClipRect();

                    reorder_points(p_topLeft, p_bottomRight);

                    if (adding_preview1)
                        points.pop_back();

                    break;

                    case TEXT_OBJECT:
                    default:
                    break;
                }

                ///////////////////////////////////////////////////////////////////////////////////////////////
                //                                           DRAW ALL
                ///////////////////////////////////////////////////////////////////////////////////////////////

                // clip lines and objects within the canvas (if we resize it, etc.)
                draw_list->PushClipRect(ImVec2(0.0f, 0.0f), pTextCanvas->image_pos + subview_size);

                for (unsigned int i = 0; i < delayTabDrawnObjects.size(); i++)
                {
                    {
                        switch(delayTabDrawnObjects[i].anObjectType)
                        {
                            case EMPTY_RECTANGLE:
                            // already stored
                            draw_list->AddRect( pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0],
                                                pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[1],
                                                delayTabDrawnObjects[i].objBackgroundColor, 0.0f, ~0 , delayTabDrawnObjects[i].thickness);
                            break;

                            case EMPTY_ELLIPSE:

                            P1P4 = sqrtf( ( delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x)*(delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x) + (delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y)*(delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y) );
                                            draw_list->AddCircle(ImVec2(pTextCanvas->image_pos.x + delayTabDrawnObjects[i].objectPoints[0].x, pTextCanvas->image_pos.y + delayTabDrawnObjects[i].objectPoints[1].y),
                                            P1P4,
                                            delayTabDrawnObjects[i].objBackgroundColor, 32, delayTabDrawnObjects[i].thickness);
                            break;

                            case FILLED_RECTANGLE:

                            draw_list->AddRectFilled(pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0], pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[1], delayTabDrawnObjects[i].objBackgroundColor);
                            break;

                            case FILLED_ELLIPSE:

                            P1P4 = sqrtf( (delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x)*(delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x) + (delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y)*(delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y) );
                            draw_list->AddCircleFilled(pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0], P1P4, delayTabDrawnObjects[i].objBackgroundColor, 32);
                            break;

                            case SIMPLE_LINE:

                            draw_list->AddLine(pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0], pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[1], delayTabDrawnObjects[i].objBackgroundColor, delayTabDrawnObjects[i].thickness);
                            break;

                            case SIMPLE_ARROW:

                            draw_list->AddLine(pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0], pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[1], delayTabDrawnObjects[i].objBackgroundColor, delayTabDrawnObjects[i].thickness);
                            P1P4 = sqrtf( (delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x)*(delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x) + (delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y)*(delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y) );

                            if (P1P4 > 1.5f * arrowLength)
                            {
                                ImVec2 pointC(  delayTabDrawnObjects[i].objectPoints[1].x - (arrowLength * (delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x))/P1P4,
                                                delayTabDrawnObjects[i].objectPoints[1].y - (arrowLength * (delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y))/P1P4);
                                ImVec2 pointD(  pointC.x + (arrowWidth*(delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y))/P1P4,
                                                pointC.y - (arrowWidth*(delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x))/P1P4);
                                ImVec2 pointE(  pointC.x - (arrowWidth*(delayTabDrawnObjects[i].objectPoints[1].y - delayTabDrawnObjects[i].objectPoints[0].y))/P1P4,
                                                pointC.y + (arrowWidth*(delayTabDrawnObjects[i].objectPoints[1].x - delayTabDrawnObjects[i].objectPoints[0].x))/P1P4);

                                draw_list->PathClear();
                                draw_list->PathLineTo(ImVec2(pointD.x + pTextCanvas->image_pos.x, pointD.y + pTextCanvas->image_pos.y));
                                draw_list->PathLineTo(ImVec2(delayTabDrawnObjects[i].objectPoints[1].x + pTextCanvas->image_pos.x, delayTabDrawnObjects[i].objectPoints[1].y + pTextCanvas->image_pos.y));
                                draw_list->PathLineTo(ImVec2(pointE.x + pTextCanvas->image_pos.x, pointE.y + pTextCanvas->image_pos.y));
                                draw_list->PathStroke(delayTabDrawnObjects[i].objBackgroundColor, false, delayTabDrawnObjects[i].thickness);
                            }
                            break;

                            case RANDOM_ARROW:

                            for (int j = 2 ; j < delayTabDrawnObjects[i].objectPoints.size() ; j++)
                            {
                                    //draw a bezier curve
                                    draw_list->AddBezierCurve(   pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[0],  //P1 == start
                                                                 pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[1], // P2 == CP1
                                                                 pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[2], // P3 == CP2
                                                                 pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[3], // P4 == end
                                                                 delayTabDrawnObjects[i].objBackgroundColor, delayTabDrawnObjects[i].thickness, 64); // drawing parameters
                            }

                            P1P4 = sqrtf((delayTabDrawnObjects[i].objectPoints[3].x - delayTabDrawnObjects[i].objectPoints[0].x)*(delayTabDrawnObjects[i].objectPoints[3].x - delayTabDrawnObjects[i].objectPoints[0].x) + (delayTabDrawnObjects[i].objectPoints[3].y - delayTabDrawnObjects[i].objectPoints[0].y)*(delayTabDrawnObjects[i].objectPoints[3].y - delayTabDrawnObjects[i].objectPoints[0].y) );

                            if (P1P4 > 1.5f * arrowLength)
                            {
                                ImVec2 pointC(  delayTabDrawnObjects[i].objectPoints[3].x - (arrowLength * (delayTabDrawnObjects[i].objectPoints[3].x - delayTabDrawnObjects[i].objectPoints[2].x))/P1P4,
                                                delayTabDrawnObjects[i].objectPoints[3].y - (arrowLength * (delayTabDrawnObjects[i].objectPoints[3].y - delayTabDrawnObjects[i].objectPoints[2].y))/P1P4);
                                ImVec2 pointD(  pointC.x + (arrowWidth*(delayTabDrawnObjects[i].objectPoints[3].y - delayTabDrawnObjects[i].objectPoints[2].y))/P1P4,
                                                pointC.y - (arrowWidth*(delayTabDrawnObjects[i].objectPoints[3].x - delayTabDrawnObjects[i].objectPoints[2].x))/P1P4);
                                ImVec2 pointE(  pointC.x - (arrowWidth*(delayTabDrawnObjects[i].objectPoints[3].y - delayTabDrawnObjects[i].objectPoints[2].y))/P1P4,
                                                pointC.y + (arrowWidth*(delayTabDrawnObjects[i].objectPoints[3].x - delayTabDrawnObjects[i].objectPoints[2].x))/P1P4);

                                draw_list->PathClear();
                                draw_list->PathLineTo(ImVec2(pointD.x + pTextCanvas->image_pos.x, pointD.y + pTextCanvas->image_pos.y));
                                draw_list->PathLineTo(ImVec2(delayTabDrawnObjects[i].objectPoints[3].x + pTextCanvas->image_pos.x, delayTabDrawnObjects[i].objectPoints[3].y + pTextCanvas->image_pos.y));
                                draw_list->PathLineTo(ImVec2(pointE.x + pTextCanvas->image_pos.x, pointE.y + pTextCanvas->image_pos.y));
                                draw_list->PathStroke(delayTabDrawnObjects[i].objBackgroundColor, false, delayTabDrawnObjects[i].thickness);

                            }
                            break;

                            case RANDOM_LINE :
                            for (unsigned int i = 0 ; i < delayTabDrawnObjects.size(); i++)
                            {
                                for (int j = 0 ; j < delayTabDrawnObjects[i].objectPoints.size() ; j++)
                                {
                                    if (delayTabDrawnObjects[i].anObjectType == RANDOM_LINE)
                                         draw_list->AddCircleFilled(pTextCanvas->image_pos + delayTabDrawnObjects[i].objectPoints[j], delayTabDrawnObjects[i].thickness, delayTabDrawnObjects[i].objBackgroundColor, 8);
                                }

                            }
                            break;

                            default:
                            break;
                        }
                    }
                }
                draw_list->PopClipRect();

// END CANVAS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

// MAGNIFIER
                // MAGNIFIER todo : always available

                // zoom value
                static     float aZoom = 1.0f;
                /*static*/ float W = bottomRight.x - topLeft.x;
                /*static*/ float H = bottomRight.y - topLeft.y;

                // minimal dimensions for the zoom (else, we can't close it)
                if (W < 260.0f)
                    W = ZOOM_WIDTH_MIN;

                if (H < 260.0f)
                    H = ZOOM_HEIGHT_MIN;

                ImVec2 center((bottomRight.x + topLeft.x)/2.0f + LEFT_IMAGE_BORDER, (bottomRight.y + topLeft.y)/2.0f);
                ImVec2 delta( W/(2.0f*aZoom), H/(2.0f*aZoom));
                ImVec2 uv0 = ImVec2((center.x - delta.x)/w, (center.y - delta.y)/(w*ratio));
                ImVec2 uv1 = ImVec2((center.x + delta.x)/w, (center.y + delta.y)/(w*ratio));

                if ((b_zoom_available) && (!b_paused))
                {
                    current_delayTab_drawing_task = DRAWING_ZOOMED_AREA;
                    selectedObject = NOT_A_DRAWN_OBJECT;
                    ImGui::SetNextWindowFocus();
                    ImGuiWindowFlags zoomWinFlags = 0;
                    zoomWinFlags |= ImGuiWindowFlags_NoSavedSettings;

                    if (adding_rect)
                        ImGui::SetNextWindowSize(ImVec2(W + 18.0f, H + 70.0f));

                    ImGui::Begin( ICON_FA_SEARCH ZOOM_WINDOW);
                    {
                        // resizable window
                        ImGui::Image(reinterpret_cast<void * >(backgroundTextureId),
                                     ImVec2(ImGui::GetWindowWidth() - 18.0f, ImGui::GetWindowHeight() - 70.0f),
                                     uv0,uv1, ImColor(255,255,255,255), ImColor(255,255,255,255));

                        ImGui::Text(ZOOM_VALUE); ImGui::SameLine();
                        ImGui::PushItemWidth(125);
                        ImGui::SliderFloat("##ghost", &aZoom, 1.0f, 4.0f, "##%.1f");
                        ImGui::PopItemWidth(); ImGui::SameLine();

                        if (ImGui::Button(CLOSE_BUTTON))
                        {
                            // in this case, draw nothing in the lense
                            ImGui::SetWindowSize(ImVec2(ZOOM_WIDTH_MIN, ZOOM_HEIGHT_MIN));
                            b_zoom_available = false;
                            p_aDrawnObject->anObjectType = NOT_A_DRAWN_OBJECT;
                        }
                    }
                    ImGui::End();
// END MAGNIFIER

                }
                ImGui::SameLine();
                ImGui::Spacing();

// DRAW THE CHRONO 
                if (b_chrono)
                {
                    static float activityTime = 0.0f;
                    static float totalTime = 0.0f;
                    static unsigned int activityTimeMinutes = 0;
                    static unsigned int activityTimeSeconds = 0;
                    static unsigned int activityTimeTenth   = 0;
                    static unsigned int totalTimeMinutes = 0;
                    static unsigned int totalTimeSeconds = 0;
                    static unsigned int totalTimeTenth = 0;

                    if (ImGui::Button(CHRONO_RESET_BUTTON))
                        aTimer.reset();

                    ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();

                    if (ImGui::Button( aTimer.getTimerPaused() ? " " ICON_FA_PLAY " " : " " ICON_FA_PAUSE " "))
                        aTimer.start();

                    ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();

                    if (b_display_activity_time)
                    {
                        activityTime = aTimer.getActivityTime()/1000.0f;
                        //activityTime = aTimer.getActivityTime()/aTimer.frequency;
                        activityTimeMinutes = (unsigned int)activityTime / 60;
                        activityTimeSeconds = (unsigned int)activityTime % 60;
                        activityTimeTenth = (unsigned int)(100 * (activityTime - floor(activityTime)));

                        ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();

                        if (activityTimeMinutes < 1)
                            ImGui::Text(CHRONO_ACTIVITY_TIME, activityTimeSeconds, activityTimeTenth);
                        else
                            ImGui::Text(CHRONO_ACTIVITY_TIME, activityTimeMinutes, activityTimeSeconds);

                        ImGui::SameLine();
                    }

                    if (b_display_total_time)
                    {
                        totalTime = aTimer.getTimeSinceTimerStarted()/1000.0f;
                        //totalTime = aTimer.getTimeSinceTimerStarted()/aTimer.frequency;
                        totalTimeMinutes = (unsigned int)totalTime / 60;
                        totalTimeSeconds = (unsigned int)totalTime % 60;
                        totalTimeTenth = (unsigned int)(100 * (totalTime - floor(totalTime)));
                        ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();

                        if (totalTimeMinutes < 1)
                            ImGui::Text(CHRONO_TOTAL_ELLAPSED_TIME, totalTimeSeconds, totalTimeTenth);
                        else
                            ImGui::Text(CHRONO_TOTAL_ELLAPSED_TIME, totalTimeMinutes, totalTimeSeconds);

                        ImGui::SameLine();
                    }
                }
                else
                    ImGui::Dummy(ImVec2(2.0f, 2.0f));
// END DRAW THE CHRONO 

// DRAW THE PLAYER

                if (b_video_running)
                {
                    static bool test_video_crappy_done = false;
#ifdef DEBUG
                    fprintf(stdout, "position = %.2f\n", position);
#endif
                    if (ImGui::Button("  " ICON_FA_FAST_BACKWARD "  "))
                    {
                        incr =  -10.0f;
                        do_seek(incr, -1);
                    }
                    ImGui::SameLine();

                    if (ImGui::Button("  " ICON_FA_STEP_BACKWARD "  "))
                    {
                        incr =  -2.0f;
                        do_seek(incr, -1);
                    }
                    ImGui::SameLine();

                    if (ImGui::Button("  " ICON_FA_STOP "  "))
                    {
                        b_audio_paused = true;
                        SDL_PauseAudioDevice(audio_dev, b_audio_paused);
                        incr = 0.0f;
                        position = 0.0f;
                        //position = origin;
                        std::cout << "incr   : "   << incr     << std::endl;
                        std::cout << "position : " << position << std::endl;
                        std::cout << "origin : "   << origin   << std::endl;
                        do_seek(incr, (long int)origin);

//                        position = origin;
                    }
                    ImGui::SameLine();

                    if (b_audio_paused)
                    {
                        if (ImGui::Button("  " ICON_FA_PLAY "  "))
                        {
                            b_audio_paused = false;
                            SDL_PauseAudioDevice(audio_dev, b_audio_paused);

                            // mandatory, to avoid race condition with seek
//8 mai                       SDL_Delay(50);
                            // needed, to avoid the slider graber jumping
#ifdef MASTER_CLOCK
                            position = get_master_clock(is) - origin;
#else
                            position = get_audio_clock(is) - origin;
#endif
                        }
                    }
                    else if (ImGui::Button("  " ICON_FA_PAUSE "  "))
                    {
                        b_audio_paused = true;
                        SDL_PauseAudioDevice(audio_dev, b_audio_paused);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("  " ICON_FA_STEP_FORWARD "  "))
                    {
                        incr =  2.0f;
                        do_seek(incr, -1);
                    }
                    ImGui::SameLine();

                    if (ImGui::Button("  " ICON_FA_FAST_FORWARD "  "))
                    {
//8 mai                        if (video_duration - (position - origin) > 10.2f)
                        if (video_duration - (position - origin) > 10.2f)
                        {
                            incr =  10.0f;
                            do_seek(incr, -1);
                        }
                        else
                        {
//8 mai                            incr =  video_duration - (position - origin) - 0.2f;
                            incr =  video_duration - (position - origin) -0.2f;
                            do_seek(incr, -1);
                        }
                    }
                    ImGui::SameLine();

                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("      ");
                    ImGui::SameLine();

                    // TIME POSITON IN THE VIDEO WIDGET
                    ImGui::PushItemWidth(300);

                    if ((is != 0) && (!b_audio_paused))
                    {
                        if ((position > video_duration) && (!test_video_crappy_done))
                        {
                            position = initialize_position();
                            test_video_crappy_done = true;
                        }
#ifdef MASTER_CLOCK
                        position = get_master_clock(is) - origin;
#else
                        position = get_audio_clock(is) - origin;
#endif
#ifdef DEBUG2
                        static short int counter = 0;

                        if ((counter % 30) == 0 )
                        {
                            std::cout << "origin              = " << origin << std::endl;
                            std::cout << "mp4_origin          = " << mp4_origin << std::endl;
                            std::cout << "position            = " << position << std::endl;
                            std::cout << "get_audio_clock(is) = " << get_audio_clock(is) << std::endl;
                            counter = 0;
                        }
                        counter++;
#endif
                    }

                    static float sliderThicknessScale = 0.60f;
                    ImGui::PushStyleVar(ImGuiStyleVar_SliderThicknessScale, sliderThicknessScale);
                    ImGui::SliderFloat(" ", &position, 0.0f, video_duration, "%.2f");
                    static bool b_changing_value = false;

                    // logical
                    if ((ImGui::IsItemActive())||(ImGui::IsItemEdited())||ImGui::IsMouseDragging()||ImGui::IsItemClicked())
                    {
#ifdef MASTER_CLOCK
                        if (fabs(position - get_master_clock(is)) >= 0.2f)
                            b_changing_value = true;
#else
                        if (fabs(position - get_audio_clock(is)) >= 0.2f)
                            b_changing_value = true;
#endif
                    }
                    // update
                    if(b_changing_value)
                    {
                        b_audio_paused = true;
                        SDL_PauseAudioDevice(audio_dev, b_audio_paused);
                    }

                    if (!ImGui::IsItemActive() && !ImGui::IsItemEdited() && !ImGui::IsItemClicked() && b_changing_value && !ImGui::IsMouseDragging())
                    {
                        {
                            std::cout << "origin              = " << origin << std::endl;
                            std::cout << "position            = " << position << std::endl;
                            std::cout << "get_audio_clock(is) = " << get_audio_clock(is) << std::endl;
#ifdef MASTER_CLOCK
                            incr =  position - get_master_clock(is) - 0.2f;
#else
                            incr =  position - get_audio_clock(is) - 0.2f;
#endif
                            do_seek(incr, -1);
//                            SDL_Delay(50);

                            std::cout << "origin              = " << origin << std::endl;
                            std::cout << "position            = " << position << std::endl;
                            std::cout << "get_audio_clock(is) = " << get_audio_clock(is) << std::endl;

//                            b_audio_paused = false;
//                            SDL_PauseAudioDevice(audio_dev, b_audio_paused);
                            b_changing_value = false;
                        }
                    }
                    ImGui::PopStyleVar();

                    if ((is != nullptr) && (position != origin) && (!b_audio_paused) && ((position - origin) >= (video_duration - 0.1f)))
                    {
                        b_audio_paused = true;
                        SDL_PauseAudioDevice(audio_dev, b_audio_paused);
                    }

                    ImGui::PopItemWidth(); // Time position in currently playing video
// END DRAW THE PLAYER

                    ImGui::SameLine();

                    // END TIME POSITION

                }

                ImGui::Text(RENDERING_TIME_FPS_DISPLAY, 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::EndChild();
                ImGui::EndGroup(); // FIN1
            }
// END CHILD1 : NOW DRAW OVER THE FRAME

            ImGui::SameLine(); ImGui::Dummy (ImVec2(40.0f, 0.0f)); ImGui::SameLine();

            ImGui::BeginChild("child2", ImVec2(WEBCAM_PARAMETERS_AREA_WIDTH, h), true);

            /*  Chronomètre, choix de la webcam etc */
            ImGui::BeginGroup();
            {
                static float indentation = -4.0f;
                ImGui::Indent(indentation);

                if ((image_format >= IMAGE_FORMAT_640x360) && (!b_paused))
                    ImGui::Checkbox("  " ICON_FA_FILM RECORD_VIDEO_CHECKBOX, &b_recording_checked);
                else
                    ImGui::NewLine();

                ImGui::NewLine();
                if ((ImGui::Button("  " ICON_FA_FOLDER_OPEN_O CURRENT_WORKING_DIRECTORY_BUTTON)) && (!b_paused))
                {
                    selectFolder(currentPath);
                    std::cout << "currentPath : " << currentPath << std::endl;
                }

                if (b_paused)
                    b_zoom_available = false;
                else
                {
                    if ((b_recording_checked) && (!b_recording_video))
                    {
                        if (strlen(defaultPath) == 0)
                        {
                            // open modal dialog : il faut définir un répertoire de travail
                            // dans lequel seront enregistrées les images
                            ImGui::OpenPopup(CURRENT_WORKING_DIRECTORY_POPUP);
                            if( ImGui::BeginPopupModal(CURRENT_WORKING_DIRECTORY_POPUP, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text(HELP_CHOOSE_WORKING_REC_DIRECTORY);
                                ImGui::NewLine();

                                ImGui::Spacing(); ImGui::SameLine();
                                if (ImGui::Button(OK_BUTTON, ImVec2(120,0)))
                                {
                                    b_recording_checked = false;
                                    selectFolder(currentPath);
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                        }
                        else
                        {
                            b_recording_video = true;
                            b_draw_recording_info = true;
                            b_startNewRecording = true;
                            std::cout << "Enregistrement en cours" << std::endl;
                            inc_color();
                        }
                    }
                    else if ((!b_recording_checked) && (b_recording_video))
                    {
                        b_recording_video = false;
                        b_draw_recording_info = false;
                        oVideoWriter.release();
                        std::cout << "Fin d'enregistrement" << std::endl;
                    }
                    else if ((!b_recording_checked) && (!b_recording_video))
                    {
                        if (ImGui::Button(ADVANCED_RECORDING_PARAMETERS_BUTTON))
                            ImGui::OpenPopup(CODEC_FPS_SIZE_POPUP);

                        if (ImGui::BeginPopupModal(CODEC_FPS_SIZE_POPUP, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            static int container = 1; // default is ".avi"
                            /* Choix du container */
                            ImGui::Text(VIDEO_CONTAINER);
                            ImGui::RadioButton( " " MP4_CONTAINER_RADIOBUTTON, &container, 0); ImGui::SameLine();
                            ImGui::RadioButton( " " AVI_CONTAINER_RADIOBUTTON, &container, 1); ImGui::SameLine();
                            ImGui::RadioButton( " " MKV_CONTAINER_RADIOBUTTON, &container, 2);

                            switch(container)
                            {
                                case 0 : myContainer = MP4_CONTAINER_RADIOBUTTON ; break;
                                case 1 : myContainer = AVI_CONTAINER_RADIOBUTTON ; break;
                                case 2 : myContainer = MKV_CONTAINER_RADIOBUTTON ; break;

                                default:
                                break;
                            }

                            ImGui::NewLine();

                            /* Choix du codec */
                            static int video_format = 1; // default == DIV3, gives good results on Linux (no idea on Windows)
                            // prefered seems to be DIV3 on Linux, MJPG on Windows ; FMP4 on MacOS X
                            ImGui::Text(VIDEO_CODEC_TEXT);
                            ImGui::RadioButton( " " DIVX_CODEC_RADIOBUTTON, &video_format, 0); ImGui::SameLine();
                            ImGui::RadioButton( " " DIV3_CODEC_RADIOBUTTON, &video_format, 1); ImGui::SameLine();
                            ImGui::RadioButton( " " MJPG_CODEC_RADIOBUTTON, &video_format, 2); ImGui::SameLine();
                            ImGui::RadioButton( " " FMP4_CODEC_RADIOBUTTON, &video_format, 3);
                            ImGui::RadioButton( " " FLV1_CODEC_RADIOBUTTON, &video_format, 4); ImGui::SameLine();
                            ImGui::RadioButton( " " DX50_CODEC_RADIOBUTTON, &video_format, 5); ImGui::SameLine();
                            ImGui::RadioButton( " " U263_CODEC_RADIOBUTTON, &video_format, 6); ImGui::SameLine();
                            ImGui::RadioButton( " " H264_CODEC_RADIOBUTTON, &video_format, 7);
                            // produces instant crash on Linux
                            // ImGui::RadioButton( " X262", &video_format, 4);

                            switch(video_format)
                            {
                                case 0 : myFourcc = CV_FOURCC('D', 'I', 'V', 'X'); break;
                                case 1 : myFourcc = CV_FOURCC('D', 'I', 'V', '3'); break;
                                case 2 : myFourcc = CV_FOURCC('M', 'J', 'P', 'G'); break;
                                case 3 : myFourcc = CV_FOURCC('F', 'M', 'P', '4'); break;
                                case 4 : myFourcc = CV_FOURCC('D', 'X', '5', '0'); break; // Linux OK
                                case 5 : myFourcc = CV_FOURCC('F', 'L', 'V', '1'); break;
                                case 6 : myFourcc = CV_FOURCC('U', '2', '6', '3'); break;
                                case 7 : myFourcc = CV_FOURCC('H', '2', '6', '4'); break;
                                //case 8 : myFourcc = CV_FOURCC('X', '2', '6', '2'); break;

                                default:
                                break;
                            }
#ifdef FPS_FEATURE_ENABLED
                            /* Choix des fps */
                            static int out_fps_choice = 2;
                            // prefered seems to be DIV3 on Linux, MJPG on Windows ; FMP4 on MacOS X
                            ImGui::Text(FRAMES_PER_SECOND_CHOICE);
                            ImGui::RadioButton( " 15.0", &out_fps_choice, 0); ImGui::SameLine();
                            ImGui::RadioButton( " 20.0", &out_fps_choice, 1); ImGui::SameLine();
                            ImGui::RadioButton( " 24.0", &out_fps_choice, 2); ImGui::SameLine();
                            ImGui::RadioButton( " 25.0", &out_fps_choice, 3);
                            ImGui::RadioButton( " 60.0", &out_fps_choice, 4);

                            switch(video_format)
                            {
                                case 0 : outFPS = 15.0; break;
                                case 1 : outFPS = 20.0; break;
                                case 2 : outFPS = 24.0; break;
                                case 3 : outFPS = 25.0; break;
                                case 4 : outFPS = 60.0; break;

                                default:
                                break;
                            }

                            /* Choix dimensions de l'image */
#ifdef HIGH_DPI_SCREEN
                            const char * out_images_sizes[] = { " 640 x 480 ", " 800 x 600", " 960 x 540", " 1280 x 720 ", "1360 x 768", "1440 x 900 ", "1920 x 1080 " };
                            const int out_width_sizes[] =  { 640, 800, 960, 1280, 1360, 1440, 1920 };
                            const int out_height_sizes[] = { 480, 600, 540, 720, 768, 900, 1080 };
#else
                            const char * out_images_sizes[] = { " 640 x 360", " 800 x 600", " 960 x 540", " 1280 x 720 ", "1360 x 768" };
                            const int out_width_sizes[] =  { 640, 800, 960, 1280 1360 };
                            const int out_height_sizes[] = { 360, 600, 540, 720, 768};
#endif
                            if (ImGui::Button(RECORDED_FRAMESIZE_BUTTON))
                                ImGui::OpenPopup(RECORDED_FRAMESIZE_POPUP);

                            ImGui::SameLine();
                            ImGui::TextUnformatted(out_images_sizes[out_image_format]);

                            if (ImGui::BeginPopup(RECORDED_FRAMESIZE_POPUP))
                            {
                                for (int i = 0 ; i < IM_ARRAYSIZE(out_images_sizes); i++)
                                {
                                   if (ImGui::Selectable(out_images_sizes[i]))
                                   {
                                       outWidth  = out_width_sizes[i];
                                       outHeight = out_height_sizes[i];
                                    out_image_format = i;
                                    }
                                }
                                ImGui::EndPopup();
                            }

                            out_new_framesize = outWidth * outHeight;

                            if ((out_old_framesize != out_new_framesize))
                            {
                                outFrameSize.width = outWidth;
                                outFrameSize.height = outHeight;
                                out_old_framesize = out_new_framesize;
                            }

                            ImGui::SameLine();
#endif
                            ImGui::NewLine();
                            ImGui::NewLine();

                            if (ImGui::Button(RESTORE_DEFAULT_VALUES_BUTTON))
                            {
                                container = 1; //myContainer = ".avi";
                                video_format = 1; //myFourcc = CV_FOURCC('D', 'I', 'V', '3');
                            }

                            ImGui::SameLine();

                            if (ImGui::Button(CLOSE_BUTTON_FRAMESIZE_POPUP, ImVec2(120,0)))
                                ImGui::CloseCurrentPopup();

                            ImGui::EndPopup();
                        } // BeginPopupModal()
                    }
                }

                if ( ((!b_full_video_selected)&&(b_use_USB_webcam)) || (b_cell_phone_selected))
                {
                    ImGui::Checkbox(DELAYED_DISPLAY_CHECKBOX, &b_display_delayed);
                    delay = (int)fdelay;

                    if (b_display_delayed)
                    {
                        if (current_source != old_source)
                        {
                            while (!cb.empty())
                            {
                                cb.pop();
                            }
                            b_Once = false;
                            old_source = current_source;
                        }

                        ImGui::Text(DELAY_IN_SECONDS); ImGui::SameLine();
                        ImGui::PushID("delay");
                        ImGui::SliderInt("",&fdelay,1, DELAY_MAX);
                        ImGui::PopID();
                    }
                    else
                    {
                        ImGui::Text(" ");
                        ImGui::Spacing();
                    }
                }

                ImGui::Checkbox( "  " ICON_FA_CLOCK_O CHRONOMETER_CHECKBOX, &b_chrono);

                if (b_chrono)
                {
                    ImGui::Checkbox(CHRONOMETER_RECORD_CHECKBOX, &b_record_chrono);
                    ImGui::Checkbox(CHRONOMETER_ACTIVITY_TIME_CHECKBOX, &b_display_activity_time);
                    ImGui::Checkbox(CHRONOMETER_TOTAL_TIME_CHECKBOX, &b_display_total_time);
                }
                else
                {
                    ImGui::Text(" ");
                    ImGui::Text(" ");
                    ImGui::Text(" ");
                }

                if (!b_chrono)
                {
                    ImGui::NewLine();
                }
#ifdef FPS_TEST
                    ImGui::PushID("fps_test ");
                    ImGui::PushItemWidth(80.0f);
                    ImGui::SliderInt("fps_test ", &fps_test, 5, 100, "%d");
                    ImGui::PopItemWidth();
                    ImGui::PopID();
#endif
                static bool b_display_recordable_audio_list = false;

                ImGui::Checkbox(SEE_RECORDABLE_AUDIO_DEVICES, &b_display_recordable_audio_list);


                if(b_display_recordable_audio_list)
                {
                    static short int count = md::getSDLRecordableAudioDeviceNames();
                    static int selected = -1;

                    // FIXME : be sure everything is connected, to avoid some missing device
                    for (int n = 0; n < count ; n++)
                    {
                        char buf[1024];
                        sprintf(buf, "%s %d : %s", ICON_FA_MICROPHONE, n, (recordableAudioDeviceNames.at(n)).c_str());

                        if (ImGui::Selectable(buf, selected == n))
                        {
                            //md::selectSDLRecordableAudioDeviceName(n);
                            selected = n;
                        }
                    }

                    ImGui::NewLine();
                }

//                ImGui::NewLine();

                ImGui::Text(SOURCES_OF_IMAGES); ImGui::SameLine();

                switch(current_source)
                {
                    case USB_WEBCAM: ImGui::Text(" " ICON_FA_VIDEO_CAMERA "  " INPUT_SOURCENAME_USB_WEBCAM );
                      break;

                    case FULL_VIDEO: ImGui::Text(" " ICON_FA_FILM "  " INPUT_SOURCENAME_VIDEO );
                      break;

                    case NETWORK:
                    case IP_WEBCAM : ImGui::Text(" " ICON_FA_WIFI " " INPUT_SOURCENAME_IP_WEBCAM);
                      break;

                    case CELL_PHONE: ImGui::Text( ICON_FA_PHONE_SQUARE INPUT_SOURCENAME_CELL_PHONE );
                      break;

                    default:
                      break;
                }

                ImGui::NewLine();

                /* Choix de la source pour le flux vidéo */
                if ((!b_video_running)&&(!b_recording_video))
                {
                    // POPUP MODAL
                    if (ImGui::Button(USE_ANOTHER_IMAGE_SOURCE_BUTTON))
                        ImGui::OpenPopup(SELECT_ANOTHER_IMAGES_SOURCE);
                }

                if (ImGui::BeginPopupModal(SELECT_ANOTHER_IMAGES_SOURCE, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    // initialization
                    static int selected_image_source = current_source;

                    ImGui::RadioButton("  " ICON_FA_VIDEO_CAMERA "  " INPUT_SOURCENAME_USB_WEBCAM, &selected_image_source, 0);
                    ImGui::RadioButton("  " ICON_FA_FILM         "  " INPUT_SOURCENAME_VIDEO     , &selected_image_source, 1);
                    ImGui::RadioButton("  " ICON_FA_WIFI         " "  INPUT_SOURCENAME_IP_WEBCAM , &selected_image_source, 2);
                    ImGui::RadioButton("  " ICON_FA_PHONE_SQUARE "  " INPUT_SOURCENAME_CELL_PHONE, &selected_image_source, 3);

                    switch(selected_image_source)
                    {
                        case 0: selected_image_source = USB_WEBCAM;
                          break;

                        case 1: selected_image_source = FULL_VIDEO; b_use_USB_webcam = false;
                          break;

                        case 2: selected_image_source = IP_WEBCAM; b_use_USB_webcam = false;
                          break;

                        case 3: selected_image_source = CELL_PHONE; b_use_USB_webcam = false;
                          break;

                        default: selected_image_source = current_source;
                          break;
                    }

                    if (current_source != selected_image_source)
                        current_source = selected_image_source;

                    ImGui::NewLine();
                    ImGui::SetItemDefaultFocus();

                    if (ImGui::Button(OK_BUTTON, ImVec2(250,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                } // BeginPopupModal()

                /* Choix de la webcam si source == webcam */
                const char * webcam_names[] = { WEBCAM1, WEBCAM2, WEBCAM3 };

                /* Choix taille de l'image en pixels */
                static int aWidth = 1280;
                static int aHeight = 720;
                // CaptureDev parameters (input)
                static int capture_dev = DEFAULT_DEVICE_NUMBER;
                static int old_capture_dev = DEFAULT_DEVICE_NUMBER;

                if ((current_source == USB_WEBCAM)&&(!b_recording_video))
                    ImGui::Checkbox(ACTIVATE_THE_USB_WEBCAM, &b_use_USB_webcam);

                if (current_source == USB_WEBCAM)
                {
                    if (b_use_USB_webcam)
                    {
                        if (b_full_video_selected)
                            resetImageSource( pCap2, USB_WEBCAM, NULL, old_capture_dev, aWidth, aHeight, DEFAULT_FRAMERATE);

                        // if full video, no need to delay what has to be displayed
                        b_full_video_selected = false; 

                        if (ImGui::Button(SELECTED_WEBCAM_BUTTON))
                            ImGui::OpenPopup(WEBCAM_CHOICE_POPUP);

                        ImGui::SameLine();
                        //ImGui::Text("%s", webcam_names[capture_dev]);
                        ImGui::TextUnformatted(webcam_names[capture_dev]);

                        if (ImGui::BeginPopup(WEBCAM_CHOICE_POPUP))
                        {
                        for (int i = 0 ; i < IM_ARRAYSIZE(webcam_names); i++)
                            {
                               if (ImGui::Selectable(webcam_names[i]))
                                    capture_dev = i;
                            }
                            ImGui::EndPopup();
                        }

                        /* Choix dimensions de l'image */
#ifdef HIGH_DPI_SCREEN
                        const char * images_sizes[] = { " 640 x 360 ", " 800 x 600", " 960 x 540", "1280 x 720 ", "1360 x 768 ", "1440 x 900 ", "1920 x 1080 ", "2560 x 1440 ", "4096 x 2160 " };
                        const int width_sizes[] =  { 640, 800, 960, 1280, 1360, 1440, 1920, 2560, 4096 };
                        const int height_sizes[] = { 360, 600, 540, 720, 768, 900, 1080, 1440, 2160 };
#else
                        const char * images_sizes[] = { " 640 x 360 ", " 800 x 600", " 960 x 540", " 1280 x 720 ", "1360 x 768" };
                        const int width_sizes[] =  { 640, 800, 960, 1280, 1360 };
                        const int height_sizes[] = { 360, 600, 540, 720, 768 };
#endif
                        if (ImGui::Button(IMAGE_SIZE_IN_PIXELS_BUTTON))
                            ImGui::OpenPopup(IMAGE_FORMAT_POPUP);

                        ImGui::SameLine();
                        ImGui::TextUnformatted(images_sizes[image_format]);

                        if (ImGui::BeginPopup(IMAGE_FORMAT_POPUP))
                        {
                            for (int i = 0 ; i < IM_ARRAYSIZE(images_sizes); i++)
                            {
                                if (ImGui::Selectable(images_sizes[i]))
                                {
;                                    aWidth  = width_sizes[i];
                                     aHeight = height_sizes[i];
                                    image_format = i;
                                }
                            }
                            ImGui::EndPopup();
                        }

                        if ((capture_dev != old_capture_dev) || (image_format != old_image_format))
                        {
                            // IMPROVE ME : implement sort of array of delays, one per webcam
                            // reset the delay, saves a lot of ram when switching webcams
                            if (b_display_delayed)
                                delay = fdelay = 1;

                            int noError = 0 ;

                            if (resetImageSource( pCap2, USB_WEBCAM, NULL, capture_dev, aWidth, aHeight, DEFAULT_FRAMERATE) != noError)
                            {
                                capture_dev = old_capture_dev;
                                image_format = old_image_format;
                                std::cout << "Capture dev not existing" << std::endl;
                            }
                            else
                            {
                                old_capture_dev = capture_dev;
                                old_image_format = image_format;
                            }
                        }

                        if (ImGui::Button(RESET_THE_WEBCAM_BUTTON))
                            resetImageSource(pCap2, USB_WEBCAM, NULL, old_capture_dev, aWidth, aHeight, DEFAULT_FRAMERATE);

                        ImGui::NewLine();
                        ImGui::PushItemWidth(140);

                        static int current_manual_focus = 0;
                        static int old_manual_focus = 0;
                        static bool b_focus_auto = true;
                        static bool b_old_focus_auto = true;
                        static double focus_min = 0;
                        static double focus_max = 50;

                        ImGui::Text(AUTOMATIC_FOCUS);
                        ImGui::SameLine();
                        ImGui::Checkbox("", &b_focus_auto);

                        if (!b_focus_auto)
                        {
                            ImGui::PushID(FOCUS_VALUE);
                            ImGui::Text(CURRENT_FOCUS_VALUE);
                            ImGui::SameLine();
                            ImGui::SliderInt("", &current_manual_focus, focus_min, focus_max, "%.2f"); // fixme : implement a getter()
                            ImGui::PopID();

                            if (b_old_focus_auto != b_focus_auto)
                            {
                                current_manual_focus = 0;
                                cap2.set(CV_CAP_PROP_FOCUS, (double)0.0f);
                                b_old_focus_auto = b_focus_auto;
                                cap2.set(CV_CAP_PROP_AUTOFOCUS, 1);
                            }
                        }

                        if ((old_manual_focus != current_manual_focus) && (!b_focus_auto))
                        {
                            cap2.set(CV_CAP_PROP_FOCUS, (double)(current_manual_focus * 5.0f) / 250.0f);
                            old_manual_focus = current_manual_focus;
                        }

                        if ((b_old_focus_auto != b_focus_auto) && (b_focus_auto))
                        {
                            current_manual_focus = 0;
                            cap2.set(CV_CAP_PROP_FOCUS, (double)0.0f);
                            b_old_focus_auto = b_focus_auto;
                            cap2.set(CV_CAP_PROP_AUTOFOCUS, 1);
                        }

                        ImGui::PopItemWidth();

#ifdef EXPOSURE_FEATURE
                        static bool b_exposure_auto = true;
                        static bool b_old_exposure_auto = true;
                        static int exposure_min = 0;
                        static int exposure_max = 2047;

                        ImGui::Text(AUTOMATIC_LIGHT_EXPOSITION);
                        ImGui::SameLine();
                        ImGui::Checkbox(" ", &b_exposure_auto);

                        if (!b_exposure_auto)
                        {
                            ImGui::PushItemWidth(120.0f);
                            ImGui::PushID(LIGHT_EXPOSITION_VALUE);
                            ImGui::Text(CURRENT_LIGHT_EXPOSITION_VALUE);
                            ImGui::SameLine();
                            ImGui::SliderInt("", &current_manual_exposure, exposure_min, exposure_max, "%d"); // fixme : implement a getter()
                            ImGui::PopID();
                            ImGui::PopItemWidth();
                        }

                        if ((old_manual_exposure != current_manual_exposure) && (!b_exposure_auto))
                        {
                            // See https://github.com/opencv/opencv/issues/9738
                            cap2.set(CV_CAP_PROP_AUTO_EXPOSURE, 0.25); //MINIDART_V4L2_EXPOSURE_MANUAL);// == 1 );
                            cap2.set(CV_CAP_PROP_EXPOSURE, (double)current_manual_exposure / 2047.0f);
                            old_manual_exposure = current_manual_exposure;
                        }

                        if ((b_old_exposure_auto != b_exposure_auto) && (b_exposure_auto))
                        {
                            current_manual_exposure = 0;
                            b_old_exposure_auto = b_exposure_auto;
//                        cvSetCaptureProperty( pCap, CV_CAP_PROP_AUTO_EXPOSURE, /*CV_CAP_EXPOSURE_APERTURE_PRIORITY*/ MINIDART_V4L2_EXPOSURE_APERTURE_PRIORITY);
                            cap2.set(CV_CAP_PROP_AUTO_EXPOSURE, 0.75); //MINIDART_V4L2_EXPOSURE_APERTURE_PRIORITY); //  == 3 );
//                        cap.set(CV_CAP_PROP_SPEED, 0 /*  == 3 */);
#ifdef DEBUG
                            fprintf(stdout, "Exposition auto ?  : %f\n",cap2.get(CV_CAP_PROP_AUTO_EXPOSURE));
#endif
                        }

#endif /* EXPOSURE_FEATURE */

/////  anyway, not working well on Windows ...
/////                      // -36000/+36000 on Linux
                      #define PAN_STEP 3600
                      #define TILT_STEP 3600
                        static int current_absolute_pan = 0;
                        static int old_absolute_pan = 0;
                        static const int pan_min = -10;
                        static const int pan_max = 10;

                        static int current_absolute_tilt = 0;
                        static int old_absolute_tilt = 0;
                        static const int tilt_min = -10;
                        static const int tilt_max = 10;

                        ImGui::PushID(PAN_VALUE);
                        ImGui::Text(PAN_VALUE);
                        ImGui::SameLine();
                        ImGui::SliderInt("", &current_absolute_pan, pan_min, pan_max, "%d"); // fixme : implement a getter()
                        ImGui::PopID();

                        if (old_absolute_pan != current_absolute_pan)
                        {
                            cap2.set(CV_CAP_PROP_PAN, (double)current_absolute_pan*PAN_STEP);
                            old_absolute_pan = current_absolute_pan;
                            std::cerr << CURRENT_PAN_VALUE << cap2.get(CV_CAP_PROP_PAN)-0.5 << std::endl;
                        }

                        ImGui::PushID(TILT_VALUE);
                        ImGui::Text(TILT_VALUE);
                        ImGui::SameLine();
                        ImGui::SliderInt("", &current_absolute_tilt, tilt_min, tilt_max, "%d"); // fixme : implement a getter()
                        ImGui::PopID();

                        if (old_absolute_tilt != current_absolute_tilt)
                        {
                            cap2.set(CV_CAP_PROP_TILT, (double)current_absolute_tilt*TILT_STEP);
                            old_absolute_tilt = current_absolute_tilt;
                            std::cerr << CURRENT_TILT_VALUE << cap2.get(CV_CAP_PROP_TILT)-0.5f << std::endl;
                        }

                        static float current_absolute_zoom = 1.0f;
                        static float old_absolute_zoom = 0;
                        static const float zoom_min = 1.0f;
                        static const float zoom_max = 5.0f;

                        ImGui::PushID(ZOOM_VALUE);
                        ImGui::Text(ZOOM_VALUE);
                        ImGui::SameLine();
                        ImGui::SliderFloat("", &current_absolute_zoom, zoom_min, zoom_max, "%.1f"); // fixme : implement a getter()
                        ImGui::PopID();

                        if (old_absolute_zoom != current_absolute_zoom)
                        {
                            cap2.set(CV_CAP_PROP_ZOOM, (double)current_absolute_zoom*100);
                            old_absolute_zoom = current_absolute_zoom;
                            std::cerr << CURRENT_ZOOM_VALUE << 4.0f*cap2.get(CV_CAP_PROP_ZOOM)+1.0f << std::endl;
                        }

                        if (ImGui::Button(RESET_PAN_AND_TILT_VALUE_BUTTON))
                        {
                            cap2.set(CV_CAP_PROP_PAN, 0);
                            cap2.set(CV_CAP_PROP_TILT, 0);
                            current_absolute_tilt = 0;
                            current_absolute_pan = 0;

                            std::cerr << "Réinitialisation des valeurs de Pan et Tilt ... " << std::endl;
                            std::cerr << "Pan actuel : " << cap2.get(CV_CAP_PROP_PAN)-0.5f << std::endl;
                            std::cerr << "Tilt actuel : " << cap2.get(CV_CAP_PROP_TILT)-0.5f << std::endl;
                        }
                    }

                } /* current_source == USB_WEBCAM */

                else if ((current_source == IP_WEBCAM)||(current_source == NETWORK))
                {
                    b_full_video_selected = true; // do not delay what is displayed
                    b_cell_phone_selected = false;

                    // TESTED WORKING using vlc : http://camera1.mairie-brest.fr/mjpg/video.mjpg?resolution=320x240
                    //                            rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov
                    //     https://streams.videolan.org/samples/Matroska/
                    //                    static char flux_reseau[128] = "http://88.53.197.250:80/axis-cgi/mjpg/video.cgi?resolution=320x240";

                    static char flux_reseau[512] = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
                    char buf_flux_reseau[1024]; sprintf(buf_flux_reseau, FLOW_URL, flux_reseau); // ### operator override ID ignoring the preceeding label

                    // FIXME
                    ImGui::PushID(NETWORK_FLOW);
                    ImGui::NewLine();
                    ImGui::Button(buf_flux_reseau);

                    if (ImGui::BeginPopupContextItem(NETWORK_FLOW))
                    {
                        ImGui::Text(ENTER_URL);
                        ImGui::InputText("##insert", flux_reseau, IM_ARRAYSIZE(flux_reseau));
                        if (ImGui::Button(OK_BUTTON2))
                        {
                            std::cout << ENTERED_WEBCAM_IP_VALUE <<  flux_reseau << std::endl;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                    ImGui::NewLine();

                    if (!b_video_running)
                    {
                        if (ImGui::Button(START_THE_DIFFUSION_BUTTON))
                        {
                            resetImageSource(pCap2, IP_WEBCAM, flux_reseau, 0, 0, 0, 0.0f);
                            LoadFile(flux_reseau, IP_WEBCAM);
                        }
                    }
                    else
                    {
                        if (ImGui::Button(CLOSE_THE_FLOW_BUTTON))
                        {
                            b_video_running = false;
                            do_exit(is);
                        }
                    }
                } /* (current_source == IP_WEBCAM)||(current_source == NETWORK) */
                else if (current_source == CELL_PHONE)
                {
                    b_cell_phone_selected = true;
                    b_full_video_selected = true;

                    static char cell_phone_url[512] = "http://";
                    char buf_cell_phone_url[1024]; sprintf(buf_cell_phone_url, FLOW_URL, cell_phone_url); // ### operator override ID ignoring the preceeding label

                    ImGui::NewLine();
                    ImGui::Text(" Exemple : http://192.168.1.23:4747");

                    ImGui::PushID("cell_phone");
                    ImGui::NewLine();
                    ImGui::Button(buf_cell_phone_url);

                    if (ImGui::BeginPopupContextItem("cell_phone"))
                    {
                        ImGui::Text(ENTER_URL);
                        ImGui::InputText(" ", cell_phone_url, IM_ARRAYSIZE(cell_phone_url));
                        if (ImGui::Button(OK_BUTTON2))
                        {
                            std::cout << "adresse IP du téléphone : " <<  cell_phone_url << std::endl;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                    ImGui::NewLine();

                    if (!b_video_running)
                    {
                        if (ImGui::Button(START_BUTTON))
                        {
                            resetImageSource(pCap2, CELL_PHONE, cell_phone_url, 0, 0, 0, 0.0f);
                            LoadFile(cell_phone_url, CELL_PHONE);
                        }
                    }
                    else
                    {
                        if (ImGui::Button(CLOSE_THE_FLOW_BUTTON))
                        {
                            b_video_running = false;
                            do_exit(is);
                        }
                    }
                } /* current_source == ELL_PHONE */
                else if (current_source == FULL_VIDEO)
                {
                    b_full_video_selected = true;
                    b_cell_phone_selected = false;

                    if(b_video_running)
                    {
                        if (ImGui::Button(CLOSE_THE_VIDEO_BUTTON))
                        {
                            b_video_running = false;
                            do_exit(is);
                        }
                    }
                    else
                    {
                        if (ImGui::Button(OPEN_A_VIDEO_BUTTON) /*|| (b_open_a_file == true)*/)
                        {
                            resetImageSource(pCap2, FULL_VIDEO, 0, 0, 0, 0, 0.0f);
                            LoadFile(NULL, FULL_VIDEO);

                            // using SDL2_ffmpeg                            // VIDEO_FILE : image and sound. 
                            // Must stop at the end, and do nothing, avoiding capture dev to crash
                            b_open_a_file = false;
                        }
                    }
                } /* current_source == FULL_VIDEO */


                if (!b_paused)
                {
                    ImGui::PushID(OUTLINE_THICKNESS_VALUE);
                    ImGui::Checkbox( ICON_FA_SEARCH ZOOM_ENABLE_CHECKBOX, &b_zoom_available);

                    if (ImGui::IsItemClicked(0))
                    {
                        std::cout << "Loupe activée" << std::endl;
                        std::cout << "(before) p_aDrawnObject->anObjectType : " << p_aDrawnObject->anObjectType << std::endl;
                        p_aDrawnObject->anObjectType = NOT_A_DRAWN_OBJECT;
                        std::cout << "(after) p_aDrawnObject->anObjectType : " << p_aDrawnObject->anObjectType << std::endl;
                    }

                    ImGui::PushItemWidth(80.0f);
                    ImGui::SliderFloat(" ", &outline_thickness, 2.0f, 4.0f, "###Épaisseur du trait: ");
                    ImGui::SameLine();
                    ImGui::Text(OUTLINE_THICKNESS_VALUE);
                    ImGui::PopItemWidth();
                    ImGui::PopID();

                    if (!b_zoom_available)
                    {
                        ImGui::Text(SELECTED_AREA_COLOR);
                        ImGui::PushItemWidth(180.0f);
                        ImGui::ColorEdit3(" ", &col.x);
                        ImGui::PopItemWidth();
                    }
                    else
                        ImGui::NewLine();
                }
                else
                {
                    ImGui::NewLine();
                    ImGui::NewLine();
                }

                ImGui::Checkbox(DEFINE_A_RECTANGULAR_AREA_IN_THE_IMAGE, &b_record_rectangle);

                if (ImGui::Button(IMAGES_WIDTH_HEIGHT_RATIO))
                    ImGui::OpenPopup(CHANGE_THE_IMAGES_WIDTH_HEIGHT_RATIO);

                if (ImGui::BeginPopupModal(CHANGE_THE_IMAGES_WIDTH_HEIGHT_RATIO, NULL, ImGuiWindowFlags_None))
                {
                    // default ratio = 720p 
                    static int ratio_choice = 0;

                    /* Choix de la touche pour les raccourcis claviers */
                    ImGui::Text(IMAGES_WIDTH_HEIGHT_RATIO);
                    ImGui::RadioButton( _16_9_SCREEN              , &ratio_choice, 0);
                    ImGui::RadioButton( _16_10_MACBOOK_PRO_EXEMPLE, &ratio_choice, 1);
                    ImGui::RadioButton( _4_3_CLASSICAL_TV_SCREEN  , &ratio_choice, 2);
                    ImGui::RadioButton( _5_4_SCREEN               , &ratio_choice, 3);

                    switch(ratio_choice)
                    {
                        case 0 : ratio = RATIO(720.0f,1280.0f);
                            frame_width = 1280.0f;
                            break;

                        case 1 : ratio = RATIO(10.0f,16.0f);
                            frame_width = 1280.0f;
                            break;

                        case 2 : ratio = RATIO(3.0f,4.0f);
                            frame_width = 1024.0f;
                            break;

                        case 3 : ratio = RATIO(4.0f,5.0f);
                            frame_width = 1024.0f;
                            break;

                        default:
                            break;
                    }
                    frame_height= frame_width * ratio;


                    if (ImGui::Button(OK_BUTTON, ImVec2(120,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                } // BeginPopupModal()


                if (ImGui::Button(CHANGE_IMAGES_ORIENTATION_BUTTON))
                    ImGui::OpenPopup(IMAGES_ROTATION);

                if (ImGui::BeginPopupModal(IMAGES_ROTATION, NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    static int rotation = 0;

                    ImGui::RadioButton( ZERO_ROTATION        , &rotation, 0);
                    ImGui::RadioButton( CLOCKWISE_90_DEGREES_ROTATION  , &rotation, 1);
                    ImGui::RadioButton( _180_DEGREES_ROTATION          , &rotation, 2);
                    ImGui::RadioButton( TRIGO_90_DEGREES_ROTATION      , &rotation, 3);

                    switch(rotation)
                    {
                        case 0 : image_rotation_value = NO_ROTATION; break;
                        case 1 : image_rotation_value = ROTATE_90_CLOCKWISE; break;
                        case 2 : image_rotation_value = ROTATE_180; break;
                        case 3 : image_rotation_value = ROTATE_90_COUNTERCLOCKWISE; break;

                        default:
                            image_rotation_value = NO_ROTATION;
                        break;
                    }
                    ImGui::NewLine();

                    ImGui::PushID(HORIZONTAL_FLIP);
                    ImGui::Checkbox(HORIZONTAL_FLIP, &b_flip_image);
                    ImGui::SameLine();
                    ImGui::Text(APPLIED_BEFORE_THE_ROTATION);
                    ImGui::NewLine();
                    ImGui::PopID();

                    ImGui::SetItemDefaultFocus();

                    if (ImGui::Button(OK_BUTTON, ImVec2(120,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::EndPopup();
                } // BeginPopupModal()
                ImGui::Unindent(indentation);
                ImGui::EndGroup();
            }
            ImGui::EndChild(); // child2

            ImGui::Dummy (ImVec2(80.0f, 0.0f)); ImGui::SameLine();

            //------------------------------------------------
            ImGui::BeginChild("child3", ImVec2(w2 + 100.0f, h2), true);
            ImGui::Text(ANNOTATIONS_CHILD_WINDOW);

//  CANVAS IN VIDEO
                static bool b_create_canvas = false;

                ImGui::AlignTextToFramePadding(); ImGui::SameLine();

                ToggleButton(ANNOTATE_THE_VIDEO, &b_create_canvas);

                if (b_create_canvas)
                {
                    // FIXME
                    int iconWidth   = 32;
                    int iconHeight  = 32;
                    int frame_padding = 4;

/////////////////////////////////////////////////////////////////////////

                    static bool alpha_half_preview = true ;
                    static bool drag_and_drop = true;
                    static bool options_menu = true;
                    static float object_thickness = 2.5f;
                    static ImVec4 bcol = ImVec4( 0.3f, 0.4f, 1.0f, 0.5);
                    // LATER USE static ImVec4 ocol = ImVec4( 0.4f, 0.4f, 0.4f, 0.5);

                    if (p_delayTabDrawnObjects->size() > 0)
                    {
                        ImGui::SameLine();

                        if (ImGui::Button(DELETE_ALL_BUTTON))
                            ImGui::OpenPopup("Tout effacer ?");

                        if (ImGui::BeginPopupModal("Tout effacer ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::Text(ALL_ANNOTATIONS_WILL_BE_CANCELED_Q);

                            if (ImGui::Button(CONFIRM_BUTTON, ImVec2(120,0)))
                            {
                                while (!(p_delayTabDrawnObjects->empty()))
                                {
                                    p_delayTabDrawnObjects->pop_back();
                                }
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::SameLine();

                            if (ImGui::Button(CANCEL_BUTTON, ImVec2(120,0)))
                            {
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }

                        ImGui::SameLine();

                        if (ImGui::Button(UNDO_BUTTON)) // || cancel_last_action)
                        {
                            p_delayTabDrawnObjects->pop_back();
                        }
                    }

                    createCanvasObjectsImagesTexIds();

                    for (int i = 0 ; i < CANVAS_OBJECTS_TYPES_MAX ; i++)
                    {
                        ImGui::ImageButton( reinterpret_cast<void * >(canvasObjectImageTexId[i]), ImVec2(iconWidth, iconHeight),
                                            ImVec2(0,0), ImVec2(1,1), frame_padding, ImColor(0,0,0,255));
                        ImGui::PushID(i);
                        if (ImGui::IsItemClicked(0))
                            selectedObject = i;
                        ImGui::PopID();
                        ImGui::SameLine();
                    }

                    if (selectedObject != TEXT_OBJECT)
                    {
                        ImGuiColorEditFlags misc_flags = /*(hdr ? ImGuiColorEditFlags_HDR : 0) |*/ (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | ImGuiColorEditFlags_AlphaPreviewHalf | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
                        misc_flags |= ImGuiColorEditFlags_NoInputs;

                        ImGui::NewLine();
                        ImGui::Text(DRAWN_OBJECT_COLOR);
                        ImGui::SameLine();
                        ImGui::PushItemWidth(30);
                        ImGui::ColorEdit4("  ", &bcol.x, misc_flags);
                        ImGui::PopItemWidth();
/*
    LATER USE
                        ImGui::SameLine();

                        ImGui::Text(DRAWN_OBJECT_OUTLINE_COLOR);
                        ImGui::SameLine();
                        ImGui::PushItemWidth(30);
                        ImGui::ColorEdit4(" ", &ocol.x, misc_flags);
                        ImGui::PopItemWidth();
*/
                        ImGui::SameLine();

                        ImGui::Text(DRAWN_OBJECT_LINE_THICKNESS);
                        ImGui::SameLine();
                        ImGui::PushItemWidth(150);
                        ImGui::SliderFloat(" ", &object_thickness, 0.5, 20, "%.1f\n" );
                        ImGui::PopItemWidth();

                        p_aDrawnObject->objBackgroundColor =  ImColor(bcol.x, bcol.y, bcol.z, bcol.w);
                        p_aDrawnObject->thickness = object_thickness;
                        // LATER USE p_aDrawnObject->objOutlineColor    =  ImColor(ocol.x, ocol.y, ocol.z, ocol.w);

                        ImGui::Checkbox(HALF_ALPHA_PREVIEW, &alpha_half_preview); ImGui::SameLine();
                        ImGui::Checkbox(WITH_DRAG_AND_DROP, &drag_and_drop); ImGui::SameLine();
                        ImGui::Checkbox(WITH_OPTIONS_MENU, &options_menu);//ImGui::SameLine() ; HelpMarker(RIGHT_CLICK_FOR_INDIVIDUAL_COLOR_WIDGET_OPTIONS);
                    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FIXME : call the canvas() from there !

                    ImGui::NewLine();

                    switch (selectedObject)
                    {
                        case TEXT_OBJECT:
                            pTextCanvas->pTextObject->b_displayable = text_in_video_helper(pTextCanvas);
                            current_delayTab_drawing_task = DRAWING_TEXT; 
                        break;

                        case FILLED_RECTANGLE:
                            ImGui::NewLine();
                            //addFilledObjectParameters(); //  type = FILLED_RECTANGLE, filled_color, outline color
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        case EMPTY_RECTANGLE:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        case FILLED_ELLIPSE:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        case EMPTY_ELLIPSE:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        case RANDOM_LINE:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = FREEHAND_DRAWING; 
                        break;

                        case RANDOM_ARROW:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = FREEHAND_DRAWING; 
                        break;

                        case SIMPLE_ARROW:
                            ImGui::NewLine();
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        case SIMPLE_LINE:
                          ImGui::NewLine();
                            current_delayTab_drawing_task = DRAWING_PRIMITIVE; 
                        break;

                        default:
                          break;
                    }
                }

// END CANVAS IN VIDEO

            ImGui::EndChild();
            ImGui::EndTabItem(); // END DELAY_TAB
        }
/////////////////////////////////////////////////////////////////////////////////////////////


        if (ImGui::BeginTabItem(ICON_FA_COGS " " ICON_FA_WPFORMS ANALYSIS_WORKSHOP))
        {
            current_tab = ANALYSIS_TAB;
            static float myW = 960.0f;
            static float myH = 540.0f;
//            ImGui::Dummy(ImVec2(0.0f,6.0f));

            ImGui::BeginGroup();
            {
                ImGui::Dummy(ImVec2(12.0f,4.0f));

                if (!pClipReader->getCurrentClipFile())
                {
                    backgroundTextureId3 = glConvertMatToTexture(backgroundImage);
                    ImGui::Image(reinterpret_cast<void * >(backgroundTextureId3), ImVec2(myW, myH), ImVec2(0,0), ImVec2(1,1),
                                 ImColor(255,255,255,255), ImColor(255,255,255,255));
                }
                else
                {
                    backgroundTextureId3 = glConvertMatToTexture(aClipReader.frames[currentFrameNumber]);
                    ImGui::Image(reinterpret_cast<void * >(backgroundTextureId3), ImVec2(myW, myH), ImVec2(0,0), ImVec2(1,1),
                                 ImColor(255,255,255,255), ImColor(255,255,255,255));
                }

                static int fps_min = 5;
                static int fps_max = 60;
                static int current_fps = 30;
                static unsigned int old_time = 0;
                static unsigned int current_time = 0;
                ImGui::NewLine();

                ImGui::BeginGroup();
                {
                    if (!pClipReader->getCurrentClipFile())
                    {
                        if (ImGui::Button( ICON_FA_FILM " " OPEN_A_CLIP_BUTTON) || (b_open_a_file ==  true))
                        {
                            LoadFile(NULL, VIDEO_CLIP);

                            if (pClipReader)
                                lastFrameNumber = aClipReader.getMaxFrame();

                            b_open_a_file = false;
                        }
                    }
                    else
                    {
                        if (ImGui::Button(CLOSE_THE_CLIP_BUTTON))
                        {
                            aClipReader.setFile(NULL);
                            aClipReader.Close();
                        }
                    }

                    if (ImGui::Button(WORKING_DIRECTORY_BUTTON))
                        selectFolder(currentPath);

                }
                ImGui::EndGroup();

                ImGui::SameLine();
                ImGui::PushItemWidth(280);

                if (pClipReader->getCurrentClipFile())
                {
                    ImGui::BeginGroup();
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_SliderThicknessScale, 0.6f);
                        ImGui::SliderInt(IMAGE_CURRENTLY_DISPLAYED, &currentFrameNumber,1,lastFrameNumber-1);
                        ImGui::SliderInt(FRAMES_PER_SECOND, &current_fps,fps_min,fps_max); ImGui::SameLine();
                        ImGui::PopStyleVar();
                    }
                    ImGui::EndGroup();

                    ImGui::SameLine();
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Button("  " ICON_FA_FAST_BACKWARD "  "))
                        {
                            currentFrameNumber -= 10;

                            if (currentFrameNumber <= 0)
                                currentFrameNumber = 1;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button("  " ICON_FA_STEP_BACKWARD "  "))
                        {
                            currentFrameNumber -= 1;

                            if (currentFrameNumber <= 0)
                                currentFrameNumber = 1;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button(" " ICON_FA_STOP " "))
                        {
                            currentFrameNumber = 1;
                            b_playing_at_fps   = false;
                        }
                        ImGui::SameLine();

                        if (ImGui::Button(b_playing_at_fps ? " " ICON_FA_PAUSE " " : " " ICON_FA_PLAY " "))
                            b_playing_at_fps = !b_playing_at_fps;

                        ImGui::SameLine();

                        if (ImGui::Button("  " ICON_FA_STEP_FORWARD "  "))
                        {
                            currentFrameNumber += 1;

                            if (currentFrameNumber > (lastFrameNumber -1))
                                currentFrameNumber = lastFrameNumber -1;
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("  " ICON_FA_FAST_FORWARD "  "))
                        {
                            currentFrameNumber += 10;

                            if (currentFrameNumber > (lastFrameNumber -1))
                                currentFrameNumber = lastFrameNumber -1;
                        }

                        if (b_playing_at_fps)
                        {
                            current_time = SDL_GetTicks();

                            if (pClipReader->playAtGivenFps(current_fps, current_time, old_time))
                            {
                                if (currentFrameNumber < (lastFrameNumber - 1))
                                    currentFrameNumber++;

                                old_time = current_time;
                            }
                        }
                        ImGui::Text(RENDERING_TIME_FPS_DISPLAY, 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    }
                    ImGui::EndGroup();
                }
                ImGui::PopItemWidth();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ImGui::Text(CANVAS_GROUP);

                ImGui::NewLine();
                ImGui::Text(CANVAS_STRING_LINE01);
                ImGui::Text(CANVAS_STRING_LINE02);
                ImGui::Text(CANVAS_STRING_LINE03);
                ImGui::Text(CANVAS_STRING_LINE04);
                ImGui::Text(CANVAS_STRING_LINE05);
                ImGui::Text(CANVAS_STRING_LINE06);
                ImGui::Text(CANVAS_STRING_LINE07);
                ImGui::Text(CANVAS_STRING_LINE08);
                ImGui::Text(CANVAS_STRING_LINE09);
                ImGui::NewLine();
                ImGui::Text(CANVAS_STRING_LINE10);
                ImGui::Text(CANVAS_STRING_LINE11);
                ImGui::Text(CANVAS_STRING_LINE12);
                ImGui::Text(CANVAS_STRING_LINE13);
                ImGui::Text(CANVAS_STRING_LINE14);
                ImGui::Text(CANVAS_STRING_LINE15);
                ImGui::Text(CANVAS_STRING_LINE16);
                ImGui::Text(CANVAS_STRING_LINE17);
                ImGui::Text(CANVAS_STRING_LINE18);

                ImGui::NewLine();

                // default to 960 x 540 pixels
                static int choosen_format = 2;
#ifdef HIGH_DPI_SCREEN
                /* Dimensions des images pour le slideshow*/
                const char * frameSizes[] = { " 640 x 360 ", " 800 x 600", " 960 x 540", " 1280 x 720 ", " 1360 x 768 ", "1440 x 900 ", "1920 x 1080 " };
                const int WSizes[] =  { 640, 800, 960, 1280, 1360, 1440, 1920 };
                const int HSizes[] = { 360, 600, 540, 720, 768, 900, 1080 };
#else
                const char * frameSizes[] = { " 640 x 360 ", " 800 x 600", " 960 x 540", " 1280 x 720 ", " 1360 x 768 " };
                const int WSizes[] =  { 640, 800, 960, 1280, 1360 };
                const int HSizes[] = { 360, 600, 540, 720, 768 };
#endif
                if (ImGui::Button(IMAGES_FORMAT_IN_PIXELS))
                    ImGui::OpenPopup(IMAGES_FORMAT_POPUP);

                ImGui::TextUnformatted(frameSizes[choosen_format]);

                if (ImGui::BeginPopup(IMAGES_FORMAT_POPUP))
                {
                    for (int i = 0 ; i < IM_ARRAYSIZE(frameSizes); i++)
                    {
                        if (ImGui::Selectable(frameSizes[i]))
                        {
                            myW  = WSizes[i];
                            myH = HSizes[i];
                            choosen_format = i;
                        }
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::EndGroup();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem( ICON_FA_BAR_CHART FIGURES_TAB))
        {
            current_tab = STATS_TAB;
            b_spot_active = false;

            ImGui::BeginChild("Actions", ImVec2(260.0f, 683.0f), true);
            {
                ////switch(selectedPlayer)
                ////{
                ////    case NORMAL_PLAYER:
                        checkPlayersActions();
                ////      break;
                ////    case GK_PLAYER:
                ////    checkGoalkeeperActions();
                ////      break;
                ////}
            }
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginGroup();

            ImGui::BeginChild("Actions 3", ImVec2(1100.0f, 580.0f), true);
            {
                ImGui::NewLine();
                ImGui::Text(TEAM_A);
                ImGui::NewLine();

                ImGuiTabBarFlags tabbar2_flags = ImGuiTabBarFlags_None|ImGuiTabBarFlags_Reorderable;

                if(ImGui::BeginTabBar(PLAYERS_LIST,tabbar2_flags))
                {
                    if (ImGui::BeginTabItem(ALL_ACTIONS))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(GAME_ACTIONS_OF_ALL_PLAYERS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem(PLAYER_01))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_1_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_02))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_2_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_03))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_3_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_04))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_4_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_05))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_5_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_06))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_6_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_07))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_7_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_08))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_8_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_09))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_9_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_10))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_10_ACTIONS);

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem( PLAYER_11))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_11_ACTIONS);

                        ImGui::EndTabItem();
                    }

                    if (ImGui::BeginTabItem( PLAYER_12))
                    {
                        ImGui::NewLine();
                        ImGui::NewLine();
                        ImGui::Text(ALL_PLAYER_12_ACTIONS);

                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
            ImGui::EndChild();

            ImGui::BeginChild("Actions 2", ImVec2(1100.0f, 100.0f), true);
            {
                ImGui::NewLine();
                ImGui::Text("Événement :  Championnat France -18F"); ImGui::SameLine(); ImGui::Text("Date : 10/02/2019 "); ImGui::SameLine();ImGui::Text("Chrono :  "); ImGui::SameLine(); ImGui::Text("00:10:32 ");

                ImGui::SameLine(); ImGui::Dummy(ImVec2(200.0f,0.0f)); ImGui::SameLine();

                if (ImGui::Button("Aide pour la saisie"))
                    ImGui::OpenPopup("Saisie des actions de jeu");

                if (ImGui::BeginPopupModal("Saisie des actions de jeu", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    // FIXME : add help bullets
                    ImGui::Text("    FONCTIONNEMENT  ");
                    ImGui::Text("    Cet onglet va vous permettre de faire un suivi dynamique de joueuses / joueurs pendant un match");
                    ImGui::TextWrapped("     Type de suivi :\n"
                    "    - Entraînement ou match : chronomètre en fonctionnement + mémorisation du temps\n"
                    "    - GB [buts pris ou arrêt ou hors cadre,\n"
                    "          type/forme de tir\n"
                    "          distance et axe du tir\n"
                    "          localisation,\n"
                    "          arrêts]\n"
                    "    - joueur de champ\n"
                    "    - toute l'équipe / un onglet par joueur\n"
                    "\n"
                    "    Fonctionnement :\n"
                    "      - cocher la case : entraînement ou match // Radiobox\n"
                    "        Si match : renseigner les équipes + la date + le championnat\n"
                    "\n"
                    "      - cocher la case du type de suivi : GB ou joueurs de champ\n"
                    "        si GB :\n"
                    "          - proposer des cases cf FDME, mais avec impacts + axes du tir pour faire le suivi\n"
                    "          - afficher les pourcentages\n"
                    "          - dessiner les stats ?\n"
                    "\n"
                    "        si joueurs\n"
                    "        - renseigner le nom + le numéro des joueurs suivis\n"
                    "        - proposer les cases type FDME pour faire le suivi (le temps est ajouté si match\n"
                    "\n"
                    "    FINALISATION : envoyer les stats dans une feuille de type tableur\n"
                    "    // FIXME format OUVERT, ne pas écraser les fichiers entre eux\n"
                    "    REINITIALISATION : bouton pour remettre à zéro les stats\n");

                    ImGui::Dummy(ImVec2(250.0f, 0.0f)); ImGui::SameLine();
                    ImGui::SetItemDefaultFocus();

                    if (ImGui::Button("OK", ImVec2(120,0)))
                        ImGui::CloseCurrentPopup();

                    ImGui::NewLine();
                    ImGui::Text("    Auteur du logiciel miniDart : Eric Bachard © 2016-2019");

                    ImGui::EndPopup();
                } // BeginPopupModal()

                ImGui::Text("TODO : ajouter horloge programmable ");

            }
            ImGui::EndChild();

            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_SEARCH GK_FOLLOW_UP_TAB_ITEM))
        {
            current_tab = GK_STATS_TAB;
            b_spot_active = false;


            ImGui::Text(STATISTICS_AND_GK_FOLLOW_UP);
            static int match_officiel = 0;
            static int nombre_arrets_GB = 0;
            static int nombre_arrets_GB_7m = 0;
            static int nombre_de_buts_pris = 0;
            static int nombre_de_buts_pris_sur_7m = 0;
            static int nombre_de_poteaux = 0;
            static int nombre_de_tirs_hors_cadre = 0;
            static int nombre_de_tirs = 0;
            static int nombre_de_penalties = 0;
            static float pourcentage_arrets = 0.0f;
            static float pourcentage_arrets_7m = 0.0f;
            static float pourcentage_reussite = 0.0f;
            static float pourcentage_reussite_7m = 0.0f;
            static ImVector <int> unTir;
            static ImVector <int> unPenalty;
            static bool saisie_complete = false;

            /*       Nom du GB        */
            ImGui::SameLine(); ImGui::Text("         "); ImGui::SameLine();
            static char name[32] = "Julia";
            char buf[64]; sprintf(buf, "GB suivi(e) : %s###Toto", name); // ### operator override ID ignoring the previous label

            ImGui::PushID("name");
            ImGui::Button(buf);

            if (ImGui::BeginPopupContextItem("rename context menu"))
            {
                ImGui::Text(ENTER_THE_NAME);
                ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
                if (ImGui::Button("Ok"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::PopID();

            /*       Club        */
            ImGui::SameLine();
            static char club[32] = "HBC Pouloulou";
            char buf_club[64]; sprintf(buf_club, "Club : %s###Truc", club); // ### operator override ID ignoring the preceeding label

            ImGui::PushID("club");
            ImGui::Button(buf_club);

            if (ImGui::BeginPopupContextItem("club"))
            {
                ImGui::Text(ENTER_THE_CLUB);
                ImGui::InputText("##insert", club, IM_ARRAYSIZE(club));
                if (ImGui::Button(OK_BUTTON"Ok"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::PopID();

            /*       Championnat        */
            ImGui::SameLine(); ImGui::Text(" "); ImGui::SameLine();
            static int selected_level = 1;
            const char * names[] = { "LFH", "D2F", "N1F", "N2F", "N3F", "-18CF", "Reg" };

            if (ImGui::Button("Niveau de jeu : "))
                ImGui::OpenPopup("niveau_jeu");

            ImGui::SameLine();
            // both are roughly equivalent.
            // See:  https://github.com/ocornut/imgui/issues/1450
            // ImGui::Text("%s", names[selected_level]);
            ImGui::TextUnformatted(names[selected_level]);

            if (ImGui::BeginPopup("niveau_jeu"))
            {
                ImGui::Text(CHAMPIONSHIP);

                ImGui::NewLine();
                ImGui::Separator();

                for (int i = 0 ; i < IM_ARRAYSIZE(names); i++)
                   if (ImGui::Selectable(names[i]))
                        selected_level = i;
                ImGui::EndPopup();
            }

            /*       Équipes adverses        */
            if (match_officiel == 1)
            {
                ImGui::SameLine();
                static int selected_team = 10;
                const char* equipes[] = { "Aulnay", "Villemonble", "Noisy-Gagny",
                                           "Achenheim", "Ste Maure-Troyes", "Reims",
                                           "Echirolles", "Essahb", "Annecy", "Colmar", "Montigny les Metz" };
                if (selected_level == 3)
                {
                    if (ImGui::Button(OPPONENT))
                        ImGui::OpenPopup("adversaire");

                    ImGui::SameLine();
                    ImGui::TextUnformatted(equipes[selected_team]);

                    if (ImGui::BeginPopup("adversaire"))
                    {
                        ImGui::Text(OPPONENT);
                        ImGui::Separator();

                        for (int i = 0 ; i < IM_ARRAYSIZE(equipes); i++)
                           if (ImGui::Selectable(equipes[i]))
                                selected_team = i;

                        ImGui::EndPopup();
                    }
                }
            }

            /*      Date du suivi        */
            ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
            static char date[32] = "10 novembre 2017";
            char buff[64]; sprintf(buff, "Date : %s###Date", date); // ### operator override ID ignoring the preceeding label

            ImGui::PushID("date");
            ImGui::Button(buff);
            if (ImGui::BeginPopupContextItem("rename context menu"))
            {
                ImGui::Text(ENTER_THE_DATE);
                ImGui::InputText("##editer", date, IM_ARRAYSIZE(date));

                if (ImGui::Button(OK_BUTTON))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }
            ImGui::PopID();

            /*  Match ou championnat  */
            ImGui::SameLine(); ImGui::Text("  ");
            ImGui::SameLine();
            ImGui::RadioButton(PRACTICE_RADIOBUTTON, &match_officiel, 0);
            ImGui::SameLine();
            ImGui::RadioButton(OFFICIAL_MATCH_RADIOBUTTON, &match_officiel, 1);

            /*    Fin champs à saisir     */
            ImGui::NewLine();

            static ImU32 color = IM_COL32(255,0,0,255);
            static ImVector<ImVec2> points;
            static ImVector <const char * > shooters;
            //static ImVector<ImVec2> control_points;
            static ImVector<ImU32> colors;
            static bool adding_line = false;
            static ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
            static ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
            static bool draw_arrows = true;
            static float arrowLength = 8.0f;
            static float arrowWidth = 3.0f;
            static int result_of_shoot = 0;
            static bool show_shooters = false;
            static bool nouveau_tir = false;
            static bool cancel_last_action = false;
            static bool incrementer_shooters = false;
            static bool enable_7m = false;

        ImGui::BeginGroup();  // Group 1
        {
            ImGui::BeginGroup(); // Group 2
            {
                ImGui::SetNextWindowContentWidth(620);

                ImGui::BeginChild("playground", ImVec2(620,603), true);
                ImDrawList * draw_list = ImGui::GetWindowDrawList();


                if (ImGui::Button(DELETE_ALL_BUTTON))
                    ImGui::OpenPopup("Tout effacer ?");

                if (points.Size >= 2)
                {

                    if (ImGui::BeginPopupModal("Tout effacer ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::Text(ALL_SHOOTS_WILL_BE_CANCELED_STRING);

                        if (ImGui::Button(OK_BUTTON, ImVec2(120,0)))
                        {
                            colors.clear();
                            points.clear();
                            nombre_de_buts_pris = 0;
                            nombre_de_buts_pris_sur_7m = 0;
                            nombre_arrets_GB = 0;
                            nombre_arrets_GB_7m = 0;
                            nombre_de_poteaux = 0;
                            nombre_de_penalties = 0;
                            nombre_de_tirs_hors_cadre = 0;
                            pourcentage_arrets = 0.0f;
                            pourcentage_arrets_7m = 0.0f;
                            pourcentage_reussite_7m = 0.0f;
                            pourcentage_reussite = 0.0f;
                            nombre_de_tirs = 0;
                            unTir.clear();
                            unPenalty.clear();
                            shooters.clear();

                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::SameLine();

                        if (ImGui::Button(CANCEL_BUTTON, ImVec2(120,0)))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::SameLine();
                    if (ImGui::Button(UNDO_BUTTON) || cancel_last_action)
                    {
                        colors.pop_back();
                        colors.pop_back();
                        points.pop_back();
                        points.pop_back();

                        if (!cancel_last_action)
                        {
                            shooters.pop_back();
                            shooters.pop_back();
                            nombre_de_tirs--;

                            if (nombre_de_tirs < 0)
                                nombre_de_tirs = 0;

                            if (!unTir.empty())
                            {
                                switch (unTir[unTir.Size-1])
                                {
                                    case BUT: nombre_de_buts_pris--;              break;
                                    case ARRET_GB: nombre_arrets_GB--;            break;
                                    case POTEAU: nombre_de_poteaux--;             break;
                                    case HORS_CADRE: nombre_de_tirs_hors_cadre--; break;
                                    default: break;
                                }
                            }

                            if (!unPenalty.empty())
                            {
                                switch(unPenalty[unPenalty.Size-1])
                                {
                                     case _7M:
                                         nombre_de_penalties--;
                                         switch(unTir[unTir.Size-1])
                                         {
                                             case ARRET_GB: nombre_arrets_GB_7m--; break;
                                             case BUT: nombre_de_buts_pris_sur_7m--; break;
                                             default: break;
                                         }
                                     break;
                                     default: break;
                                }
                            }

                            if (!unTir.empty()) unTir.pop_back();
                            if (!unPenalty.empty()) unPenalty.pop_back();
                            if (nombre_de_buts_pris < 0) nombre_de_buts_pris = 0;
                            if (nombre_arrets_GB < 0) nombre_arrets_GB = 0;
                            if (nombre_arrets_GB_7m < 0) nombre_arrets_GB_7m = 0;
                            if (nombre_de_poteaux < 0) nombre_de_poteaux = 0;
                            if (nombre_de_tirs_hors_cadre < 0) nombre_de_tirs_hors_cadre = 0;
                            if (nombre_de_penalties < 0) nombre_de_penalties = 0;
                            if (nombre_de_buts_pris_sur_7m < 0) nombre_de_buts_pris_sur_7m = 0;

                            if (nombre_de_tirs > 0)
                            {
                                pourcentage_arrets = (float)(nombre_arrets_GB / (float)nombre_de_tirs);
                                pourcentage_reussite = (float)(nombre_arrets_GB + nombre_de_poteaux + nombre_de_tirs_hors_cadre)/(float)nombre_de_tirs;

                                if (nombre_de_penalties > 0)
                                {
                                    pourcentage_reussite_7m = ((float)nombre_de_penalties - (float)nombre_de_buts_pris_sur_7m)/(float) nombre_de_penalties;
                                    pourcentage_arrets_7m = ((float)nombre_arrets_GB_7m /(float) nombre_de_penalties);
                                }
                                else
                                {
                                    pourcentage_reussite_7m = 0.0f;
                                    pourcentage_arrets_7m = 0.0f;
                                }
                            }
                        }
                        nouveau_tir = false;
                        enable_7m = false;
                        cancel_last_action = false;
                    }
                }

                if (match_officiel == 1)
                {
                    /*       Période de jeu     */
                    ImGui::SameLine(); ImGui::Text("                                       "); ImGui::SameLine();
                    static int selected_halftime = 0;
                    const char * mitemps[] = { FIRST_HALF_TIME, SECOND_HALF_TIME };

                    if (ImGui::Button(GAME_PERIOD_BUTTON))
                        ImGui::OpenPopup("half_time");

                    ImGui::SameLine();
                    ImGui::TextUnformatted(mitemps[selected_halftime]);

                    if (ImGui::BeginPopup("half_time"))
                    {
                        for (int i = 0 ; i < IM_ARRAYSIZE(mitemps); i++)
                           if (ImGui::Selectable(mitemps[i]))
                            selected_halftime = i;
                        ImGui::EndPopup();
                    }
                }

                ImGui::Text(RIGHT_CLICK_TIP_FOR_ARROWS_DRAWING);
                backgroundTextureId4 = glConvertMatToTexture(playgroundImage);
                ImGui::Image(reinterpret_cast<void * >(backgroundTextureId4), ImVec2(600, 500), ImVec2(0,0), ImVec2(1,1),
                             ImColor(255,255,255,255), ImColor(255,255,255,255));

                ImGui::EndChild();

                ImGui::BeginChild("playground");

                bool adding_preview = false;

                if (canvas_pos.x > canvas_size.x) canvas_pos.x = canvas_size.x;
                if (canvas_pos.y > canvas_size.y) canvas_pos.y = canvas_size.y;
                if (canvas_pos.x < 0) canvas_pos.x = 0.0f;
                if (canvas_pos.y < 0) canvas_pos.y = 0.0f;

                ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);

                if (adding_line)
                {
                    adding_preview = true;
                    colors.push_back(color);
                    points.push_back(mouse_pos_in_canvas);

                    if (!ImGui::GetIO().MouseDown[0])
                    {
                        // compute control_points here ?
                        adding_line = adding_preview = false;
                        // FIXME : add a condition (e.g.  distance P1P4 < min_value)
                        nouveau_tir = true;
                    }
                }

                // TODO : un cas pour chaque objet
                if (ImGui::IsItemHovered())
                {
                    if (!adding_line && ImGui::IsMouseClicked(0))
                    {
                        colors.push_back(color);
                        points.push_back(mouse_pos_in_canvas);
                        adding_line = true;
#ifdef DEBUG
                        std::cout << "result_of_shoot = " << result_of_shoot << std::endl;
                        if (!unTir.empty())
                        {
                            std::cout << "unTir.Size-1 = " << unTir.Size-1 << std::endl;
                            std::cout << "unTir[" << unTir.Size-1 << "] = " << unTir[unTir.Size-1] << std::endl;
                        }
#endif
                    }

                    if (ImGui::IsMouseClicked(1))
                        ImGui::OpenPopupOnItemClick(ARROWS_POPUP_MENU);

                    if (nouveau_tir)
                        ImGui::OpenPopup("##saisie");
                }

                if (ImGui::BeginPopupContextItem(ARROWS_POPUP_MENU))
                {
                    ImGui::PushItemWidth(110);
                    ImGui::Text(ARROWS_PARAMETERS);
                    ImGui::Separator();
                    ImGui::Checkbox(PROVIDE_ADDITIONAL_INFORMATION_ABOUT_THE_SHOOT, &saisie_complete);

                    ImGui::Separator();
                    ImGui::Checkbox(DISPLAY_SHOOTERS_NUMBERS, &show_shooters);
                    ImGui::Checkbox(DRAW_THE_ARROWS, &draw_arrows);

                    if (draw_arrows)
                    {
                        ImGui::SliderFloat(ARROWS_LENGTH, &arrowLength, 5.0f, 20.0f, "%.0f");
                        ImGui::SliderFloat(ARROWS_WIDTH, &arrowWidth, 2.0f, 10.0f, "%.0f" );
                    }
                    ImGui::PopItemWidth();
                    ImGui::EndPopup();
                }

                static char number[3];
                static char numero[3];

                if (ImGui::BeginPopupModal("##saisie", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    // Numéro du joueur ayant tiré
                    if (show_shooters)
                    {
                        ImGui::InputText(ENTER_A_NUMBER_BETWEEN_1_AND_99, number, 3, ImGuiInputTextFlags_CharsDecimal);
                    }

                    if (saisie_complete)
                    {
                        ImGui::NewLine();
                        ImGui::Text(SHOOT_CONDITIONS);

                        static int duel_sur_CA = 0;
                        static int entre_les_jambes = 0;
                        static int tir_masque = 0;
                        static int tir_a_rebond = 0;
                        static int tir_a_la_hanche = 0;
                        static int tir_dans_la_course = 0;
                        static int lob = 0;
                        static int tir_avec_contre = 0;
                        static int tir_stresse = 0;

                        // a column of buttons
                        static float a = 0.7f; // 0.9f
                        static float b = 1.0f; // 0.6f
                        static float c = 0.5f; // 1.0f

                        const char * shoots[] = { SHOOT_AT_6M, WING_SHOOT, FAR_SHOOT, PIVOT_SHOOT, BACK_SHOOT, CENTRAL_SHOOT };

                        for (int i = 0 ; i < IM_ARRAYSIZE(shoots) ; i++)
                        {
                            ImGui::PushID(i);
                            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, a, a));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, b, b));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, c, c));
                            ImGui::Button(shoots[i]);
                            ImGui::PopStyleColor(3);
                            ImGui::PopID();
                        }

                        ImGui::PushItemWidth(100);
                        ImGui::InputInt(COUNTER_ATTACK_FIGHT, &duel_sur_CA, 1, 0, 32);
                        ImGui::InputInt(BETWEEN_BEANS_SHOOT, &entre_les_jambes, 1, 0, 32);
                        ImGui::InputInt(MASKED_SHOOT, &tir_masque, 1, 0, 32);
                        ImGui::InputInt(BOUNDING_SHOOT, &tir_a_rebond, 1, 0, 32);
                        ImGui::InputInt(SIDE_SHOOT, &tir_a_la_hanche, 1, 0, 32);
                        ImGui::InputInt(IN_THE_RUN_SHOOT, &tir_dans_la_course, 1, 0, 32);
                        ImGui::InputInt(OVER_HEAD_SHOOT, &lob, 1, 0, 32);
                        ImGui::InputInt(COUNTERED_SHOOT, &tir_avec_contre, 1, 0, 32);
                        ImGui::InputInt(STRESSED_SHOOT, &tir_stresse, 1, 0, 32);
                        ImGui::PopItemWidth();

                        if (duel_sur_CA < 0)
                            duel_sur_CA = 0;
                        if (entre_les_jambes < 0)
                            entre_les_jambes = 0;
                        if (tir_masque < 0)
                            tir_masque = 0;
                        if (tir_a_rebond < 0)
                            tir_a_rebond = 0;
                        if (tir_a_la_hanche < 0)
                            tir_a_la_hanche = 0;
                        if (tir_dans_la_course < 0)
                            tir_dans_la_course = 0;
                        if (lob < 0)
                            lob = 0;
                        if (tir_avec_contre < 0)
                            tir_avec_contre = 0;
                        if (tir_stresse < 0)
                            tir_stresse = 0;
                    }

                    if (ImGui::Button("Valider", ImVec2(120,0)) || (!show_shooters))
                    {
                        //static float pourcentage_tirs_de_loin = 0.0f;
                        //static float pourcentage_tirs_a_laile = 0.0f;
                        //static float pourcentage_tirs_a_6m = 0.0f;
                        nombre_de_tirs++;
                        unTir.push_back(result_of_shoot);
#ifdef DEBUG
                        for (int j = 0 ; j < shooters.Size ; j++)
                        {
                            std::cout << "shooters.Size > 0 et shooters["<< j<<"] contient :" << shooters[j] << std::endl;
                        }
#endif
                        switch (result_of_shoot)
                        {
                            case BUT: nombre_de_buts_pris++; break;
                            case ARRET_GB: nombre_arrets_GB++; break;
                            case POTEAU: nombre_de_poteaux++; break;
                            case HORS_CADRE: nombre_de_tirs_hors_cadre++; break;
                            default: break;
                        }

                        if (enable_7m == true)
                        {
                            nombre_de_penalties++;
                            unPenalty.push_back(_7M);

                            if (result_of_shoot == BUT)
                                nombre_de_buts_pris_sur_7m++;

                            if (result_of_shoot == ARRET_GB)
                                nombre_arrets_GB_7m++;
                        }
                        else
                            unPenalty.push_back(result_of_shoot);

                        pourcentage_arrets = (float)(nombre_arrets_GB / (float)nombre_de_tirs);
                        pourcentage_reussite = (float)(nombre_arrets_GB + nombre_de_poteaux + nombre_de_tirs_hors_cadre)/(float)nombre_de_tirs;

                        // FIXME : factorize ...
                        if (nombre_de_penalties > 0)
                        {
                            pourcentage_reussite_7m = ((float)nombre_de_penalties - (float)nombre_de_buts_pris_sur_7m)/(float) nombre_de_penalties;
                            pourcentage_arrets_7m = ((float)nombre_arrets_GB_7m /(float) nombre_de_penalties);
                        }
                        else
                        {
                            pourcentage_reussite_7m = 0.0f;
                            pourcentage_arrets_7m = 0.0f;
                        }

                        incrementer_shooters = true;
                        enable_7m = false;
                        nouveau_tir = false;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();

                    if (ImGui::Button(CANCEL_BUTTON, ImVec2(120,0)))
                    {
                        cancel_last_action = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();

                } // BeginPopupModal()

                if (incrementer_shooters)
                {
                    //FIXME numero = new char [number.length()+1];
                    //FIXME std::strcpy (numero[0], number.c_str());

                    //numero[0] = number[0];
                    //numero[1] = number[1];
                    //numero[2] = '\0';
                    shooters.push_back(numero);
                    shooters.push_back("");

#ifdef DEBUG
                    for (int k = 0 ; k < shooters.Size ; k++)
                    {
                        std::cout << "shooters[" << k << "] = " << shooters[k] << std::endl;
                    }
#endif
                    // let's reset the shooter number every time
                    number[0] = '\0';
                    incrementer_shooters = false;
                }

                // clip lines and objects within the canvas (if we resize it, etc.)
                draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x+canvas_size.x, canvas_pos.y+canvas_size.y));
                ImVec4 clip_rect(canvas_pos.x, canvas_pos.y, canvas_pos.x+canvas_size.x, canvas_pos.y+canvas_size.y);
                static float P1P4 = 0.1;

                for (int i = 0; i < points.Size-1; i += 2)
                {
                    // ARROW
                    P1P4 = sqrtf( (points[i+1].x -points[i].x)*(points[i+1].x -points[i].x) + (points[i+1].y -points[i].y)*(points[i+1].y - points[i].y) );

                    //if (draw_arrows == true)
                    if (P1P4 > 1.5f * arrowLength)
                    {
                        //if (P1P4 > 1.5f * arrowLength)
                        if (draw_arrows == true)
                        {
                            ImVec2 pointC(  points[i+1].x - (arrowLength * (points[i+1].x - points[i].x))/P1P4,
                                            points[i+1].y - (arrowLength * (points[i+1].y - points[i].y))/P1P4);

                            ImVec2 pointD(  pointC.x + (arrowWidth*(points[i+1].y -points[i].y))/P1P4,
                                            pointC.y - (arrowWidth*(points[i+1].x -points[i].x))/P1P4);

                            ImVec2 pointE(  pointC.x - (arrowWidth*(points[i+1].y -points[i].y))/P1P4,
                                            pointC.y + (arrowWidth*(points[i+1].x -points[i].x))/P1P4);

                            float offsetX = canvas_pos.x;
                            float offsetY = canvas_pos.y;
                            float thickness = 1.5f;

                            draw_list->PathClear();
                            draw_list->PathLineTo(ImVec2(pointD.x + offsetX, pointD.y + offsetY));
                            draw_list->PathLineTo(ImVec2(points[i+1].x + offsetX, points[i+1].y + offsetY));
                            draw_list->PathLineTo(ImVec2(pointE.x + offsetX, pointE.y + offsetY));
                            draw_list->PathStroke(colors[i], false, thickness);

                            if ((show_shooters) && (i < shooters.Size))
                                draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2(canvas_pos.x+points[i].x - 2.0f ,canvas_pos.y+points[i].y+5.0f), ImColor(0,0,0,255), shooters[i], NULL, 0.0f, &clip_rect);

                        }
                    }
                    draw_list->AddBezierCurve(   /*P1 start*/
                                                 ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y),
                                                 //control_points[i],
                                                 //control_points[i+1],
                                                 /*P2 CP1*/
                                                 ImVec2(canvas_pos.x + points[i].x + (points[i+1].x - points[i].x)*0.25f, canvas_pos.y + points[i].y + (points[i+1].y - points[i].y)*0.25f),
                                                 /*P3 CP2*/
                                                 ImVec2(canvas_pos.x + points[i].x + (points[i+1].x - points[i].x)*0.75f, canvas_pos.y + points[i].y + (points[i+1].y - points[i].y)*0.75f),
                                                 /*P4 :end*/
                                                 ImVec2(canvas_pos.x + points[i+1].x, canvas_pos.y + points[i+1].y),
                                                 /* drawing parameters*/
                                                 colors[i], 2.0f, 64);

                }
                draw_list->PopClipRect();

                if (adding_preview)
                {
                    colors.pop_back();
                    points.pop_back();
                }

                ImGui::EndChild();
            }
            ImGui::EndGroup(); // end group 2

            ImGui::BeginGroup(); // group 3
            {
                /* Choix type de tir */
                // LATER static bool feminines = false;

                ImGui::Text(SHOOT_RESULT);
                ImGui::SameLine();

                if (ImGui::RadioButton(GOAL_RADIOBUTTON , &result_of_shoot, 0)) color = IM_COL32(255,0,0,255) /* red */;
                    ImGui::SameLine();

                if (ImGui::RadioButton(GK_STOP_RADIOBUTTON, &result_of_shoot, 1)) color = IM_COL32(0,255,0,255) /* green */;
                    ImGui::SameLine();

                if (ImGui::RadioButton(BORDER_SHOOT_RADIOBUTTON, &result_of_shoot, 2)) color = IM_COL32(0,0,0,255) /* black */;
                    ImGui::SameLine();

                if (ImGui::RadioButton(OUT_SHOOT_RADIOBUTTON, &result_of_shoot, 3)) color = IM_COL32(0,0,255,255) /* blue */;
                    ImGui::SameLine();

                //static float a = 0.7f; // 0.9f
                static float b = 1.0f; // 0.6f
                static float c = 0.5f; // 1.0f
                static int i = 3;

                if (enable_7m == true)
                {
                    ImGui::PushID(_7M_BUTTON);
                    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, b, b));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, b, b));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, c, c));
                    ImGui::Button(_7M_BUTTON);
                    if (ImGui::IsItemClicked(0))
                    {
                        enable_7m = !enable_7m;
                    }
                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                }
                else if (ImGui::Button(_7M_BUTTON))
                    enable_7m = true;

                ImGui::Checkbox(DRAW_THE_ARROWS, &draw_arrows); ImGui::SameLine();
                ImGui::Checkbox(DISPLAY_THE_SHOOTERS_NUMBER, &show_shooters);
//                ImGui::NewLine();
            }
            ImGui::EndGroup(); // end group 3
        }
        ImGui::EndGroup(); // end group 1

            ImGui::SameLine();
            ImGui::BeginGroup();  // group 4
            {
                ImGui::BeginChild("Stats GB", ImVec2(350.0f,350.0f ), true);
                {
                    ImGui::Text(GK_FIGURES_STRING_01);
                    ImGui::NewLine();
                    ImGui::Text(GK_FIGURES_STRING_02, nombre_de_tirs);
                    ImGui::Text(GK_FIGURES_STRING_03, nombre_de_penalties);
                    ImGui::Text(GK_FIGURES_STRING_04, nombre_de_tirs - nombre_de_tirs_hors_cadre);
                    ImGui::Text(GK_FIGURES_STRING_05, nombre_de_buts_pris);
                    ImGui::Text(GK_FIGURES_STRING_06, nombre_de_buts_pris_sur_7m);
                    ImGui::Text(GK_FIGURES_STRING_07, nombre_arrets_GB);
                    ImGui::Text(GK_FIGURES_STRING_08, nombre_arrets_GB_7m);
                    ImGui::Text(GK_FIGURES_STRING_09, 100*pourcentage_arrets_7m);
                    ImGui::Text(GK_FIGURES_STRING_10, 100*pourcentage_reussite_7m);
                    ImGui::NewLine();
                    ImGui::Text(GK_FIGURES_STRING_11, 100*pourcentage_arrets);
                    ImGui::Text(GK_FIGURES_STRING_11, 100*pourcentage_reussite);
                }
                ImGui::EndChild();

//                ImGui::NewLine();

                ImGui::BeginChild(EFFICIENCY_AREA, ImVec2(350.0f,300.0f ), true);
                {
                    ImGui::Text(GK_EFFICIENCY_AREA);
                }
                ImGui::EndChild();

            }
            ImGui::EndGroup();  // end group 4

                ImGui::SameLine();

            ImGui::BeginGroup();  // group 5
            {
                ImGui::BeginChild(IMPACTS_LOCATION, ImVec2(350.0f,325.0f), true);
                {
                    ImGui::Text(IMPACTS_LOCATION);
                }
                ImGui::EndChild();

//                ImGui::NewLine();

                ImGui::BeginChild(GOAL_FIGURES, ImVec2(350.0f,325.0f ), true);
                {
                    ImGui::Text(GOALS_FIGURES);
                }
                ImGui::EndChild();
            }
            ImGui::EndGroup();  // end group 5

//            ImGui::Separator();
            ImGui::NewLine();
            ImGui::Text(CHARACTERISTICS_OF_THE_SHOOTS_TEXT);
            // ---------------------------------//
            ImGui::EndTabItem();
        }


        if (ImGui::BeginTabItem(ICON_FA_WRENCH PREFERENCES_TAB_ITEM))
        {
            int selected_theme = current_theme; // default theme
            b_spot_active = false;

            current_tab = PREFERENCES_TAB;

            ImGui::Text(PREFERENCES_STRING_01);
            ImGui::Text(PREFERENCES_STRING_02);
            ImGui::SliderFloat(PREFERENCES_STRING_03, &value2,0,1.0f);
            ImGui::NewLine();
            ImGui::Text(PREFERENCES_STRING_04);
            ImGui::RadioButton(PREFERENCES_STRING_05, &selected_theme, LIGHT_GREEN_THEME);
            ImGui::RadioButton(PREFERENCES_STRING_06,              &selected_theme, DARK_THEME);
            ImGui::RadioButton(PREFERENCES_STRING_07,           &selected_theme, CLASSIC_THEME);
            ImGui::RadioButton(PREFERENCES_STRING_08,        &selected_theme, LIGHT_BLUE_THEME);
            ImGui::RadioButton(PREFERENCES_STRING_09,        &selected_theme, WINDOWS_THEME);
 
           if (selected_theme != current_theme)
                set_theme(selected_theme);

            // default modifier is KMOD_ALT
            static int modifier_choice = MOD_ALT_KEY;

            /* Choix de la touche pour les raccourcis claviers */
            ImGui::Text(PREFERED_SHORTCUT_KEY_TEXT); ImGui::SameLine();
            ImGui::RadioButton( ALT_SHORTCUT_TEXT, &modifier_choice, 0); ImGui::SameLine();
            ImGui::RadioButton( SHIFT_SHORTCUT_TEXT, &modifier_choice, 1); ImGui::SameLine();
            //ImGui::RadioButton( " SHIFT ", &modifier_choice, 2);

            switch(modifier_choice)
            {
//                case MOD_CTRL_KEY   : prefered_modifier = KMOD_CTRL; break;
                case MOD_ALT_KEY    : prefered_modifier = KMOD_ALT ; break;
                case MOD_SHIFT_KEY  : prefered_modifier = KMOD_SHIFT ; break;

                default:
                    break;
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_COMPASS HELP_TAB_ITEM))
        {
            current_tab = HELP_TAB;
            b_spot_active = false;

            ImGui::NewLine();
            ImGui::Text(THIS_PAGE_AIMS_TO_BETTER_USE_MINIDART);

            ImGui::BeginGroup();
            {
                backgroundTextureId2 = glConvertMatToTexture(backgroundImage);
                ImGui::Image(reinterpret_cast<void * >(backgroundTextureId2), ImVec2(512, 288), ImVec2(0,0), ImVec2(1,1),
                             ImColor(255,255,255,255), ImColor(255,255,255,255));

                ImGui::SameLine();

                ImGui::Text("          ");
                ImGui::SameLine();

                static float sliderThicknessScale = 0.6f;
                ImGui::PushStyleVar(ImGuiStyleVar_SliderThicknessScale, sliderThicknessScale);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

                static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
                ImGui::PushID("set1");
                ImGui::SameLine();

                for (int i = 0; i < 7; i++)
                {
                    ImGui::PushID(i);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i/7.0f, 0.5f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i/7.0f, 0.9f, 0.9f));
                    ImGui::VSliderFloat("##v", ImVec2(20,256), &values[i], 0.0f, 1.0f, "");

                    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                        ImGui::SetTooltip("%.3f", values[i]);

                    ImGui::PopStyleColor(4);
                    ImGui::PopID();
                    ImGui::SameLine();
                }
                ImGui::PopID();
                ImGui::PushItemWidth(200);
                ImGui::SliderFloat(SLIDER_THIKNESS, &sliderThicknessScale, 0.0f, 1.0f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::PopStyleVar(2);
            }
            ImGui::EndGroup();
////!!
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_FA_GRADUATION_CAP CREDITS_TAB_ITEM))
        {

            current_tab = CREDITS_TAB;
            b_spot_active = false;

            static float w = 440.0f;
            static float h = 260.0f;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

            ImGui::BeginChild("child4", ImVec2(w, h), true);
            ImGui::Text(AUTHOR_ID_COPYRIGHT_YEARS);
            ImGui::Text("   ");
            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::InvisibleButton("vsplitter", ImVec2(8.0f,h)); 
            if (ImGui::IsItemActive())
                w += ImGui::GetIO().MouseDelta.x;
            ImGui::SameLine();
            ImGui::BeginChild("child5", ImVec2(0, h), true);
            ImGui::EndChild();
            ImGui::InvisibleButton("hsplitter", ImVec2(-1,8.0f));

            if (ImGui::IsItemActive())
                h += ImGui::GetIO().MouseDelta.y;

            ImGui::BeginChild("child6", ImVec2(0,0), true);
            ImGui::NewLine();
            ImGui::Text(DISPLAY_NUMBER_ID_CLUB_RESULT_IMPACT_CONDITIONS);

            ImGui::EndChild();
            ImGui::PopStyleVar();

            ImGui::EndTabItem();

        }
        ImGui::EndTabBar();

        }

////!! FIXME TEST
}
        ImGui::End();

        // end GUI::Draw()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Gui::render()
        // add new shaders there ?
        glUseProgram(0);
        ImGui::Render();

        // render the users' stack too
        if (ImGui::GetDrawData() == NULL)
            std::cout << "ImGui::GetDrawData() is null" << std::endl;

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        /* End ImGui rendering*/
        ///////////////////////

        /* DRAW_EVERYTHING */
        //// SDL_GL_SwapWindow(engine.window);
        SDL_GL_SwapWindow(window);

        //// TEST with SDL2 renderer
        ////glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);

        // GUI::cleanup();

        // Gui::cleanupTexIds() to avoid leaks with OpenGL Texture Ids
        if (backgroundTextureId  != 0)
            glDeleteTextures(1, &backgroundTextureId);
        if (backgroundTextureId2 != 0)
            glDeleteTextures(1, &backgroundTextureId2);
        if (backgroundTextureId3 != 0)
            glDeleteTextures(1, &backgroundTextureId3);
        if (backgroundTextureId4 != 0)
            glDeleteTextures(1, &backgroundTextureId4);

        cleanCanvasObjectsImagesTexIds();

        //unneeded if vsync is active
        #ifndef USE_VSYNC
        SDL_Delay(10);
        #endif

        // If frame finished early
        int frameTicks = SDL_GetTicks() - fpsCapStartTicks;
#ifdef DEBUG
        static int counter2 = 0;
        if ((counter2 % 5) == 0)
        {
            std::cout << "frameTicks =  " << frameTicks << std::endl;
            counter2 = 0;
        }
        counter2++;
#endif
        if( frameTicks < SCREEN_TICK_PER_FRAME )
        {
            //Wait remaining time
            SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        }
    }

    // FIXME VideoWriter must be released too, else the last video will not be released (nor available)
    oVideoWriter.release();
    // Timer Dtor
    aTimer.~Timer();
    // CaptureDev::Close()

    if (!captureDev.USBFrame.empty())
        captureDev.USBFrame.release();

    cap2.release();
    aClipReader.Close();
    pClipReader = 0;

    // Gui::Quit()
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    //engine.clean_and_close();
    //engine.~Engine();
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_GL_DeleteContext(glcontext);
    SDL_Quit();

    return 0;
}


static void ShowMenuFile()
{
    // FIXME : broken.
    // send ImGui evens instead ?
    //SDL_Event an_Event;
    //SDL_Event an_Event2;
    ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O"))
    {
    }

    if (ImGui::MenuItem("Fullscreen", "ALT+F"))
    {
    }

    if (ImGui::MenuItem("Checked", NULL, true)) {}

    if (ImGui::MenuItem( "Quit", "Alt+F4" ) )
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent( &event );
    }
}

void update(const SDL_Event * p_event)
{


}

static int video_init(char * filename)
{
    // OPENCV : convert FFMPEG frame into cv::Mat
    // Link : https://stackoverflow.com/questions/29263090/ffmpeg-avframe-to-opencv-mat-conversion
    // int w = pinfo.video.output.width;
    // int h = pinfo.video.output.height;
    fprintf(stdout, "video_init done\n");
    fprintf(stdout, "calling main2(filename), filename in video_init contains : %s\n", filename);

    if (main2(filename) == 0)
        b_video_running = true;

    fprintf(stdout, "b_video_running = %d (currently in %s) \n", b_video_running, __FUNCTION__);

    return 0;
}




static void ToggleButton(const char* str_id, bool* v)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    ImGuiContext& g = *GImGui;
    float ANIM_SPEED = 0.08f;
    if (g.LastActiveId == g.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
    {
        float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}


