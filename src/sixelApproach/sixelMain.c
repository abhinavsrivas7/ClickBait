#include "sixel.h"
#define CLICKBAIT 46
#define CIRCLE_RADIUS 100
#define BOX_SIDE 500
#define BOX_HALF_SIDE (BOX_SIDE / 2)
int leftClickCounter = 0, rightClickCounter = 0;
Bool filledState = true;

void IncrementCounter(const CbEnvironment *environment, Bool isLeft, Bool isRight)
{
    if (isLeft)  { ++leftClickCounter;  }
    if (isRight) { ++rightClickCounter; }

    CbRenderer_NavigateToCell(environment->Out, (COORD) { .X = 1, .Y = 2 });
    CbDrawBuffer_Draw(environment->Out, "Left Click Counter Value: %d.\n", leftClickCounter);
    CbDrawBuffer_Draw(environment->Out, "Right Click Counter Value: %d.\n", rightClickCounter);
    CbDrawBuffer_Draw(environment->Out, "Click inside the box to increment..\n");
}

static Bool IsCbExitEvent(const INPUT_RECORD *event)
{
    return event->EventType == KEY_EVENT
        && event->Event.KeyEvent.bKeyDown 
        && event->Event.KeyEvent.uChar.AsciiChar == QUIT_COMMAND;
}

static void DrawSixelBox(const CbEnvironment *environment, COORD center)
{
    COORD start = { .X = center.X - BOX_HALF_SIDE, .Y = center.Y - BOX_HALF_SIDE };
    COORD offsets = CbRenderer_StartDrawing(environment->Out, &environment->Screen, start);
    SHORT strips = BOX_SIDE / SIXEL_HEIGHT;
    CbRenderer_SetColor(environment->Out, RED, 1);
    CbRenderer_SetColor(environment->Out, BLUE, 2);

    for (SHORT strip = 0; strip <= strips; strip++) 
    {
        Bool isEdgeStrip = strip == 0 || strip == strips;

        for (SHORT col = 0; col < BOX_SIDE; col++) 
        {
            BYTE column[8] = {0};
            Bool isEdgeColumn = col == 0 || col == (BOX_SIDE - 1);
            CbRenderer_UseColor(environment->Out, col >= BOX_HALF_SIDE ? 2 : 1);

            for(BYTE bit = 0; bit < SIXEL_HEIGHT; bit++)
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

            CbRenderer_DrawColumn(environment->Out, column);
        }

        CbRenderer_MoveDownByStrips(environment->Out, 1);
        CbRenderer_MoveRightByColumns(environment->Out, offsets.X);
    }

    filledState = !filledState;
    
    CbRenderer_NavigateToCell(environment->Out, CbScreen_PixelToCell(
        &environment->Screen, 
        (COORD) { .X = center.X - CLICKBAIT, .Y = center.Y }
    ));

    CbDrawBuffer_Draw(environment->Out, "CLICK-BAIT");
    CbRenderer_StopDrawing(environment->Out);
}

// static void DrawSixelCircle(const CbEnvironment *environment, COORD center)
// {
//     COORD start = { .X = center.X + CIRCLE_RADIUS, .Y = center.Y };
//     COORD offsets = CbRenderer_StartDrawing(environment->Out, &environment->Screen, start);
// }

static void HandleMouseEvent(
    const MOUSE_EVENT_RECORD *mouseEvent, 
    const CbEnvironment *environment,
    const COORD center
)
{   
    SMALL_RECT boxVertices = {
        .Top = center.Y - BOX_HALF_SIDE,
        .Bottom = center.Y + BOX_HALF_SIDE,
        .Left = center.X - BOX_HALF_SIDE,
        .Right = center.X + BOX_HALF_SIDE
    };

    COORD boxTopLeftCell = CbScreen_PixelToCell(&environment->Screen, (COORD) {
        .X = boxVertices.Left - 1, .Y = boxVertices.Top - 1
    });

    COORD boxBottomRightCell = CbScreen_PixelToCell(&environment->Screen, (COORD) {
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
        CbEnvironment_ClearScreen(environment->Out);
        IncrementCounter(environment, leftButtonPressed, rightButtonPressed);
        DrawSixelBox(environment, center);
    }
}

int CbInit()
{
    DWORD read;
    INPUT_RECORD event;
    CbEnvironment *environment = CbEnvironment_Prepare();
    COORD center = CbScreen_FindCenter(&environment->Screen);
    DrawSixelBox(environment, center);
    IncrementCounter(environment, false, false);

    while(true)
    {
        ReadConsoleInputW(environment->In, &event, 1, &read); 

        if(IsCbExitEvent(&event))
        {
            CbEnvironment_Cleanup(environment);
            return 0;
        }
        else if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            CbEnvironment_ClearScreen(environment->Out);
            CbScreen_Update(&environment->Screen, environment->In, environment->Out);
            center = CbScreen_FindCenter(&environment->Screen);
            DrawSixelBox(environment, center);
            IncrementCounter(environment, false, false);
        }
        else if(event.EventType == MOUSE_EVENT)
        {
            HandleMouseEvent(&event.Event.MouseEvent, environment, center);   
        }
    }
}