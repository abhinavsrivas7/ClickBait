#include "Buffers.h"

static inline size_t GetCapacity(const CommandBuffer *that)
{
    return COMMAND_BUFFER_SIZE - that->Count;
}

static inline void CheckCapacityAndFlush(
    CommandBuffer *that, const Platform *platform, const size_t min)
{
    if(GetCapacity(that) < min)
    {
        CommandBuffer_Flush(that, platform);
    }
}

void DrawBuffer_Init(DrawBuffer *that)
{
    memset(that->Frame, UNDEFINED_PIXEL, sizeof(that->Frame));
}

void CommandBuffer_Init(CommandBuffer *that)
{
    that->Count = 0;
    that->Mode = COMMAND_BUFFER_MODE_ANSI;
}

void CommandBuffer_Flush(CommandBuffer *that, const Platform *platform)
{
    if (that->Count == 0) return;
    Platform_DumpToConsole(platform, that->Buffer, that->Count);
    that->Count = 0;
}

void CommandBuffer_AppendS(
    CommandBuffer *that, const Platform *platform, const char* command, const size_t size)
{
    assert(size < COMMAND_BUFFER_SIZE);
    CheckCapacityAndFlush(that, platform, size);
    memcpy(that->Buffer + that->Count, command, size);
    that->Count += size;
    CheckCapacityAndFlush(that, platform, COMMAND_BUFFER_FLUSH_THRESHOLD);
}

void CommandBuffer_AppendC(CommandBuffer *that, const Platform *platform, const char c)
{
    that->Buffer[that->Count++] = c;
    CheckCapacityAndFlush(that, platform, COMMAND_BUFFER_FLUSH_THRESHOLD);
}

void CommandBuffer_AppendFS(
    CommandBuffer *that, const Platform *platform, const char *commandFormat, ...)
{
    va_list args;
    va_start(args, commandFormat);
    CheckCapacityAndFlush(that, platform, COMMAND_BUFFER_FLUSH_THRESHOLD);
    char* offset = that->Buffer + that->Count;
    int written = vsnprintf(offset, GetCapacity(that), commandFormat, args);
    va_end(args);
    assert(written >= 0);
    that->Count += written;
    CheckCapacityAndFlush(that, platform, COMMAND_BUFFER_FLUSH_THRESHOLD);
}

void CommandBuffer_SetMode(CommandBuffer *that, const Platform *platform, Bool mode)
{
    if(that->Mode == mode || mode > COMMAND_BUFFER_MODE_SIXEL) 
    { 
        return; 
    }
    if(that->Mode == COMMAND_BUFFER_MODE_SIXEL) 
    {
        CommandBuffer_AppendS(that, platform, CommandAndSize(ExitSixelMode));
    }
    else 
    {
        CommandBuffer_AppendS(that, platform, CommandAndSize(EnterSixelMode));
    }

    that->Mode = mode;
}