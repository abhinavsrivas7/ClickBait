#include "Renderer.h"
//Clickbait's implementation of COHEN-SUTHERLAND line clipping algorithm.
#define INSIDE 0 //0000
#define LEFT   1 //0001
#define RIGHT  2 //0010
#define BOTTOM 4 //0100
#define TOP    8 //1000

static Channel Renderer_ComputeCode(const Point point, const Point screen)
{
    Channel code = INSIDE;

    if(point.X < 0)              { code |= LEFT;   }
    else if(point.X >= screen.X) { code |= RIGHT;  }
    if(point.Y < 0)              { code |= TOP;    }
    else if(point.Y >= screen.Y) { code |= BOTTOM; }
    
    return code;
}

Bool Renderer_ClipLine(const Context *context, LineSegment *line)
{
    Point intersection = {0}, delta = {0};

    short edge,
        height = context->Platform.ScreenSize.Pixels.Y - 1, 
        width  = context->Platform.ScreenSize.Pixels.X - 1;
    
    Channel outsideCode,
        codeA = Renderer_ComputeCode(line->A, context->Platform.ScreenSize.Pixels),
        codeB = Renderer_ComputeCode(line->B, context->Platform.ScreenSize.Pixels);

    while(true)
    {
        if(!(codeA | codeB)) { return true;  }
        if(codeA & codeB)    { return false; } 
        
        outsideCode = codeA ? codeA : codeB;
        delta.X = line->B.X - line->A.X; 
        delta.Y = line->B.Y - line->A.Y; 
        
        if(outsideCode & (TOP | BOTTOM))
        {
            edge = (outsideCode & TOP) ? 0 : height;
            intersection.X = line->A.X + (edge - line->A.Y) * delta.X / delta.Y;
            intersection.Y = edge;
        }
        else if(outsideCode & (LEFT | RIGHT))
        {
            edge = (outsideCode & LEFT) ? 0 : width;
            intersection.Y = line->A.Y + (edge - line->A.X) * delta.Y / delta.X;
            intersection.X = edge;
        }

        if(outsideCode == codeA) 
        {
            line->A.X = intersection.X;
            line->A.Y = intersection.Y;
            codeA = Renderer_ComputeCode(line->A, context->Platform.ScreenSize.Pixels);
        } 
        else 
        {
            line->B.X = intersection.X;
            line->B.Y = intersection.Y;
            codeB = Renderer_ComputeCode(line->B, context->Platform.ScreenSize.Pixels);
        }
    }
}
