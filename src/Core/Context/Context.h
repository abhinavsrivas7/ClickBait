#pragma once
#include "../Buffers/Buffers.h"

typedef struct {
    Platform      Platform;
    CommandBuffer CommandBuffer;
    PaletteCache  PaletteCache;
    DrawBuffer    DrawBuffer;
} Context;

Context *Context_Startup();
void Context_Shutdown(Context *context);