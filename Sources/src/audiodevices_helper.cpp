/*
 * File audiodevices_helper.cpp belongs to miniDart project
 * Copyright : Eric Bachard  / 2019 april 26th, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include "application.hpp"
#include "audiodevices.hpp"
#include <vector>
#include <cstring>
#include "imgui.h"
#include "ffmpeg_player.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>


static short int count = 0;
static std::string currentRecordableAudioDevice = "";  // default device

extern SDL_AudioDeviceID audio_dev;
extern std::vector<std::string> recordableAudioDeviceNames;

namespace md

{
    short int getSDLRecordableAudioDeviceNames(void)
    {
        while (SDL_GetAudioDeviceName(count, 1) != nullptr)
        {
            recordableAudioDeviceNames.push_back(SDL_GetAudioDeviceName(count, 1));
            std::cout << "Audio device (with recording capability) device number : " << count  << " name : "  <<  SDL_GetAudioDeviceName(count, 1) << std::endl;
            count++;

            if (count > 4)
                break;
        }
        return count;
    }

    bool selectSDLRecordableAudioDeviceName(short int aDevice)
    {
        if (aDevice >= count)
        {
            std::cerr << "Wrong device" << std::endl;
            return false;
        }
        currentRecordableAudioDevice = recordableAudioDeviceNames.at(aDevice);
        return true;
    }

    std::string getCurrentRecordableAudioDevice(void)
    {
        return currentRecordableAudioDevice;
    }

    int audio_open(void *arg, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate)
    {
        // define audio specs containers
        SDL_AudioSpec wanted_specs;
        SDL_AudioSpec specs;

        // set audio settings
        wanted_specs.freq = wanted_sample_rate;
        wanted_specs.format = AUDIO_S16SYS;
        wanted_specs.channels = wanted_nb_channels;
        wanted_specs.silence = 0;
        wanted_specs.samples = SDL_AUDIO_MIN_BUFFER_SIZE * 2;
        wanted_specs.callback = audio_callback;
        wanted_specs.userdata = arg;

        if (!(audio_dev = SDL_OpenAudioDevice((const char *)currentRecordableAudioDevice.c_str(), 0, &wanted_specs, &specs, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE)))
        {
            std::cout << "[audio]could not open audio, error:" << SDL_GetError() << std::endl;
            return -1;
        }
        return  specs.size;
    }


} /* end namespace md */
