#pragma once
#include "../Color/Color.h"
#define FRAME_WIDTH                    3840
#define FRAME_HEIGHT                   2160
#define UNIQUE_COLORS                  (CHANNEL_SIZE * CHANNEL_SIZE * CHANNEL_SIZE)
#define UNDEFINED_PIXEL                UNIQUE_COLORS
#define COMMAND_BUFFER_SIZE            (256 * 1024 * 1024) 
#define COMMAND_BUFFER_MODE_ANSI       0
#define COMMAND_BUFFER_MODE_SIXEL      1
#define COMMAND_BUFFER_FLUSH_THRESHOLD 64 

//DRAW BUFFER
typedef struct { Channel Frame[FRAME_HEIGHT][FRAME_WIDTH]; } DrawBuffer;
void DrawBuffer_Init(DrawBuffer *that);

//COMMAND BUFFER
typedef struct { 
    Bool Mode; size_t Count; char Buffer[COMMAND_BUFFER_SIZE]; 
} CommandBuffer;

void CommandBuffer_Init(CommandBuffer *that);
void CommandBuffer_Flush(CommandBuffer *that, const Platform *platform);

void CommandBuffer_SetMode(
    CommandBuffer *that, const Platform *platform, Bool mode
);

void CommandBuffer_AppendC(
    CommandBuffer *that, const Platform *platform, const char c
);

void CommandBuffer_AppendS(
    CommandBuffer *that, 
    const Platform *platform, 
    const char* command, 
    const size_t size
);

void CommandBuffer_AppendFS(
    CommandBuffer *that, 
    const Platform *platform, 
    const char *commandFormat, ...
);