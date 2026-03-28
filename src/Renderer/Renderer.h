#pragma once
#include "../Context/Context.h"
#define SIXEL_HEIGHT 6
#define SIXEL_OFFSET 63 //2^6 - 1

void  Renderer_RegisterPalette(Context *context);
void  Renderer_UseColor(Context *context, const Color color);
void  Renderer_DrawColumn(Context *context, const Channel column[8]);
void  Renderer_StopDrawing(Context *context);
void  Renderer_MoveDownByStrips(Context *context, const short strips);
void  Renderer_MoveRightByColumns(Context *context, const short columns);
void  Renderer_NavigateToCell(Context *context, const Point cell);
Point Renderer_NavigateToNearestPixel(Context *context, const Point pixel);