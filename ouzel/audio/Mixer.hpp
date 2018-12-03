// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_AUDIO_MIXER_HPP
#define OUZEL_AUDIO_MIXER_HPP

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <set>
#include <vector>
#include "audio/Object.hpp"

namespace ouzel
{
    namespace audio
    {
        class Mixer
        {
        public:
            struct Command
            {
                enum class Type
                {
                    DELETE_OBJECT,
                    INIT_BUS,
                    ADD_LISTENER,
                    REMOVE_LISTENER,
                    ADD_PROCESSOR,
                    REMOVE_PROCESSOR,
                    INIT_LISTENER,
                    UPDATE_LISTENER,
                    INIT_PROCESSOR,
                    UPDATE_PROCESSOR,
                    ADD_OUTPUT_BUS,
                    SET_MASTER_BUS
                };

                Command() {}
                explicit Command(Type initType): type(initType) {}

                Type type;
                uintptr_t objectId;
                uintptr_t destinationObjectId;
                std::function<std::unique_ptr<Object>(void)> createFunction;
                std::function<void(Object*)> updateFunction;
            };

            Mixer() {}

            Mixer(const Mixer&) = delete;
            Mixer& operator=(const Mixer&) = delete;

            Mixer(Mixer&&) = delete;
            Mixer& operator=(Mixer&&) = delete;

            void addCommand(const Command& command);
            
            void process();
            void getData(uint32_t frames, uint32_t sampleRate, uint16_t channels, std::vector<float>& result);

            uintptr_t getObjectId()
            {
                auto i = deletedObjectIds.begin();

                if (i == deletedObjectIds.end())
                    return ++lastObjectId; // zero is reserved for null node
                else
                {
                    uintptr_t objectId = *i;
                    deletedObjectIds.erase(i);
                    return objectId;
                }
            }

            void deleteObjectId(uintptr_t objectId)
            {
                deletedObjectIds.insert(objectId);
            }

        private:
            uintptr_t lastObjectId = 0;
            std::set<uintptr_t> deletedObjectIds;

            std::vector<std::unique_ptr<Object>> objects;

            Object* masterBus = nullptr;
            std::mutex commandMutex;
            std::condition_variable commandConditionVariable;
            std::queue<Command> commandQueue;
        };
    } // namespace audio
} // namespace ouzel

#endif // OUZEL_AUDIO_MIXER_HPP