/*
 * Fichier d'en tête ClipReader.hpp pour le projet miniDart
 * Auteur : Eric Bachard  / mercredi 1er novembre 2017, 8:48:17 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#ifndef __CLIP_READER_H
#define __CLIP_READER_H

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <cstdio>
#include <vector>

#ifndef MAX_PATH
#define MAX_PATH 512
#include <limits.h>
#endif


#include "imgui.h"

//#define DEFAULT_FILE_SIZE  14
//char defaultFileName[DEFAULT_FILE_SIZE] = "MyVideo1.avi";

class ClipReader
{
    public:
        // Ctor
        ClipReader();
        // Dtor
        ~ClipReader();
        bool initClip();
        cv::VideoCapture maClipReader;
        void setPos();
        unsigned int getPos();
        int getWidth();
        int getHeight();
        int getMaxFrame();
        bool setMaxFrame(int);
        int getFrameCount();
        void setFile(char *);
        bool  playAtGivenFps(int, unsigned int, unsigned int);
        //int prepare();
        void resetFrames();
        void Close();
        bool newClipLoaded;
        std::vector<cv::Mat > frames;
        char * getCurrentClipFile();
        char * getOldClipFile();
    private:
        unsigned int pos;
        char * currentClipFile;
        char * oldClipFile;
        int frameCount;
        int frameWidth;
        int frameHeight;
        int maxFrame;
};


#endif  /*__CLIP_READER_H */
