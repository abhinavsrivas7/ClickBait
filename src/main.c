#include "Clickbait.h"
#define CLICKBAIT 46
#define CIRCLE_RADIUS 100
#define BOX_SIDE 500
#define BOX_HALF_SIDE (BOX_SIDE / 2)
int leftClickCounter = 0, rightClickCounter = 0;
Bool filledState = true;
const Color orange = Color_FromShades(FULL, MEDIUM, OFF);
const Color green = Color_Green(MEDIUM); 

void IncrementCounter(Context *context, Bool isLeft, Bool isRight)
{
    if (isLeft)  { ++leftClickCounter;  }
    if (isRight) { ++rightClickCounter; }

    Renderer_NavigateToCell(context, (Point) { .X = 1, .Y = 2 });
    
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
}

static Bool IsCbExitEvent(const INPUT_RECORD *event)
{
    return event->EventType == KEY_EVENT
        && event->Event.KeyEvent.bKeyDown 
        && event->Event.KeyEvent.uChar.AsciiChar == QUIT_COMMAND;
}

static void DrawSixelBox(Context *context, Point center)
{
    Point start = { .X = center.X - BOX_HALF_SIDE, .Y = center.Y - BOX_HALF_SIDE };
    Point offsets = Renderer_NavigateToNearestPixel(context, start);
    short strips = BOX_SIDE / SIXEL_HEIGHT;
    Renderer_RegisterPalette(context);

    for (short strip = 0; strip <= strips; strip++) 
    {
        Bool isEdgeStrip = strip == 0 || strip == strips;

        for (short col = 0; col < BOX_SIDE; col++) 
        {
            Channel column[8] = {0};
            Bool isEdgeColumn = col == 0 || col == (BOX_SIDE - 1);
            Color color = col >= BOX_HALF_SIDE ? orange : green;

            Renderer_UseColor(context, color);

            for(Channel bit = 0; bit < SIXEL_HEIGHT; bit++)
            {
                Bool shouldDrawRow = !isEdgeStrip
                    || (strip == 0 && bit >= offsets.Y)
                    || (strip == strips && bit < offsets.Y);

                column[bit] = filledState ? shouldDrawRow : (shouldDrawRow && isEdgeColumn);
                
                if(!filledState)
                {
                    column[bit] = ((strip == 0 && bit == offsets.Y)
                        || (strip == (strips - 1) && offsets.Y == 0 && bit == SIXEL_HEIGHT - 1)
                        || (strip == strips && bit == offsets.Y - 1))
                        ? true
                        : column[bit];    
                }
            }

            Renderer_DrawColumn(context, column);
        }

        Renderer_MoveDownByStrips(context, 1);
        Renderer_MoveRightByColumns(context, offsets.X);
    }

    filledState = !filledState;
    Point clickbaitPixel = { .X = center.X - CLICKBAIT, .Y = center.Y };
    Point clickbaitCell = Platform_PixelToCell(&context->Platform, clickbaitPixel);
    Renderer_NavigateToCell(context, clickbaitCell);
    CommandBuffer_AppendFS(&context->CommandBuffer, &context->Platform, "CLICK-BAIT");
    Renderer_StopDrawing(context);
}

static void HandleMouseEvent(
    const MOUSE_EVENT_RECORD *mouseEvent, 
    Context *context,
    const Point center
)
{   
    SMALL_RECT boxVertices = {
        .Top = center.Y - BOX_HALF_SIDE,
        .Bottom = center.Y + BOX_HALF_SIDE,
        .Left = center.X - BOX_HALF_SIDE,
        .Right = center.X + BOX_HALF_SIDE
    };

    Point boxTopLeftCell = Platform_PixelToCell(&context->Platform, (Point) {
        .X = boxVertices.Left - 1, .Y = boxVertices.Top - 1
    });

    Point boxBottomRightCell = Platform_PixelToCell(&context->Platform, (Point) {
        .X = boxVertices.Right - 1, .Y = boxVertices.Bottom - 1
    });

    Bool isInBox = mouseEvent->dwMousePosition.X >= boxTopLeftCell.X
        && mouseEvent->dwMousePosition.X <  boxBottomRightCell.X
        && mouseEvent->dwMousePosition.Y >= boxTopLeftCell.Y
        && mouseEvent->dwMousePosition.Y <  boxBottomRightCell.Y;

    Bool leftButtonPressed = mouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;
    Bool rightButtonPressed = mouseEvent->dwButtonState & RIGHTMOST_BUTTON_PRESSED;
    
    if(isInBox && (leftButtonPressed || rightButtonPressed))
    {
        Platform_ClearScreen(&context->Platform);
        IncrementCounter(context, leftButtonPressed, rightButtonPressed);
        DrawSixelBox(context, center);
    }
}

int main()
{
    DWORD read;
    INPUT_RECORD event;
    Context *context = Context_Startup();
    Point center = Platform_FindScreenCenter(&context->Platform);
    DrawSixelBox(context, center);
    IncrementCounter(context, false, false);

    while(true)
    {
        ReadConsoleInputW(context->Platform.In, &event, 1, &read); 

        if(IsCbExitEvent(&event))
        {
            Context_Shutdown(context);
            return 0;
        }
        else if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            Platform_ClearScreen(&context->Platform);
            Platform_UpdateScreenSize(&context->Platform);
            center = Platform_FindScreenCenter(&context->Platform);
            DrawSixelBox(context, center);
            IncrementCounter(context, false, false);
        }
        else if(event.EventType == MOUSE_EVENT)
        {
            HandleMouseEvent(&event.Event.MouseEvent, context, center);   
        }
    }
}
