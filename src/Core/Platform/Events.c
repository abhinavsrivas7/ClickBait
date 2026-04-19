#include "Platform.h"

static Word read;
static INPUT_RECORD privateEvent;

void Platform_PollForEvents(const Platform *that, Event *event)
{
    if(!ReadConsoleInputW(that->In, &privateEvent, 1, &read))
    {
        event->Event = PLATFORM_NO_EVENT;
        return;
    }     
    if(privateEvent.EventType == WINDOW_BUFFER_SIZE_EVENT)
    {
        event->Event = PLATFORM_WINDOW_RESIZED_EVENT;
        return;
    }
    else if(privateEvent.EventType == KEY_EVENT && privateEvent.Event.KeyEvent.bKeyDown)
    {
        if(privateEvent.Event.KeyEvent.uChar.AsciiChar == QUIT_COMMAND)
        {
            event->Event = PLATFORM_EXIT_EVENT;
            return;
        }
        else 
        {
            event->Event = PLATFORM_KEY_PRESSED_EVENT;
            event->Character = privateEvent.Event.KeyEvent.uChar.AsciiChar;
            return;
        }
    }
    else if(privateEvent.EventType == MOUSE_EVENT)
    {      
        event->Position.X = privateEvent.Event.MouseEvent.dwMousePosition.X;
        event->Position.Y = privateEvent.Event.MouseEvent.dwMousePosition.Y;  

        if(privateEvent.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            event->Event = PLATFORM_LEFT_MOUSE_CLICKED_EVENT;   
            return;
        }
        if(privateEvent.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
        {
            event->Event = PLATFORM_RIGHT_MOUSE_CLICKED_EVENT;   
            return;
        }
        if(privateEvent.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
        {   
            short state = (short) HIWORD(privateEvent.Event.MouseEvent.dwButtonState);     
            
            event->Event = state > 0 
                ? PLATFORM_MOUSE_WHEELED_UP_EVENT
                : PLATFORM_MOUSE_WHEELED_DOWN_EVENT;

            return;
        }
    }

    event->Event = PLATFORM_NO_EVENT;
    return;
}

Bool Platform_IsMouseClickedEvent(Event *event) 
{ 
    return event->Event >= PLATFORM_LEFT_MOUSE_CLICKED_EVENT 
        && event->Event <= PLATFORM_RIGHT_MOUSE_CLICKED_EVENT;
}