// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#pragma once

#include "core/Setup.h"

#if OUZEL_COMPILE_ALSA

#include <atomic>
#include <thread>
#include <alsa/asoundlib.h>

#include "audio/AudioDevice.hpp"

namespace ouzel
{
    namespace audio
    {
        class AudioDeviceALSA final: public AudioDevice
        {
            friend Audio;
        public:
            virtual ~AudioDeviceALSA();

        protected:
            AudioDeviceALSA();

            void run();

        private:
            snd_pcm_t* playbackHandle = nullptr;
            snd_pcm_hw_params_t* hwParams = nullptr;
            snd_pcm_sw_params_t* swParams = nullptr;

            unsigned int periods = 4;
            snd_pcm_uframes_t periodSize = 1024;

            std::vector<uint8_t> data;

            std::atomic_bool running;
            std::thread audioThread;
        };
    } // namespace audio
} // namespace ouzel

#endif
