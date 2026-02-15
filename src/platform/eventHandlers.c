#include "headers/eventHandlers.h"
#include "headers/graphics.h"

void resetAndReAssign(const HANDLE stdOut, MOUSE_EVENT_RECORD *curr, MouseResetEvent *prev)
{
    if(prev->assigned)
    {
        resetCell(stdOut, &prev->existing.dwMousePosition);
    }

    prev->existing = *curr;
    prev->assigned = true;
}

void handleMouseEvent(
    const HANDLE stdOut, MOUSE_EVENT_RECORD *event, MouseResetEvent *existing)
{
    COORD position = event->dwMousePosition;
    
    if(event->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        resetAndReAssign(stdOut, event, existing);
        setCellBackground(stdOut, &position, RED);
    }

    if(event->dwButtonState & RIGHTMOST_BUTTON_PRESSED)
    {
        resetAndReAssign(stdOut, event, existing);
        setCellBackground(stdOut, &position, GREEN);
    }

    if(event->dwEventFlags == MOUSE_WHEELED)
    {
        SHORT state = (SHORT) HIWORD(event->dwButtonState);
        printf("Scroll %s at (%d,%d)\n", state > 0 ? "up" : "down", position.X, position.Y);
    }
}

void handleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD *event, COORD *screenSize)
{
    COORD size = event->dwSize;
    printf("Window resized to (%d, %d)\n", size.X, size.Y);
    screenSize->X = size.X;
    screenSize->Y = size.Y;
}


void handleKeyEvent(KEY_EVENT_RECORD *event)
{
    printf("%c", event->uChar.AsciiChar);
}