#pragma once
#include "../common.h"
#include "../CbScreen/CbScreen.h"
#include "CbDrawBuffer.h"
#define SIXEL_HEIGHT 6
#define SIXEL_OFFSET 63 //2^6 - 1

typedef struct        {  BYTE R;   BYTE G;   BYTE B; } Color;
#define RED   (Color) { .R = 255, .G = 0,   .B = 0   }
#define GREEN (Color) { .R = 0,   .G = 255, .B = 0   }
#define BLUE  (Color) { .R = 0,   .G = 0,   .B = 255 }
#define BLACK (Color) { .R = 0,   .G = 0,   .B = 0   }

void CbRenderer_SetColor(HANDLE stdOut, const Color color, SHORT colorRegister);
void CbRenderer_UseColor(HANDLE stdOut, SHORT colorRegister);
void CbRenderer_DrawColumn(HANDLE stdOut, BYTE bits[8]);
void CbRenderer_StopDrawing(HANDLE stdOut);
void CbRenderer_MoveDownByStrips(HANDLE stdOut, SHORT strips);
void CbRenderer_MoveRightByColumns(HANDLE stdOut, SHORT columns);
void CbRenderer_NavigateToCell(HANDLE stdOut, COORD cell);
COORD CbRenderer_StartDrawing(HANDLE stdOut, const CbScreen* screenSize, COORD pixel);
