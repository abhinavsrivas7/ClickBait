#pragma once
#include "include.h"

void handleMouseEvent(MOUSE_EVENT_RECORD *event);
void handleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD *event, COORD *screenSize);
void handleKeyEvent(const KEY_EVENT_RECORD *event);
Bool isExitEvent(const INPUT_RECORD *event); 