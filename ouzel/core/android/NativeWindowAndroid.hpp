// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#pragma once

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "core/NativeWindow.hpp"

namespace ouzel
{
    class NativeWindowAndroid final: public NativeWindow
    {
    public:
        NativeWindowAndroid(EventHandler& initEventHandler,
                            const std::string& newTitle);
        virtual ~NativeWindowAndroid();

        void handleResize(const Size2& newSize);
        void handleSurfaceChange(jobject surface);
        void handleSurfaceDestroy();

        ANativeWindow* getNativeWindow() const { return window; }

    private:
        ANativeWindow* window = nullptr;
    };
}
