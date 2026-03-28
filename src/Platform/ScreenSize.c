#include <math.h>
#include "Platform.h"
#define ANSI_START           1
#define POLLING_RETRY_DELAY  10
#define EVENT_BUFFER_SIZE    256
#define RESPONSE_BUFFER_SIZE 1024

Point Platform_PixelToCell(const Platform *that, const Point pixel)
{
    return (Point) { //1 based ANSI offset
        .X = (short) (
            floorf(((float) pixel.X) * that->ScreenSize.CellsPerPixel.X) + ANSI_START
        ),
        .Y = (short) (
            floorf(((float) pixel.Y) * that->ScreenSize.CellsPerPixel.Y) + ANSI_START
        )
    };
}

Point Platform_CellToTopLeftPixel(const Platform *that, const Point cell)
{
    return (Point) { //-1 to get width upto end of previous cell
        .X = (short) (
            (float) (cell.X - ANSI_START) * that->ScreenSize.PixelsPerCell.X
        ),
        .Y = (short) (
            (float) (cell.Y - ANSI_START) * that->ScreenSize.PixelsPerCell.Y
        )
    };
}

Point Platform_FindScreenCenter(const Platform *that)
{
    return (Point) { 
        .X = that->ScreenSize.Pixels.X >> 1, 
        .Y = that->ScreenSize.Pixels.Y >> 1 
    };
}

static inline Bool Platform_HasEvents(
    const Platform *that, INPUT_RECORD *eventBuffer, Word *eventsRead)
{
    Word pendingEvents;

    return GetNumberOfConsoleInputEvents(that->In, &pendingEvents)
        && pendingEvents > 0 
        && ReadConsoleInputW(that->In, eventBuffer, EVENT_BUFFER_SIZE, eventsRead)
        && *eventsRead > 0;
}

static inline Bool Platform_IsKeyDownEvent(const INPUT_RECORD *event)
{
    return event->EventType == KEY_EVENT && event->Event.KeyEvent.bKeyDown;
}

static Bool Platform_AssignScreenSize(Platform *that, const wchar_t *respBuffer)
{
    wchar_t *pixels = wcsstr(respBuffer, L"[4;");
    wchar_t *cells = wcsstr(respBuffer, L"[8;");

    if (pixels && cells) 
    {
        swscanf(pixels, L"[4;%hd;%hdt", &that->ScreenSize.Pixels.Y, &that->ScreenSize.Pixels.X);
        swscanf(cells, L"[8;%hd;%hdt", &that->ScreenSize.Cells.Y, &that->ScreenSize.Cells.X);
        
        that->ScreenSize.CellsPerPixel = (FloatPoint) {
            .X = (float)that->ScreenSize.Cells.X / (float)that->ScreenSize.Pixels.X,
            .Y = (float)that->ScreenSize.Cells.Y / (float)that->ScreenSize.Pixels.Y
        };

        that->ScreenSize.PixelsPerCell = (FloatPoint) {
            .X = 1.0f / that->ScreenSize.CellsPerPixel.X,
            .Y = 1.0f / that->ScreenSize.CellsPerPixel.Y,
        };

        return true;
    }

    return false;
}

void Platform_UpdateScreenSize(Platform *that)
{
    FlushConsoleInputBuffer(that->In);
    Platform_DumpToConsole(that, CommandAndSize(ExitSixelMode));
    Platform_DumpToConsole(that, CommandAndSize(RequestScreenSize));
    wchar_t respBuffer[RESPONSE_BUFFER_SIZE] = {0}; int respBufferIndex = 0;
    INPUT_RECORD eventBuffer[EVENT_BUFFER_SIZE]; Word eventsRead;

    while(true)
    {
        if (Platform_HasEvents(that, eventBuffer, &eventsRead))
        {    
            for (Word i = 0; i < eventsRead; i++) 
            {  
                if (Platform_IsKeyDownEvent(&eventBuffer[i])) 
                {
                    wchar_t c = eventBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                    assert(respBufferIndex < RESPONSE_BUFFER_SIZE - 1);
                    if (c != 0) { respBuffer[respBufferIndex++] = c; }
                }
            }

            if(Platform_AssignScreenSize(that, respBuffer)) { break; }
        }
        
        Sleep(POLLING_RETRY_DELAY);
    }
}
