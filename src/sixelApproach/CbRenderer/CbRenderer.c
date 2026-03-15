#include "CbRenderer.h"

void CbRenderer_StartDrawing(const CbScreen* screenSize)
{// \033Pq (sixel graphics mode). \"1;1 (pixel aspect ratio). ;%d;%d (canvas size).
    printf("\033Pq\"1;1;%d;%d", screenSize->Pixels.X, screenSize->Pixels.Y);
}

void CbRenderer_SetColor(const Color color, SHORT colorRegister)
{// #1 (use color register number 1 [uptp 256]). ;2 (24 bit color gamut mode)
    printf("#%hd;2;%d;%d;%d", colorRegister, color.R, color.G, color.B);
}

void CbRenderer_UseColor(SHORT colorRegister)
{
    printf("#%hd", colorRegister);
}

void CbRenderer_DrawColumn(BYTE bits[8])
{
    BYTE pixels = 0;
    
    for (int i = 0; i < SIXEL_HEIGHT; i++) 
    { 
        pixels |= bits[i] << i; 
    }

    printf("%c", (char)(pixels + SIXEL_OFFSET));
}

COORD CbRenderer_NavigateToNearestPixel(
    const CbScreen *screenSize, COORD pixel, Bool exitDrawing)
{
    COORD cell = CbScreen_PixelToCell(screenSize, pixel);
    CbRenderer_NavigateToCell(cell);
    COORD cellTopLeft = CbScreen_CellToTopLeftPixel(screenSize, cell);
    COORD offsets = { .X = pixel.X - cellTopLeft.X, .Y = pixel.Y - cellTopLeft.Y };
    CbRenderer_StartDrawing(screenSize);
    CbRenderer_MoveDownByStrips(offsets.Y / SIXEL_HEIGHT);
    CbRenderer_MoveRightByColumns(offsets.X);

    if(exitDrawing) { CbRenderer_StopDrawing(); }    

    return (COORD) { .X = offsets.X, .Y = offsets.Y % 6}; 
}

void CbRenderer_NavigateToCell(COORD cell)
{
    printf("\033[%hd;%hdH", cell.Y, cell.X);
}

void CbRenderer_MoveRightByColumns(SHORT columns)
{
    if (columns > 0) { printf("!%hd?", columns); }
}

void CbRenderer_MoveDownByStrips(SHORT strips) 
{ 
    for (SHORT i = 0; i < strips; i++) { printf("-"); }
}

void CbRenderer_StopDrawing() { printf("\033\\"); }