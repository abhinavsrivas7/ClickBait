#include "Examples.h"

int leftClickCounter = 0, rightClickCounter = 0;
const Point counterCoordinates = { .X = 1, .Y = 2 };

static void IncrementCounter(Context *context, EventTypes eventType)
{
    if (eventType == PLATFORM_LEFT_MOUSE_CLICKED_EVENT)  { ++leftClickCounter;  }
    if (eventType == PLATFORM_RIGHT_MOUSE_CLICKED_EVENT) { ++rightClickCounter; }

    Renderer_NavigateToCell(context, counterCoordinates);
    
    CommandBuffer_AppendFS(
        &context->CommandBuffer,
        &context->Platform,
        "Left Click Counter Value: %d.\n", 
        leftClickCounter
    );

    CommandBuffer_AppendFS(
        &context->CommandBuffer,
        &context->Platform,
        "Right Click Counter Value: %d.\n", 
        rightClickCounter
    );

    CommandBuffer_AppendFS(
        &context->CommandBuffer,
        &context->Platform,
        "Click inside the box to increment..\n"
    );

    CommandBuffer_Flush(&context->CommandBuffer, &context->Platform);
}

static void HandleMouseEvent(const Event *mouseEvent, Context *context, const Point center)
{   
    AABB boxPixel = {
        .TopLeft = {
            .X = (center.X - BOX_HALF_SIDE) - 1, .Y = (center.Y - BOX_HALF_SIDE) - 1
        },
        .Dimensions = {
            .X = (center.X + BOX_HALF_SIDE) - 1, .Y = (center.Y + BOX_HALF_SIDE) - 1
        }
    };

    AABB boxCell = {
        .TopLeft = Platform_PixelToCell(&context->Platform, boxPixel.TopLeft),
        .Dimensions = Platform_PixelToCell(&context->Platform, boxPixel.Dimensions)
    };

    Bool isInBox = mouseEvent->Position.X >= boxCell.TopLeft.X
        && mouseEvent->Position.X <  boxCell.Dimensions.X
        && mouseEvent->Position.Y >= boxCell.TopLeft.Y
        && mouseEvent->Position.Y <  boxCell.Dimensions.Y;
    
    if(isInBox)
    {
        Platform_ClearScreen(&context->Platform);
        ExecuteExample(context, center);
        IncrementCounter(context, mouseEvent->Event);
    }
}

int EventLoop()
{
    Event event;
    Context *context = Context_Startup();
    Point center = Platform_FindScreenCenter(&context->Platform);
    ExecuteExample(context, center);
    IncrementCounter(context, PLATFORM_NO_EVENT);

    while(true)
    {
        Platform_PollForEvents(&context->Platform, &event);

        if(event.Event == PLATFORM_EXIT_EVENT)
        {
            Context_Shutdown(context);
            return 0;
        }
        else if(event.Event == PLATFORM_WINDOW_RESIZED_EVENT)
        {
            Platform_ClearScreen(&context->Platform);
            Platform_UpdateScreenSize(&context->Platform);
            center = Platform_FindScreenCenter(&context->Platform);
            ExecuteExample(context, center);
            IncrementCounter(context, PLATFORM_WINDOW_RESIZED_EVENT);
        }
        else if(Platform_IsMouseClickedEvent(&event))
        {
            HandleMouseEvent(&event, context, center);   
        }
    }
}