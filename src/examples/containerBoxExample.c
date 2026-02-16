#include "examples.h"

int containerBoxEventLoop(const STD *std, COORD *screenSize, ContainerBox *containerBox)
{
    DWORD read;
    INPUT_RECORD event;

    while(true)
    {
        ReadConsoleInputW(std->In, &event, 1, &read); 

        if(isExitEvent(&event))
        {
            containerBoxDestructor(containerBox);
            cleanup(std);
            return 0;
        }
        else if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            handleWindowResizeEvent(&event.Event.WindowBufferSizeEvent, screenSize);
            clearScreen();
            renderContainerBox(containerBox, screenSize);
        }
    }
}

int containerBoxExample()
{
    STD std = getHandles();
    COORD screenSize = prepare(&std);
    SHORT a = 10, b = 60, diff = 15;
    Color sideColors[4] = {RED, GREEN, BLUE, GREEN};
    COORD vertices[4] = {{.X=a, .Y=a}, {.X=b, .Y=a}, {.X=b, .Y=b}, {.X=a, .Y=b}};
    ContainerBox *containerBox = containerBoxConstructor(vertices, sideColors);
    
    if(containerBox == NULL)
    {
        return 69;
    }

    for(Bool i = 0; i < 4; i++)
    {
        vertices[i].X = vertices[i].X == a ? a + diff : b - diff;
        vertices[i].Y = vertices[i].Y == a ? a + diff : b - diff;
    }

    if(!addChild(containerBox, vertices, sideColors))
    {
        return 69;
    }

    renderContainerBox(containerBox, &screenSize);
    return containerBoxEventLoop(&std, &screenSize, containerBox);
}