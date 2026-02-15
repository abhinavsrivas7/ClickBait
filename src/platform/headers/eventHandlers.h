#pragma once
#include "include.h"
#include "../../headers/bool.h"

typedef struct _MouseResetEvent { 
    MOUSE_EVENT_RECORD existing; BOOL assigned; 
} MouseResetEvent;

void handleMouseEvent(
    const HANDLE stdOut, MOUSE_EVENT_RECORD *event, MouseResetEvent *existing
);

void handleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD *event, COORD *screenSize);

void handleKeyEvent(KEY_EVENT_RECORD *event);