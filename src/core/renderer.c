#include "headers/renderer.h"
#include "../platform/headers/graphics.h"

COORD mapVertex(const Vertex *vertex, const COORD *screenSize)
{
    return (COORD) {
        .X = floor(((double)vertex->X / MAX_INPUT_COORDINATE) * screenSize->X),
        .Y = floor(((double)vertex->Y / MAX_INPUT_COORDINATE) * screenSize->Y)
    };
}

void renderLine(
    const Vertex vertices[static 2], 
    const Color color, 
    const HANDLE stdOut,
    const COORD *screenSize)
{
    if((vertices[0].X != vertices[1].X) && (vertices[0].Y != vertices[1].Y))
    {
        return;
    }

    COORD line[2] = { 
        mapVertex(&vertices[0], screenSize), mapVertex(&vertices[1], screenSize) 
    };

    SHORT x1 = min(line[0].X, line[1].X), x2 = max(line[0].X, line[1].X);
    SHORT y1 = min(line[0].Y, line[1].Y), y2 = max(line[0].Y, line[1].Y);

    for (SHORT x = x1; x <= x2; x++) {
        for (SHORT y = y1; y <= y2; y++) {
            COORD current = {x, y};
            setCellBackground(stdOut, &current, color);
        }
    }
}

void renderBox(const Box *box, const HANDLE stdOut, COORD *screenSize)
{
    if(box == NULL)
    {
        return;
    }

    Vertex line[2];

    for(Bool i = 0; i < 4; i++)
    {
        line[0] = box->Vertices[i];
        line[1] = box->Vertices[(i + 1) % 4];
        renderLine(line, box->SideColors[i], stdOut, screenSize);
    }
}

void renderContainerBox(const ContainerBox *containerBox, const HANDLE stdOut, COORD *screenSize)
{
    if(containerBox == NULL)
    {
        return;
    }

    renderBox(&containerBox->box, stdOut, screenSize);

    for(Bool i = 0; i < containerBox->childrenCount; i++)
    {
        renderContainerBox(containerBox->children[i], stdOut, screenSize);
    }
}

void renderListBox(const ListBox *listBox, const HANDLE stdOut, COORD *screenSize)
{
    if(listBox == NULL)
    {
        return;
    }

    renderBox(&listBox->box, stdOut, screenSize);

    for(UINT i = 0; i < listBox->childrenCount; i++)
    {
        renderBox(&listBox->children[i], stdOut, screenSize);
    }
}