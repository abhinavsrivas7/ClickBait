#pragma once
#include "../Platform/Platform.h"
#define COMMAND_BUFFER_SIZE            (256 * 1024 * 1024) 
#define COMMAND_BUFFER_FLUSH_THRESHOLD 64 
typedef enum                           { ANSI = 0, SIXEL = 1 } CommandBufferModes;

typedef struct {
    CommandBufferModes Mode; size_t Count; char Buffer[COMMAND_BUFFER_SIZE];
} CommandBuffer;

void CommandBuffer_Init(CommandBuffer *that);
void CommandBuffer_Flush(CommandBuffer *that, const Platform *platform);

void CommandBuffer_AppendC(
    CommandBuffer *that, const Platform *platform, const char c
);

void CommandBuffer_AppendS(
    CommandBuffer *that, const Platform *platform, const char* command, const size_t size
);

void CommandBuffer_AppendFS(
    CommandBuffer *that, const Platform *platform, const char *commandFormat, ...
);

void CommandBuffer_SetMode(
    CommandBuffer *that, const Platform *platform, CommandBufferModes mode
);