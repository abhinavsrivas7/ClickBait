#include "CbDrawBuffer.h"
CbDrawBufferState *State = NULL;

static inline size_t GetCapacity()
{
    return CB_DRAW_BUFFER_SIZE - State->Count;
}

static int WriteToBuffer(HANDLE stdOut, const char *commandFormat, va_list args)
{
    va_list argsCopy;
    va_copy(argsCopy, args);
    size_t capacity = GetCapacity();
       
    int written = vsnprintf(
        State->Buffer + State->Count, capacity, commandFormat, argsCopy
    );
    
    va_end(argsCopy);

    if(written < 0 || ((size_t)written >= capacity))
    {
        CbDrawBuffer_FlushBuffer(stdOut);
        
        written = vsnprintf(
            State->Buffer, CB_DRAW_BUFFER_SIZE, commandFormat, args
        );
    }

    return written;
}

void CbDrawBuffer_Init()
{
    State = malloc(sizeof(CbDrawBufferState));
    assert(State);
    State->Count = 0;
    State->Mode = UNSET;
}

void CbDrawBuffer_Draw(HANDLE stdOut, const char *commandFormat, ...)
{
    va_list args;
    va_start(args, commandFormat);
    int written = WriteToBuffer(stdOut, commandFormat, args);
    va_end(args);
    size_t capacity = GetCapacity();
    assert(written >= 0 && ((size_t) written <= capacity));
    State->Count += written;
    
    if(capacity < CB_DRAW_BUFFER_FLUSH_THRESHOLD)
    {
        CbDrawBuffer_FlushBuffer(stdOut);
    }
}

void CbDrawBuffer_DrawChar(HANDLE stdOut, char c)
{
    if(GetCapacity() < CB_DRAW_BUFFER_FLUSH_THRESHOLD)
    {
        CbDrawBuffer_FlushBuffer(stdOut);
    }

    State->Buffer[State->Count++] = c;
}

void CbDrawBuffer_SetBufferMode(HANDLE stdOut, CbDrawBufferModes mode)
{
    if(State->Mode == mode) return;
    if(State->Mode == SIXEL) CbDrawBuffer_Draw(stdOut, "\033\\");
    if(mode == SIXEL) CbDrawBuffer_Draw(stdOut, "\033Pq\"1;1");

    State->Mode = mode;
}

void CbDrawBuffer_FlushBuffer(HANDLE stdOut)
{
    if (State->Count == 0) return;
    DWORD written;
    WriteConsoleA(stdOut, State->Buffer, (DWORD) State->Count, &written, NULL);
    State->Count = 0;
    State->Mode = UNSET;
}

void CbDrawBuffer_ImmediateFlush(HANDLE stdOut, const char* command, size_t size)
{
    DWORD written;
    WriteConsoleA(stdOut, command, (DWORD) size, &written, NULL);
}

void CbDrawBuffer_Free()
{
    assert(State->Count == 0);
    free(State);
}