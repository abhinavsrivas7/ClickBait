#include "headers/eventHandlers.h"
#include "headers/graphics.h"

void handleMouseEvent(MOUSE_EVENT_RECORD *event)
{
    COORD position = event->dwMousePosition;
    
    if(event->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        return;
    }

    if(event->dwButtonState & RIGHTMOST_BUTTON_PRESSED)
    {
        return;
    }

    if(event->dwEventFlags == MOUSE_WHEELED)
    {
        SHORT state = (SHORT) HIWORD(event->dwButtonState);
        printf("Scroll %s at (%d,%d)\n", state > 0 ? "up" : "down", position.X, position.Y);
    }
}

void handleWindowResizeEvent(const WINDOW_BUFFER_SIZE_RECORD *event, COORD *screenSize)
{
    screenSize->X = event->dwSize.X;
    screenSize->Y = event->dwSize.Y;
}

void handleKeyEvent(const KEY_EVENT_RECORD *event)
{
    printf("%c", event->uChar.AsciiChar);
}

Bool isExitEvent(const INPUT_RECORD *event)
{
    return event->EventType == KEY_EVENT
        && event->Event.KeyEvent.bKeyDown 
        && event->Event.KeyEvent.uChar.AsciiChar == QUIT_COMMAND;
}