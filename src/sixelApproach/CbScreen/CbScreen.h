#pragma once
#include "../common.h"
#include "../CbRenderer/CbDrawBuffer.h"

typedef struct { float X; float Y; } FLOAT_COORD;

typedef struct { 
    COORD Cells; 
    COORD Pixels; 
    FLOAT_COORD CellsPerPixel;
    FLOAT_COORD PixelsPerCell;
} CbScreen;

void CbScreen_Update(CbScreen *screenSize, HANDLE stdIn, HANDLE stdOut);
COORD CbScreen_PixelToCell(const CbScreen *screenSize, COORD pixel);
COORD CbScreen_CellToTopLeftPixel(const CbScreen *screenSize, COORD cell);
COORD CbScreen_FindCenter(const CbScreen *screen);