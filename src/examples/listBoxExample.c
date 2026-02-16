#include "examples.h"

int listBoxEventLoop(const STD *std, COORD *screenSize, ListBox *listBox)
{
    DWORD read;
    INPUT_RECORD event;

    while(true)
    {
        ReadConsoleInputW(std->In, &event, 1, &read); 

        if(isExitEvent(&event))
        {
            listBoxDestructor(listBox);
            cleanup(std);
            return 0;
        }
        else if(event.EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            handleWindowResizeEvent(&event.Event.WindowBufferSizeEvent, screenSize);
            clearScreen();
            renderListBox(listBox, screenSize);
        }
    }
}

int listBoxExample()
{
    STD std = getHandles();
    COORD screenSize = prepare(&std);
    SHORT a = 10, b = 90, c = 50, diff = 7, children = 5;
    Color sideColors[4] = {RED, GREEN, BLUE, GREEN};
    COORD vertices[4] = {{.X=a, .Y=a}, {.X=a+c, .Y=a}, {.X=a+c, .Y=b}, {.X=a, .Y=b}};
    ListBox *listBox = listBoxConstructor(vertices, sideColors, children);
    
    if(listBox == NULL)
    {
        return 69;
    }

    for(Bool child = 0; child < children; child++)
    {
        for(Bool i = 0; i < 4; i++)
        {
            listBox->children[child].Vertices[i].X = vertices[i].X == a 
                ? a + diff 
                : a + c - diff;

            listBox->children[child].Vertices[i].Y = vertices[i].Y == a 
                ? a + (diff * ((child * 2) + 1)) 
                : a + (diff * ((child * 2) + 2));

            listBox->children[child].SideColors[i] = sideColors[i];
        }
    }
    
    renderListBox(listBox, &screenSize);
    return listBoxEventLoop(&std, &screenSize, listBox);
}