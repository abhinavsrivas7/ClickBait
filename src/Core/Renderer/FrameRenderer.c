#include "Renderer.h"

static Bool uniqueColors[UNIQUE_COLORS];

static void Renderer_MarkColorsUsedInStrip(
    const DrawBuffer *that, short yOffset, short screenWidth)
{
    memset(uniqueColors, false, UNIQUE_COLORS);

    for(Channel y = 0; y < SIXEL_HEIGHT; y++) 
    {
        short actualY = y + yOffset;

        for(short x = 0; x < screenWidth; x++) 
        {
            Channel pixel = that->Frame[actualY][x];
            if(pixel < UNDEFINED_PIXEL) { uniqueColors[pixel] = true; }
        }
    }
}

static void Renderer_DrawUntilPreviousSixel(
    Context *context, const Channel previous, const short rleCount)
{
    if(previous == 0)
    {
        CommandBuffer_AppendFS(
            &context->CommandBuffer, &context->Platform, MoveRightBy(rleCount)
        );
    }
    else
    {
        char command = (char)(previous + SIXEL_OFFSET);

        if(rleCount == 1)
        {
            CommandBuffer_AppendC(&context->CommandBuffer, &context->Platform, command);
        }
        else
        {
            CommandBuffer_AppendFS(
                &context->CommandBuffer, &context->Platform, RleCommand(command, rleCount)
            );
        }
    }
}

static void Renderer_RenderStrip(
    Context *context, Channel color, short yOffset, short screenWidth)
{
    CommandBuffer_AppendC(&context->CommandBuffer, &context->Platform, MoveToStripStart);
    CommandBuffer_AppendFS(&context->CommandBuffer, &context->Platform, UseColor(color));
    Channel bit = 0, sixel = 0, previousSixel = 0; short rleCount = 0;

    for(short x = 0; x < screenWidth; x++)
    {    
        sixel = 0;

        for(Channel y = 0; y < SIXEL_HEIGHT; y++)
        {
            bit = context->DrawBuffer.Frame[y + yOffset][x] == color;
            sixel |= bit << y;
        }

        if(x == 0)
        {
            previousSixel = sixel;
            rleCount = 1;
            continue;
        }

        if(sixel == previousSixel) 
        {
            rleCount++;
        }
        else 
        {
            Renderer_DrawUntilPreviousSixel(context, previousSixel, rleCount);
            previousSixel = sixel;
            rleCount = 1;
        }
    }

    Renderer_DrawUntilPreviousSixel(context, previousSixel, rleCount);
}

void Renderer_RenderFrame(Context *context)
{
    short strips =  (context->Platform.ScreenSize.Pixels.Y / SIXEL_HEIGHT);
    short screenWidth = context->Platform.ScreenSize.Pixels.X;
    
    CommandBuffer_SetMode(
        &context->CommandBuffer, &context->Platform, COMMAND_BUFFER_MODE_SIXEL
    );

    CommandBuffer_AppendS(
        &context->CommandBuffer,
        &context->Platform,
        context->PaletteCache.Cache,
        context->PaletteCache.CacheSize
    );

    for(short strip = 0; strip <= strips; strip++)
    {    
        short yOffset = strip * SIXEL_HEIGHT;
        Renderer_MarkColorsUsedInStrip(&context->DrawBuffer, yOffset, screenWidth);

        for(Channel color = 0; color < UNIQUE_COLORS; color++)
        {
            if(uniqueColors[color])
            {
                Renderer_RenderStrip(context, color, yOffset, screenWidth);
            }
        }

        CommandBuffer_AppendC(&context->CommandBuffer, &context->Platform, MoveDownByOneStrip);
    }

    DrawBuffer_Init(&context->DrawBuffer);
}