/*
 * File audiodevices_helper.cpp belongs to miniDart project
 * Copyright : Eric Bachard  / 2019 July 7th, 14:35:03 (UTC+0200)
 * This document is under GPL v2 license
 * See : http://www.gnu.org/licenses/gpl-2.0.html
 */

#include <cstdint>
#include <cstring>
#include <vector>
#include <SDL2/SDL.h>

#ifndef _AUDIO_DEVICES_HPP
#define _AUDIO_DEVICES_HPP

// Everything is based on the recently improved SDL_API : https://wiki.libsdl.org/CategoryAudio
// SDL_GetAudioDeviceName(i, 0) => no recording capability
// SDL_GetAudioDeviceName(i, 1) => recording capability  // this is the important information

// SDL2 API PROPOSES : 
//     SDL_NewAudioStream
//     SDL_AudioStreamPut
//     SDL_AudioStreamAvailable
//     SDL_AudioStreamGet
//     SDL_AudioStreamFlush
//     SDL_AudioStreamClear
//     SDL_FreeAudioStream

// https://wiki.libsdl.org/SDL_LockAudioDevice
// https://wiki.libsdl.org/SDL_UnlockAudioDevice

// audio devices.hpp // is 8 sufficient  ??  (currently testing 8 with miniDart,
// and got NULL for number 6 and number 7 (values starting from 0)
#define MAX_RECORDING_DEVICES  8
// be consistent with SDL_AUDIO_MIN_BUFFER_SIZE (ffmpeg_player.hpp)
// try 4 or 8 seconds (multiple of 2 ?)
#define RECORDER_BUFFER_DEFAULT_SECONDS  4

const int RECORDING_BUFFER_TIME = RECORDER_BUFFER_DEFAULT_SECONDS + 1;

// not const, because can be granted, in case of problems (slow machine e.g.)

// APPLICATION 
// static int recording_buffer_size =  RECORDER_BUFFER_DEFAULT_VALUE + 1;
// static int numberOfAudioDevices;

// Audio recorder Callbacks
// void audioRecorderCallback(void* userdata, uint8_t* audio_stream, int len);
// void audioPlaybackCallback(void* userdata, uint8_t* audio_stream, int len);
//    void audioRecorderCallback(void* userdata, uint8_t* audio_stream, int len);
//    void audioPlaybackCallback(void* userdata, uint8_t* audio_stream, int len);

//audiodevices::init() : trouve toutes les sources sonores 
//0. non enregistrables
//1. enregistrables
// => keep "1" device names  only


namespace md  // miniDart alias
{
    //std::vector<std::string> recordableSDLAudioDeviceNames;
    short int   getSDLRecordableAudioDeviceNames(void);
    bool        selectSDLRecordableAudioDevice(short int);
    std::string getCurrentRecordableAudioDevice();
    int         audio_open(void *arg, int64_t wanted_channel_layout, int wanted_nb_channels, int wanted_sample_rate);

    enum AudioDeviceState
    {
        AUDIO_DEVICE_SELECTED_AND_VALID        =  1 << 0,
        AUDIO_DEVICE_NO_RECORD_CAPABILITY      =  1 << 1,
        AUDIO_DEVICE_WITH_RECORDING_CAPABILITY =  1 << 2,
        AUDIO_DEVICE_PLAYBACK                  =  1 << 3,
        AUDIO_DEVICE_NO_STATE                  =  1 << 4,
        AUDIO_DEVICE_ERROR                     =  1 << 5,
        AUDIO_DEVICE_NONE                      =  1 << 6
    };

    class AudioDevice
    {
      public:

        // Ctor
        AudioDevice();
        // Dtor
        ~AudioDevice();

        // SDL_GetNumAudioDevices( SDL_TRUE) will return the number
        // of audio devices, but not sure they are recordable => init() will check
        bool          init();
        void          cleanAndClose();

        // will probably use :
        // SDL_GetAudioDeviceName(i, SDL_TRUE) associated to name.str().cstr();
        std::string   getName(void);
        void          setName(std::string aName);

        // actualise la liste de périphériques son
        // does SDL allow that :  bool resetDevice() 
        bool          isSelected;
        bool          isRecordable;
        bool          isPlayback;

      private:
        std::string   s_name;
        int           currentState;
        SDL_AudioSpec audioDeviceReceivedRecordingSpec;
        SDL_AudioSpec audioDeviceReceivedPlaybackSpec;
    };

    // A vector of audioDevices, one can (un)select and start/stop recording
    // This object is meant to make the lik with SDL2 audio and the system.

    class AudioDevicesList
    {
      public:
        // Ctor
        AudioDevicesList();

        // Dtor
        ~AudioDevicesList();

        bool          init();
        void          clean_and_close();

        // a vector containing all the current detected audiodevices
        std::vector <AudioDevice > audioDevicesVector;

        // audiodDevice recordable ?
        int           addAudioDevice();
        void          audioRecordingCallback(void* userdata, uint8_t stream, int len);
        void          audioPlayingCallback(void* userdata, uint8_t stream, int len);

        // helper to select a given sound device
        bool          selectDevice(int aNumber);
        int           getCurrentSelectedDevice();
        void          startRecording(int currentRecordingDevice);
        bool          IsCurrentDeviceRecording();

        // FIXME : needs some work, starting reading the libav documentation + find code samples
        // audiodevices::toFFMPEG()  // need some love

        // getVectorSize() returns audioDevicesVector.size();
        int           getVectorSize();
        bool          setDriverName( char * name);
        char *        getDriverName();

        // visibility ?
        uint8_t *     audioDevicesBuffer;
        uint32_t      audioDevicesBufferByteSize;
        uint32_t      audioDevicesBufferBytePosition;
        uint32_t      audioDevicesBufferByteMaxPosition;

      private:
        bool          b_recording_audio;
        int           currentSelectedDevice;
        int           currentRecordingAudioDevice;
        char *        driver_name;
    };

} // namespace md;


#endif /* _AUDIO_DEVICES_HPP */
