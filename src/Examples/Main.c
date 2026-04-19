#include "Examples.h"

void ExecuteExample(Context *context, Point center)
{
    //Renderer_DrawImage(context, center);
    DrawBox(context, center);
}

int main()
{
    return EventLoop();
}