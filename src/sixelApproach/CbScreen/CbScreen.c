#include "CbScreen.h"
#include <math.h>
#define ANSI_START_INDEX 1
#define POLLING_RETRY_DELAY 10
#define EVENT_BUFFER_SIZE 256
#define RESPONSE_BUFFER_SIZE 1024

void CbScreen_Update(CbScreen *screen, HANDLE stdIn, HANDLE stdOut)
{
    FlushConsoleInputBuffer(stdIn);
    char command[] = "\033[14t\033[18t";
    CbDrawBuffer_SetBufferMode(stdOut, ANSI);
    CbDrawBuffer_ImmediateFlush(stdOut, command, sizeof(command) - 1);
    WCHAR responseBuffer[RESPONSE_BUFFER_SIZE] = {0}; 
    int responseBufferIndex = 0;
    INPUT_RECORD eventBuffer[EVENT_BUFFER_SIZE];
    DWORD eventsRead;

    while(true)
    {
        DWORD pending = 0;
        GetNumberOfConsoleInputEvents(stdIn, &pending);
        
        Bool shouldBuffer = pending > 0 
            ? ReadConsoleInputW(stdIn, eventBuffer, EVENT_BUFFER_SIZE, &eventsRead)
            : false;
        
        if (shouldBuffer) 
        {    
            for (DWORD i = 0; i < eventsRead; i++) 
            {  
                Bool isKeyDownEvent = eventBuffer[i].EventType == KEY_EVENT 
                    && eventBuffer[i].Event.KeyEvent.bKeyDown;

                if (isKeyDownEvent) 
                {
                    WCHAR c = eventBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                    assert(responseBufferIndex < RESPONSE_BUFFER_SIZE - 1);

                    if (c != 0) 
                    {
                        responseBuffer[responseBufferIndex++] = c;
                    }
                }
            }

            WCHAR *pixels = wcsstr(responseBuffer, L"[4;");
            WCHAR *cells = wcsstr(responseBuffer, L"[8;");

            if (pixels && cells) 
            {
                swscanf(
                    pixels, 
                    L"[4;%hd;%hdt", 
                    &screen->Pixels.Y, 
                    &screen->Pixels.X
                );
                
                swscanf(
                    cells, 
                    L"[8;%hd;%hdt", 
                    &screen->Cells.Y, 
                    &screen->Cells.X
                );
                
                screen->CellsPerPixel = (FLOAT_COORD) {
                    .X = (float)screen->Cells.X / (float)screen->Pixels.X,
                    .Y = (float)screen->Cells.Y / (float)screen->Pixels.Y
                };

                screen->PixelsPerCell = (FLOAT_COORD) {
                    .X = 1.0f / screen->CellsPerPixel.X,
                    .Y = 1.0f / screen->CellsPerPixel.Y,
                };

                break; 
            }
        }
        
        Sleep(POLLING_RETRY_DELAY);
    }
}

COORD CbScreen_PixelToCell(const CbScreen *screen, COORD pixel)
{
    return (COORD) { //1 based ANSI offset
        .X = (SHORT) (
            floorf(((float) pixel.X) * screen->CellsPerPixel.X) + ANSI_START_INDEX
        ),
        .Y = (SHORT) (
            floorf(((float) pixel.Y) * screen->CellsPerPixel.Y) + ANSI_START_INDEX
        )
    };
}

COORD CbScreen_CellToTopLeftPixel(const CbScreen *screen, COORD cell)
{
    return (COORD) { //-1 to get width upto end of previous cell
        .X = (SHORT) ((float) (cell.X - ANSI_START_INDEX) * screen->PixelsPerCell.X),
        .Y = (SHORT) ((float) (cell.Y - ANSI_START_INDEX) * screen->PixelsPerCell.Y)
    };
}

COORD CbScreen_FindCenter(const CbScreen *screen)
{
    SMALL_RECT rectangle = { 
        .Top = 0, 
        .Bottom = screen->Pixels.Y,
        .Left = 0, 
        .Right = screen->Pixels.X,
    };

    return (COORD) {
        .X = (rectangle.Right + rectangle.Left) >> 1,
        .Y = (rectangle.Bottom + rectangle.Top) >> 1,
    };
}