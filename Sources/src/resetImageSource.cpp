/*
 * File resetImageSource.cpp, belongs to miniDart project
 * Copyright Eric Bachard  / mercredi 26 décembre 2018, 14:20:13 (UTC+0200)
 * This document is under GPL v2 license
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "application.hpp"
#include "resetImageSource.hpp"
#include "capturedev.hpp"

int resetImageSource(cv::VideoCapture * currentCaptureDev, int source , const char * url, int aDeviceNumber, int aWidth, int aHeight, double fps)
{
    // FIXME : we need to verify the new dapture dev IS available (to avoid crashing) but there is no easy way to check 
    // Last : if no webcam, it will crash on Linux,
    //        no problem on Windows (default background replaces the missing frames)
    currentCaptureDev->release();
    bool isFullVideo = (source == USB_WEBCAM) ? false : true ;

    fprintf (stdout, "isFullVideo vaut %d \n", isFullVideo);

    switch (source)
    {
        case CELL_PHONE:
        case FULL_VIDEO:
        case IP_WEBCAM:
          break;

        case USB_WEBCAM:
#ifdef NATIVE_BUILD

        currentCaptureDev->open(aDeviceNumber);
//  other working choice, with recent OpenCV >= 3.4.4 :
//        currentCaptureDev->open(aDeviceNumber + cv::CAP_V4L2);
#else
        currentCaptureDev->open(aDeviceNumber + cv::CAP_DSHOW);
#endif
          break;

        default:
          break;
    }

    if (!currentCaptureDev->isOpened())
    {
        b_lostWebcam = true;
        return -1;
    }
    else
        b_lostWebcam = false;

    if (!isFullVideo)
    {
#ifndef NATIVE_BUILD
        // works *well* on Windows ONLY (not on Linux ^^ )
        // on Linux, returns: VIDEOIO ERROR: V4L: Property <unknown property string>(16) not supported by device or something close
        currentCaptureDev->set(cv::CAP_PROP_FOURCC, CV_FOURCC('M','J','P','G'));
#endif
        currentCaptureDev->set(cv::CAP_PROP_FRAME_WIDTH, aWidth);
        currentCaptureDev->set(cv::CAP_PROP_FRAME_HEIGHT,aHeight);
#ifdef NATIVE_BUILD
        // *WHAT follows* works only on Linux. Don't ask me why
        currentCaptureDev->set(cv::CAP_PROP_FPS, fps);
#endif
        currentCaptureDev->set(CV_CAP_PROP_ZOOM, 100.0f);
    }
    return 0;
}

