#pragma once
#include "../common.h"
#define CB_DRAW_BUFFER_SIZE 268435456 //256 MB
#define CB_DRAW_BUFFER_FLUSH_THRESHOLD 64 //256 MB

typedef enum { 
    UNSET = 0, 
    ANSI = 1, 
    SIXEL = 2 
} CbDrawBufferModes;

typedef struct {
    char Buffer[CB_DRAW_BUFFER_SIZE];
    size_t Count;
    CbDrawBufferModes Mode;
} CbDrawBufferState;

void CbDrawBuffer_Init(); //to be changed later to be allocated as part of cbenv
void CbDrawBuffer_SetBufferMode(HANDLE stdOut, CbDrawBufferModes mode);
void CbDrawBuffer_Draw(HANDLE stdOut, const char *commandFormat, ...);
void CbDrawBuffer_DrawChar(HANDLE stdOut, char c);
void CbDrawBuffer_FlushBuffer(HANDLE stdOut);
void CbDrawBuffer_ImmediateFlush(HANDLE stdOut, const char* command, size_t size);
void CbDrawBuffer_Free();