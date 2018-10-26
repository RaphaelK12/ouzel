// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#pragma once

#include "graphics/RenderResource.hpp"

namespace ouzel
{
    namespace graphics
    {
        class RenderDeviceEmpty;

        class ShaderResourceEmpty final: public RenderResource
        {
        public:
            ShaderResourceEmpty(RenderDeviceEmpty& renderDeviceEmpty);
        };
    } // namespace graphics
} // namespace ouzel
