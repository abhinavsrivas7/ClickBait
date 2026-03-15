#include "CbRenderer.h"

void CbRenderer_SetColor(HANDLE stdOut, const Color color, SHORT colorRegister)
{   // #1 (use color register number 1 [uptp 256]). ;2 (24 bit color gamut mode)
    CbDrawBuffer_Draw(
        stdOut, "#%hd;2;%d;%d;%d", colorRegister, color.R, color.G, color.B
    );
}

void CbRenderer_UseColor(HANDLE stdOut, SHORT colorRegister)
{
    CbDrawBuffer_Draw(stdOut, "#%hd", colorRegister);
}

void CbRenderer_DrawColumn(HANDLE stdOut, BYTE bits[8])
{
    BYTE pixels = 0;
    
    for (int i = 0; i < SIXEL_HEIGHT; i++) 
    { 
        pixels |= bits[i] << i; 
    }

    CbDrawBuffer_DrawChar(stdOut, (char)(pixels + SIXEL_OFFSET));
}

COORD CbRenderer_StartDrawing(HANDLE stdOut, const CbScreen *screenSize, COORD pixel)
{
    COORD cell = CbScreen_PixelToCell(screenSize, pixel);
    CbRenderer_NavigateToCell(stdOut, cell);
    COORD cellTopLeft = CbScreen_CellToTopLeftPixel(screenSize, cell);
    COORD offsets = { .X = pixel.X - cellTopLeft.X, .Y = pixel.Y - cellTopLeft.Y };
    CbDrawBuffer_SetBufferMode(stdOut, SIXEL);
    CbRenderer_MoveDownByStrips(stdOut, offsets.Y / SIXEL_HEIGHT);
    CbRenderer_MoveRightByColumns(stdOut, offsets.X);
    return (COORD) { .X = offsets.X, .Y = offsets.Y % 6}; 
}

void CbRenderer_NavigateToCell(HANDLE stdOut, COORD cell)
{
    CbDrawBuffer_SetBufferMode(stdOut, ANSI);
    CbDrawBuffer_Draw(stdOut, "\033[%hd;%hdH", cell.Y, cell.X);
}

void CbRenderer_MoveRightByColumns(HANDLE stdOut, SHORT columns)
{
    if (columns > 0) 
    { 
        CbDrawBuffer_Draw(stdOut, "!%hd?", columns); 
    }
}

void CbRenderer_MoveDownByStrips(HANDLE stdOut, SHORT strips) 
{ 
    for (SHORT i = 0; i < strips; i++) 
    { 
        CbDrawBuffer_Draw(stdOut, "-"); 
    }
}

void CbRenderer_StopDrawing(HANDLE stdOut) 
{ 
    CbDrawBuffer_FlushBuffer(stdOut);
}