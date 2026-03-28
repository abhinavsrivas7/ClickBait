#include "Context.h"

Context *Context_Startup()
{
    Context *that = malloc(sizeof(Context));
    assert(that);
    Platform_InitForStartup(&that->Platform);
    CommandBuffer_Init(&that->CommandBuffer);
    Color_InitPaletteCache(&that->PaletteCache);
    return that;
}

void Context_Shutdown(Context *that)
{
    assert(that);
    Platform_RestoreForShutdown(&that->Platform);
    free(that);
}