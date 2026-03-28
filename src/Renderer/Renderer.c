#include "Renderer.h"

void Renderer_RegisterPalette(Context *context)
{
    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);

    CommandBuffer_AppendS(
        &context->CommandBuffer,
        &context->Platform,
        context->PaletteCache.Cache,
        context->PaletteCache.CacheSize
    );
}

void  Renderer_UseColor(Context *context, const Color color)
{
    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);
    Channel index = Color_GetPaletteIndex(color);
    CommandBuffer_AppendFS(&context->CommandBuffer, &context->Platform, UseColor(index));
}

void Renderer_DrawColumn(Context *context, const Channel column[8])
{
    Channel pixels = 0;

    for (int i = 0; i < SIXEL_HEIGHT; i++) { pixels |= column[i] << i; }

    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);

    CommandBuffer_AppendC(
        &context->CommandBuffer, &context->Platform, (char)(pixels + SIXEL_OFFSET)
    );
}

void Renderer_StopDrawing(Context *context)
{
    CommandBuffer_Flush(&context->CommandBuffer, &context->Platform);
}

void Renderer_MoveDownByStrips(Context *context, const short strips)
{
    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);
    
    for (SHORT i = 0; i < strips; i++) 
    { 
        CommandBuffer_AppendC(
            &context->CommandBuffer, &context->Platform, MoveDownByOneStrip
        );
    }
}

void Renderer_MoveRightByColumns(Context *context, const short columns)
{
    if (columns > 0) 
    { 
        CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);
    
        CommandBuffer_AppendFS(
            &context->CommandBuffer, &context->Platform, MoveRightBy(columns)
        ); 
    }
}

void Renderer_NavigateToCell(Context *context, const Point cell)
{
    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, ANSI);
    
    CommandBuffer_AppendFS(
        &context->CommandBuffer, &context->Platform, NavigateToCell(cell.X, cell.Y)
    );
}

Point Renderer_NavigateToNearestPixel(Context *context, const Point pixel)
{
    Point cell = Platform_PixelToCell(&context->Platform, pixel);
    Renderer_NavigateToCell(context, cell);
    Point cellTopLeft = Platform_CellToTopLeftPixel(&context->Platform, cell);
    Point offsets = { .X = pixel.X - cellTopLeft.X, .Y = pixel.Y - cellTopLeft.Y };
    CommandBuffer_SetMode(&context->CommandBuffer, &context->Platform, SIXEL);
    Renderer_MoveDownByStrips(context, offsets.Y / SIXEL_HEIGHT);
    Renderer_MoveRightByColumns(context, offsets.X);
    return (Point) { .X = offsets.X, .Y = offsets.Y % 6 };
}