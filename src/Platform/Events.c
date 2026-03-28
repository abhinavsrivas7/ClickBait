#include "Platform.h"

static Word read;
static INPUT_RECORD event;
static Event NoEvent            = { .Event = PLATFORM_NO_EVENT             };
static Event ExitEvent          = { .Event = PLATFORM_EXIT_EVENT           };
static Event WindowResizedEvent = { .Event = PLATFORM_WINDOW_RESIZED_EVENT };

Event Platform_PollForEvents(const Platform *that)
{
    if(!ReadConsoleInputW(that->In, &event, 1, &read))
    {
        return NoEvent;
    }     
    if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
    {
        return WindowResizedEvent;
    }
    else if(event.EventType == KEY_EVENT && event.Event.KeyEvent.bKeyDown)
    {
        if(event.Event.KeyEvent.uChar.AsciiChar == QUIT_COMMAND)
        {
            return ExitEvent;
        }
        else 
        {
            return (Event) { 
                .Event = PLATFORM_KEY_PRESSED_EVENT,
                .Character = event.Event.KeyEvent.uChar.AsciiChar
            };
        }
    }
    else if(event.EventType == MOUSE_EVENT)
    {    
        Point position = {
            .X = event.Event.MouseEvent.dwMousePosition.X,
            .Y = event.Event.MouseEvent.dwMousePosition.Y
        };    

        if(event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            return (Event) { 
                .Event = PLATFORM_LEFT_MOUSE_CLICKED_EVENT, 
                .Position = position
            };
        }
        if(event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
        {
            return (Event) { 
                .Event = PLATFORM_RIGHT_MOUSE_CLICKED_EVENT, 
                .Position = position
            };
        }
        if(event.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
        {            
            return (Event) { 
                .Event = ((short) HIWORD(event.Event.MouseEvent.dwButtonState)) > 0 
                    ? PLATFORM_MOUSE_WHEELED_UP_EVENT
                    : PLATFORM_MOUSE_WHEELED_DOWN_EVENT, 
                .Position = position
            }; 
        }
    }

    return NoEvent;
}