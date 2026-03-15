#include "examples.h"

int boxExampleEventLoop(const STD *std, COORD *screenSize, Box *box)
{
    DWORD read;
    INPUT_RECORD event;

    while(true)
    {
        ReadConsoleInputW(std->In, &event, 1, &read); 

        if(isExitEvent(&event))
        {
            cleanup(std);
            return 0;
        }
        else if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            handleWindowResizeEvent(&event.Event.WindowBufferSizeEvent, screenSize);
            clearScreen();
            renderBox(box, screenSize);
        }
    }
}

int boxExample()
{
    STD std = getHandles();
    COORD screenSize = prepare(&std);
    Box box = {0};
    UINT a = 10, b = 20;
    Color sideColors[4] = {RED, GREEN, BLUE, GREEN};
    COORD vertices[4] = {{.X=a, .Y=a}, {.X=b, .Y=a}, {.X=b, .Y=b}, {.X=a, .Y=b}};

    if(!boxConstructor(&box, vertices, sideColors))
    {
        return 69;
    }

    renderBox(&box, &screenSize);
    return boxExampleEventLoop(&std, &screenSize, &box);
}