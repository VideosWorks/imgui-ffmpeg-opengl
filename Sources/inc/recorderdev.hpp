/*
 * Fichier d'en tête recorderdev.h pour le projet miniDart
 * Auteur : Eric Bachard  / lundi 3 octobre 2016, 14:35:03 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __RECORDERDEV__HPP
#define __RECORDERDEV__HPP

#include <SDL2/SDL.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <cstdlib>
#include <list>
#include <queue>

#include "common.hpp"
#include "modes.h"

/* class   */

class RecorderDev : public IODev
{
  public:
    CaptureDev(SDL_Renderer * renderer);
    virtual ~CaptureDev();
    eMODE       getCurrentMode();
    void        setCurrentMode(eMODE);
    bool        getIsDisplayRECActive();
    void        setDisplayRECActive(bool);
    bool        getIsNewRecordingStarted();
    void        setNewRecordingStarted(bool);

    int         Draw();
    void        Update();
    void        Render();
    void        Close();
    bool        bRecorderInit();
    SDL_Texture * CreateTexture(SDL_Renderer *);

  private:
    cv::Mat     maFrame;
    cv::Mat     maFrame2;
    SDL_Texture  * maTexture;
    cv::VideoWriter  maVideoWriter;

    int         mdWidth;
    int         mdHeight;
    cv::Size    mdFrameSize;
    eMODE       maCurrentMode;
    bool        mbIsDisplayRECActive;
    bool        mbIsNewRecordingStarted;
    bool        mbIsRecording;
    bool        mbFirstRun;
};

#endif /* __RECORDERDEV__HPP */

