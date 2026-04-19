#include "Renderer.h"
#include <math.h>

static void Renderer_DrawLineH(
    Context *context, LineSegment line, const Channel colorIndex)
{
    if(line.A.X == line.B.X) { return; }
    short start = min(line.A.X, line.B.X), end = max(line.A.X, line.B.X);
    
    for(short i = start; i <= end; i++) 
    { 
        context->DrawBuffer.Frame[line.A.Y][i] = colorIndex; 
    }
}

static void Renderer_DrawLineV(
    Context *context, LineSegment line, const Channel colorIndex)
{
    if(line.A.Y == line.B.Y) { return; }
    short start = min(line.A.Y, line.B.Y), end = max(line.A.Y, line.B.Y);
    
    for(short i = start; i <= end; i++) 
    { 
        context->DrawBuffer.Frame[i][line.A.X] = colorIndex; 
    }
}

//Clickbait's implementation of BRESENHAM aliased line drawing algorithm.
static void Renderer_DrawLineD(
    Context *context, LineSegment line, const Channel colorIndex)
{
    Point step =  { 
        .X = line.A.X < line.B.X ? 1 : -1, 
        .Y = line.A.Y < line.B.Y ? 1 : -1 
    };

    Point delta = { 
        .X = abs(line.A.X - line.B.X), 
        .Y = -abs(line.A.Y - line.B.Y)    
    };
    
    short error = delta.X + delta.Y, decision;

    while(true)
    {
        context->DrawBuffer.Frame[line.A.Y][line.A.X] = colorIndex;
        decision = 2 * error;

        if(line.A.X == line.B.X && line.A.Y == line.B.Y) { break; }
        if(decision >= delta.Y) { error += delta.Y; line.A.X += step.X; }
        if(decision <= delta.X) { error += delta.X; line.A.Y += step.Y; }
    }
}

void Renderer_DrawPoint(Context *context, const Point point, const Color color)
{
    if(point.X >= 0 && point.X < context->Platform.ScreenSize.Pixels.X
    && point.Y >= 0 && point.Y < context->Platform.ScreenSize.Pixels.Y)
    {
        context->DrawBuffer.Frame[point.Y][point.X] = Color_GetPaletteIndex(color); 
    }
}

void Renderer_DrawLine(Context *context, LineSegment line, const Color color)
{
    Channel colorIndex = Color_GetPaletteIndex(color);

    if(!Renderer_ClipLine(context, &line)) { return; }
    
    if((line.A.X == line.B.X) && (line.A.Y == line.B.Y)) 
    {
        Renderer_DrawPoint(context, line.A, color);
    }
    else if(line.A.X == line.B.X) 
    { 
        Renderer_DrawLineV(context, line, colorIndex); 
    }
    else if(line.A.Y == line.B.Y) 
    { 
        Renderer_DrawLineH(context, line, colorIndex); 
    }
    else 
    { 
        Renderer_DrawLineD(context, line, colorIndex); 
    }
}

void Renderer_NavigateToCell(Context *context, const Point cell)
{
    CommandBuffer_SetMode(
        &context->CommandBuffer, &context->Platform, COMMAND_BUFFER_MODE_ANSI
    );
    
    CommandBuffer_AppendFS(
        &context->CommandBuffer, &context->Platform, NavigateToCell(cell.X, cell.Y)
    );
}