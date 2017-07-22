// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_METAL

#include <algorithm>
#include "BufferResourceMetal.h"
#include "RendererMetal.h"
#include "core/Engine.h"
#include "utils/Log.h"

namespace ouzel
{
    namespace graphics
    {
        BufferResourceMetal::BufferResourceMetal()
        {
        }

        BufferResourceMetal::~BufferResourceMetal()
        {
            if (buffer)
            {
                [buffer release];
            }
        }

        bool BufferResourceMetal::init(Buffer::Usage newUsage, bool newDynamic)
        {
            if (!BufferResource::init(newUsage, newDynamic))
            {
                return false;
            }

            return true;
        }

        bool BufferResourceMetal::init(Buffer::Usage newUsage, const void* newData, uint32_t newSize, bool newDynamic)
        {
            if (!BufferResource::init(newUsage, newData, newSize, newDynamic))
            {
                return false;
            }

            if (!data.empty())
            {
                if (buffer) [buffer release];

                RendererMetal* rendererMetal = static_cast<RendererMetal*>(sharedEngine->getRenderer());

                buffer = [rendererMetal->getDevice() newBufferWithLength:bufferSize
                                                                 options:MTLResourceCPUCacheModeWriteCombined];

                if (!buffer)
                {
                    Log(Log::Level::ERR) << "Failed to create Metal buffer";
                    return false;
                }

                std::copy(data.begin(), data.end(), static_cast<uint8_t*>([buffer contents]));

                bufferSize = static_cast<uint32_t>(data.size());
            }

            return true;
        }

        bool BufferResourceMetal::setData(const void* newData, uint32_t newSize)
        {
            if (!BufferResource::setData(newData, newSize))
            {
                return false;
            }

            if (!data.empty())
            {
                if (!buffer || data.size() > bufferSize)
                {
                    if (buffer) [buffer release];

                    RendererMetal* rendererMetal = static_cast<RendererMetal*>(sharedEngine->getRenderer());

                    buffer = [rendererMetal->getDevice() newBufferWithLength:bufferSize
                                                                     options:MTLResourceCPUCacheModeWriteCombined];

                    if (!buffer)
                    {
                        Log(Log::Level::ERR) << "Failed to create Metal buffer";
                        return false;
                    }

                    bufferSize = static_cast<uint32_t>(data.size());
                }

                std::copy(data.begin(), data.end(), static_cast<uint8_t*>([buffer contents]));
            }

            return true;
        }

        bool BufferResourceMetal::upload()
        {
            std::lock_guard<std::mutex> lock(uploadMutex);

            if (dirty)
            {
                if (!data.empty())
                {
                    if (!buffer || data.size() > bufferSize)
                    {
                        if (buffer) [buffer release];

                        RendererMetal* rendererMetal = static_cast<RendererMetal*>(sharedEngine->getRenderer());

                        buffer = [rendererMetal->getDevice() newBufferWithLength:bufferSize
                                                                         options:MTLResourceCPUCacheModeWriteCombined];

                        if (!buffer)
                        {
                            Log(Log::Level::ERR) << "Failed to create Metal buffer";
                            return false;
                        }

                        bufferSize = static_cast<uint32_t>(data.size());
                    }

                    std::copy(data.begin(), data.end(), static_cast<uint8_t*>([buffer contents]));
                }

                dirty = 0;
            }

            return true;
        }
    } // namespace graphics
} // namespace ouzel

#endif
