#include "Examples.h"

#define CLICKBAIT 46
#define CIRCLE_RADIUS 100
#define BOX_SIDE 504
#define BOX_HALF_SIDE (BOX_SIDE / 2)
Bool filledState = true;
Color orange = Color_FromShades(SHADE_FULL, SHADE_MEDIUM, SHADE_OFF);
Color green = Color_Green(SHADE_MEDIUM); 

void DrawBox(Context *context, Point center)
{
    Point 
        topLeft =     { .X = center.X - BOX_HALF_SIDE, .Y = center.Y - BOX_HALF_SIDE },
        topRight =    { .X = center.X + BOX_HALF_SIDE, .Y = center.Y - BOX_HALF_SIDE },
        bottomLeft =  { .X = center.X - BOX_HALF_SIDE, .Y = center.Y + BOX_HALF_SIDE },
        bottomRight = { .X = center.X + BOX_HALF_SIDE, .Y = center.Y + BOX_HALF_SIDE };

    if(filledState)
    {    
        for (short col = topLeft.X; col <= topRight.X; col++)
        {
            Renderer_DrawLine(context, (LineSegment) {
                .A = { .X = col, .Y = topLeft.Y    },
                .B = { .X = col, .Y = bottomLeft.Y }
            }, col >= center.X ? green : orange);
        }
    }
    else
    {
        Renderer_DrawLine(context, (LineSegment) {
            .A = topLeft, .B = bottomLeft
        }, orange);

        Renderer_DrawLine(context, (LineSegment) {
            .A = topRight, .B = bottomRight
        }, green);

        Renderer_DrawLine(context, (LineSegment) {
            .A = topLeft, .B = { .X = center.X, .Y = topLeft.Y }
        }, orange);

        Renderer_DrawLine(context, (LineSegment) {
            .A = { .X = center.X + 1, .Y = topRight.Y }, .B = topRight
        }, green);

        Renderer_DrawLine(context, (LineSegment) {
            .A = bottomLeft, .B = { .X = center.X, .Y = bottomLeft.Y }
        }, orange);

        Renderer_DrawLine(context, (LineSegment) {
            .A = { .X = center.X + 1, .Y = bottomRight.Y }, .B = bottomRight
        }, green);
    }

    Renderer_DrawLine(context, (LineSegment) { 
        .A = topLeft, .B = bottomRight 
    }, Color_Blue(SHADE_DARK));

    Renderer_DrawLine(context, (LineSegment) { 
        .A = topRight, .B = bottomLeft 
    }, Color_Blue(SHADE_DARK));

    Renderer_RenderFrame(context);
    filledState = !filledState;
    Point clickbaitPixel = { .X = center.X - CLICKBAIT, .Y = center.Y };
    Point clickbaitCell = Platform_PixelToCell(&context->Platform, clickbaitPixel);
    Renderer_NavigateToCell(context, clickbaitCell);
    CommandBuffer_AppendFS(&context->CommandBuffer, &context->Platform, "CLICK-BAIT");
    CommandBuffer_Flush(&context->CommandBuffer, &context->Platform);
}