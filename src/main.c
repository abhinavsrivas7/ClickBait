#include "platform/headers/setup.h"
#include "platform/headers/eventHandlers.h"
#include "core/headers/renderer.h"

int main()
{
    STD std = getHandles();
    COORD screenSize = prepare(&std);
    Box box = {0};
    UINT a = 10, b = 20;
    Color sideColors[4] = {RED, GREEN, BLUE, GREEN};
    Vertex vertices[4] = {{.X=a, .Y=a}, {.X=b, .Y=a}, {.X=b, .Y=b}, {.X=a, .Y=b}};

    if(!boxConstructor(&box, vertices, sideColors))
    {
        return 69;
    }

    renderBox(&box, std.Out, &screenSize);
    return 0;
}