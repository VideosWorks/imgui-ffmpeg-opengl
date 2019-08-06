/*
 * File audiodevices_helper.cpp belongs to miniDart project
 * Copyright : Eric Bachard  / 2017 November 1st, 11:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */


#include "clipreader.hpp"

#define DEFAULT_FILENAME_SIZE  16
static char defaultFileName[DEFAULT_FILENAME_SIZE] = "./MyVideo01.avi";

ClipReader::ClipReader()
{
    oldClipFile = defaultFileName;
    currentClipFile = nullptr;
}

ClipReader::~ClipReader()
{
}

bool ClipReader::initClip()
{
    cv::VideoCapture maClipReader(currentClipFile, cv::CAP_FFMPEG);

    frameWidth  = maClipReader.get(cv::CAP_PROP_FRAME_WIDTH);
    frameHeight = maClipReader.get(cv::CAP_PROP_FRAME_HEIGHT);
    frameCount  = maClipReader.get(cv::CAP_PROP_FRAME_COUNT);
    int firstFrameNumber = 1; // default, but could change bcause of some weird codecs
    this->maxFrame = 1;
    this->frames.empty();
    int pos = firstFrameNumber;
    bool bStop = false;
    cv::Mat f;

    while (bStop == false)
    {
        pos = maClipReader.get(CV_CAP_PROP_POS_FRAMES);
        maClipReader >> f;

        if (!f.empty())
        {
            //myFrames[pos] = f;
            this->frames.push_back(f);
            f.release();
        }
        else
            bStop = true;
    }
    f.release();

#define RELEASE_VERSION
#ifdef RELEASE_VERSION
    if (pos > 500)
    {
        // limit to the first 500 images, to avoid machines crashes and users not happy
        this->setMaxFrame(500);
    }
    else
#endif
        this->setMaxFrame(pos);

    std::cout << "Opening a clip : "  << std::endl;
    std::cout << "pos =  " << pos << std::endl;
    std::cout << "maxFrame =  " << maxFrame << std::endl;

    return true;
}

void ClipReader::setFile( char * filename)
{
    this->oldClipFile = this->currentClipFile;
    this->currentClipFile = filename;
    this->resetFrames();
#ifdef DEBUG
    std::cout << "Vidéo chargée : " << currentClipFile << std::endl;
#endif
}

bool ClipReader::playAtGivenFps(int fps, unsigned int current, unsigned int old)
{
    bool toReturn = false;

    unsigned int delta_time = (unsigned int)(1000.0f/fps);
    unsigned int loop_time = current - old;

    if (delta_time <= loop_time)
        toReturn = true;

    return toReturn;
}


void ClipReader::resetFrames()
{
    while (!frames.empty())
    {
        this->frames.pop_back();
    }
}


void ClipReader::Close()
{
    this->oldClipFile = this->currentClipFile;
    this->currentClipFile = nullptr;
    this->resetFrames();
    this->maClipReader.release();
}

int ClipReader::getWidth()
{
    return this->frameWidth;
}

int ClipReader::getHeight()
{
    return this->frameHeight;
}

int ClipReader::getMaxFrame()
{
    return this->maxFrame;
}

bool ClipReader::setMaxFrame(int aFrameNumber)
{
    this->maxFrame = aFrameNumber;
    return true;
}

char * ClipReader::getCurrentClipFile()
{
    return this->currentClipFile;
}

char * ClipReader::getOldClipFile()
{
    return this->oldClipFile;
}
