/*
 * File capturedev.cpp belongs to miniDart project
 * Copyright : Eric Bachard  / 2018 May 6th, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <SDL2/SDL.h>
#include "capturedev.hpp"
#include "application.hpp"

#ifndef NATIVE_BUILD
    #ifndef HAVE_VIDEOINPUT
    #define HAVE_VIDEOINPUT
    #endif
    #ifndef HAVE_DSHOW
    #define HAVE_DSHOW
    #endif
#endif

CaptureDev::CaptureDev()
{
    mb_opened = false;
    md_delay = 1;
    //maDevNumber = DEFAULT_DEVICE_NUMBER;
}

CaptureDev::~CaptureDev()
{
    Close();
}

//bool CaptureDev::Init(int aDevNumber)
bool CaptureDev::Init(int aDevice)
{
    if (maVideoCap.isOpened())
        maVideoCap.release();

    mb_opened = false;

    maDevNumber = aDevice;

#ifdef NATIVE_BUILD
    // Linux
    // FIXME : propose a list of active devices
    // + FIXME : add other possibilities, e.g. using GSTREAMER (cv::CAP_GSTREAMER)
//    maVideoCap.open(maDevNumber + cv::CAP_GSTREAMER);
    maVideoCap.open(maDevNumber + cv::CAP_V4L2);
#else
    // Windows
    // http://docs.opencv.org/trunk/d0/da7/videoio_overview.html
    maVideoCap.open(maDevNumber + cv::CAP_DSHOW);
#endif /* NATIVE_BUILD */

    if (!maVideoCap.isOpened() || !bFirstFrameSuccess())
        std::cerr << "***Could not initialize capturing...***" << std::endl;

#if defined(_WIN32) || defined (WIN32)
    // works only on Windows
    setFourCC(CV_FOURCC('M','J','P','G'));
#endif

    initializeFrameSize();

#ifdef NATIVE_BUILD
    setCaptureFPS(LINUX_DEFAULT_FRAMERATE);
#endif

    mfCaptureFPS = getCaptureFPS();

    // FIXME : this has nothing to do there ...
    // better put that in the VideoRecorder
    //    mdStackSize = delay*mfCaptureFPS;

    // keep a trace of the previous delay
    //    short int delay_ref = delay;

    // in case of failure during initialization, we return false
    mb_opened = maVideoCap.isOpened();

    return mb_opened;
}

void CaptureDev::Close()
{
    maVideoCap.release();
}

bool CaptureDev::bFirstFrameSuccess()
{
    bool toReturn = true;
    cv::Mat firstFrame;
    maVideoCap.retrieve(firstFrame);

    if (!firstFrame.empty())
    {
        mb_opened = maVideoCap.read(firstFrame); // read a new frame from video
        toReturn = mb_opened;
    }
    else
        toReturn = false;

    // avoid leaking
    firstFrame.release();
    return toReturn;
}


void CaptureDev::getFrame()
{

   bool bSuccess3 = maVideoCap.read(this->USBFrame);

   if (!bSuccess3)
       std::cerr << "Pb with cap2.getFrame()" << std::endl;
}

double CaptureDev::getCaptureFPS(void)
{
    return (maVideoCap.get(cv::CAP_PROP_FPS));
}

void CaptureDev::setDevNumber(int aDev)
{
  maDevNumber = aDev;
}

int CaptureDev::getDevNumber(void)
{
  return maDevNumber;
}

void CaptureDev::set_delay(int delay)
{
  md_delay = delay;
}

int CaptureDev::get_delay(void)
{
  return md_delay;
}


void CaptureDev::setFourCC(unsigned int FourCC)
{
    //maVideoCap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
    maVideoCap.set(CV_CAP_PROP_FOURCC, FourCC);
}

void CaptureDev::setCaptureFPS(int aValue)
{
    // depends on the webcam, but breaks everything on Windows
    // Linux : 24 fps is a good value for i3 or i5 + 6GB of RAM 
    //         30 fps for i7 + 8GB RAM or more
    // tested OK with C920, C922, lifecam 3000
    maVideoCap.set(cv::CAP_PROP_FPS, aValue);
}

void CaptureDev::initializeFrameSize(void)
{
    maVideoCap.set(cv::CAP_PROP_FRAME_WIDTH, CAPTURED_FRAME_WIDTH);
    maVideoCap.set(cv::CAP_PROP_FRAME_HEIGHT, CAPTURED_FRAME_HEIGHT);

    //namedWindow("MyVideo", CV_WINDOW_OPENGL);  (thread issue)
    mfWidth  = maVideoCap.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    mfHeight = maVideoCap.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    //set framesize for use with videoWriter
    if (mfWidth * mfHeight < 1.0 )
    {
        mfWidth  = CAPTURED_FRAME_WIDTH;
        mfHeight = CAPTURED_FRAME_HEIGHT;
    }

    maFrameSize = cv::Size(static_cast<int>(mfWidth), static_cast<int>(mfHeight));
}

void CaptureDev::setFrameSize(double aWidth, double aHeight)
{
    maVideoCap.set(cv::CAP_PROP_FRAME_WIDTH, aWidth);
    maVideoCap.set(cv::CAP_PROP_FRAME_HEIGHT, aHeight);

    // check whether it works
    mfWidth = maVideoCap.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    mfHeight = maVideoCap.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    // just in case something wrong occured
    if (mfWidth * mfHeight == 0 )
    {
        mfWidth  = CAPTURED_FRAME_WIDTH;
        mfHeight = CAPTURED_FRAME_HEIGHT;
    }
    maFrameSize = cv::Size(static_cast<int>(mfWidth), static_cast<int>(mfHeight));
}

double CaptureDev::getWidth(void)
{
    return mfWidth;
}

double CaptureDev::getHeight(void)
{
    return mfHeight;
}


cv::Size CaptureDev::getFrameSize(void)
{
    return maFrameSize;
}

bool CaptureDev::IsCapturePaused(void)
{
    return mb_paused;
}

bool CaptureDev::IsCaptureOpened(void)
{
    return mb_opened;
}


bool CaptureDev::canReadFrame(int fps, unsigned int current, unsigned int old)
{
    bool toReturn = false;

    unsigned int delta_time = (unsigned int)(1000.0f/fps);
    unsigned int loop_time = current - old;

    std::cerr << " current_time : " << current << std::endl;
    std::cerr << " old_time : " << old << std::endl;
    std::cerr << " delta_time : " << delta_time << std::endl;
    std::cerr << " loop_time : " << loop_time << std::endl;

    if (delta_time <= loop_time)
        toReturn = true;

    return toReturn;
}
