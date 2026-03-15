#pragma once
#include "../common.h"
#include "../CbScreen/CbScreen.h"
#define SIXEL_HEIGHT 6
#define SIXEL_OFFSET 63 //2^6 - 1

typedef struct        {  BYTE R;   BYTE G;   BYTE B; } Color;
#define RED   (Color) { .R = 255, .G = 0,   .B = 0   }
#define GREEN (Color) { .R = 0,   .G = 255, .B = 0   }
#define BLUE  (Color) { .R = 0,   .G = 0,   .B = 255 }
#define BLACK (Color) { .R = 0,   .G = 0,   .B = 0   }

void CbRenderer_StartDrawing(const CbScreen* screenSize);
void CbRenderer_SetColor(const Color color, SHORT colorRegister);
void CbRenderer_UseColor(SHORT colorRegister);
void CbRenderer_DrawColumn(BYTE bits[8]);
void CbRenderer_StopDrawing();
void CbRenderer_MoveDownByStrips(SHORT strips);
void CbRenderer_MoveRightByColumns(SHORT columns);
void CbRenderer_NavigateToCell(COORD cell);

COORD CbRenderer_NavigateToNearestPixel(
    const CbScreen* screenSize, COORD pixel, Bool exitDrawing
);
