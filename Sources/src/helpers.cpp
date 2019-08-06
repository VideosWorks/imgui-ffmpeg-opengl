/*
 * Fichier d'en tête helpers.h pour le projet miniDart
 * Auteur : Eric Bachard  / mardi 20 septembre 2016, 23:42:21 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <opencv2/opencv.hpp>
#include <SDL2/SDL_surface.h>

#include "helpers.h"
#include "audiodevices.hpp"

//using namespace cv;
using namespace std;

static GLuint gltexture;

// helpers, tools
void fill_SDL_Texture(SDL_Texture * texture, cv::Mat const &mat)
{
    IplImage img2 = (IplImage)mat;
    IplImage *img = &img2;
    unsigned char * texture_data = NULL;
    int texture_pitch = 0;

    SDL_LockTexture(texture, 0, (void **)&texture_data, &texture_pitch);
    memcpy(texture_data, (void *)img->imageData, img->width * img->height * img->nChannels);
    SDL_UnlockTexture(texture);
}

// helpers, tools
SDL_Surface* convert_to_SDL_Surface(const cv::Mat& frame)
{
    IplImage img2 = (IplImage)frame;
    IplImage *img = &img2;

    return SDL_CreateRGBSurfaceFrom((void*)img->imageData, img->width, img->height, 
        img->depth*img->nChannels, img->widthStep, 0xff0000, 0x00ff00, 0x0000ff, 0);
}

// helpers, tools
SDL_Texture* convert_MatToSDL_Texture(const cv::Mat &matImg, SDL_Window  * window)
{
    IplImage opencvimg2 = (IplImage)matImg;
    IplImage* opencvimg = &opencvimg2;

     //Convert to SDL_Surface
    SDL_Surface* frameSurface = SDL_CreateRGBSurfaceFrom(
                              (void*)opencvimg->imageData,
                              opencvimg->width, opencvimg->height,
                              opencvimg->depth*opencvimg->nChannels,
                              opencvimg->widthStep,
                              0xff0000, 0x00ff00, 0x0000ff, 0);

    if(frameSurface == NULL)
    {
        SDL_Log("Couldn't convert Mat to Surface.");
        return NULL;
    }

    //Convert to SDL_Texture
    SDL_Texture* frameTexture = SDL_CreateTextureFromSurface(
                    SDL_GetRenderer(window), frameSurface);

    if(frameTexture == NULL)
    {
        SDL_Log("Couldn't convert Mat(converted to surface) to Texture.");
        return NULL;
    }
    else
    {
        SDL_Log("Mat to SDL_Texture conversion successfull");
        return frameTexture;
    }

    cvReleaseImage(&opencvimg);
}


static float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

GLuint glConvertMatToTexture(cv::Mat const &mat)
{
    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_UNPACK_ALIGNMENT, (mat.step & 3) ? 1 : 4);

    //set length of one complete row in data (doesn't need to equal image.cols)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, mat.step/mat.elemSize());

    //glReadPixels(0, 0, mat.cols, mat.rows, GL_BGR, GL_UNSIGNED_BYTE, mat.data);
    //cv::flip(mat, mat, 0);

    glGenTextures(1, &gltexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, gltexture);

    // does nothing ?
    glGenerateMipmap(GL_TEXTURE_2D);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexImage2D(GL_TEXTURE_2D,     // Type of texture
                 0,                 // Pyramid level (for mip-mapping) - 0 is the top level
                 GL_RGB8,            // Internal colour format to convert to
                 mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
                 mat.rows,          // Image height i.e. 480 for Kinect in standard mode
                 0,                 // Border width in pixels (can either be 1 or 0)
                 GL_BGR,            // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
                 GL_UNSIGNED_BYTE,  // Image data type
                 mat.ptr());        // The actual image data itself



    return gltexture;
}



/*                                                                              *
 * Worry-free text writing into OpenCV images                                   *
 * Author : Philipp Hasper, code used by courtesy of his author                 *
 * Link   :  http://www.hasper.info/worry-free-text-writing-into-opencv-images/ */

namespace ioxp {
    void putText(cv::Mat imgROI, const std::string &text, const int fontFace = cv::FONT_HERSHEY_PLAIN,
        const cv::Scalar color = cv::Scalar::all(255), const int thickness = 1, const int lineType = cv::LINE_8)
    {
        /*
         * Split the given text into its lines
         */
        std::vector<std::string> textLines;
        std::istringstream f(text);
        std::string s;

        while (std::getline(f, s, '\n')) {
            textLines.push_back(s);
        }

        /*
         * Calculate the line sizes and overall bounding box
         */
        std::vector<cv::Size> textLineSizes;
        cv::Size boundingBox(0,0);
        int baseline = 0;

        for (std::string line : textLines) {
            cv::Size lineSize = cv::getTextSize(line, fontFace, 1, thickness, &baseline);
            baseline += 2 * thickness;
            lineSize.width += 2 * thickness;
            lineSize.height += baseline;
            textLineSizes.push_back(lineSize);
            boundingBox.width = std::max(boundingBox.width, lineSize.width);
            boundingBox.height += lineSize.height;
        }

        const double scale = std::min(imgROI.rows / static_cast<double>(boundingBox.height),
                                      imgROI.cols / static_cast<double>(boundingBox.width));
        boundingBox.width *= scale;
        boundingBox.height *= scale;
        baseline *= scale;

        for (size_t i = 0; i < textLineSizes.size(); i++) {
            textLineSizes.at(i).width *= scale;
            textLineSizes.at(i).height *= scale;
        }
        /*
         * Draw the text line-by-line
         */
        int y = (imgROI.rows - boundingBox.height + baseline) / 2;

        for (size_t i = 0; i < textLines.size(); i++) {
            y += textLineSizes.at(i).height;
            // center the text horizontally
            cv::Point textOrg((imgROI.cols - textLineSizes.at(i).width) / 2, y - baseline);
            cv::putText(imgROI, textLines.at(i), textOrg, fontFace, scale, color, thickness, lineType);
        }
    }
}



