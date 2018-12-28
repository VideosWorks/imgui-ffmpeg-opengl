/*
 * Fichier d'en tête resetImageSource.hpp pour le projet miniDart
 * Auteur : Eric Bachard  / mercredi 26 décembre 2018, 14:20:14 (UTC+0200)
 * Ce document est sous Licence GPL v2
 * voir : http://www.gnu.org/licenses/gpl-2.0.html
 */


#ifndef __RESET_IMAGE_SOURCE_HPP
#define __RESET_IMAGE_SOURCE_HPP

#include <opencv2/opencv.hpp>

int resetImageSource(cv::VideoCapture *, int, const char *, int, int, int, double /* fps */);

#endif /* __RESET_IMAGE_SOURCE_HPP */
