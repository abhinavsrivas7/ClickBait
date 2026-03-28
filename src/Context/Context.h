#pragma once
#include "../CommandBuffer/CommandBuffer.h"
#include "../Color/Color.h"

typedef struct {
    Platform      Platform;
    CommandBuffer CommandBuffer;
    PaletteCache  PaletteCache;
} Context;

Context *Context_Startup();
void Context_Shutdown(Context *context);