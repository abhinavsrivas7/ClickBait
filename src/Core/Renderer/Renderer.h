#pragma once
#include "../Context/Context.h" 
#include "Bitmap.h"

typedef struct { Point A; Point B; } LineSegment;

Bool Renderer_ClipLine(const Context *context, LineSegment *line);
void Renderer_DrawPoint(Context *context, const Point point, const Color color);
void Renderer_DrawLine(Context *context, LineSegment line, const Color color);
void Renderer_RenderFrame(Context *context);
void Renderer_NavigateToCell(Context *context, const Point cell);
void Renderer_DrawImage(Context *context,  const Point center);