#include "headers/renderer.h"
#include "../platform/headers/graphics.h"

COORD mapVertex(const COORD *vertex, const COORD *screenSize)
{
    return (COORD) {
        .X = floor(((double)vertex->X / MAX_INPUT_COORDINATE) * (screenSize->X - 1)),
        .Y = floor(((double)vertex->Y / MAX_INPUT_COORDINATE) * (screenSize->Y - 1))
    };
}

void renderLine(const COORD vertices[static 2], const Color *color, const COORD *screenSize)
{
    if((vertices[0].X != vertices[1].X) && (vertices[0].Y != vertices[1].Y))
    {
        return;
    }

    COORD line[2] = {mapVertex(&vertices[0], screenSize), mapVertex(&vertices[1], screenSize)};
    SHORT x1 = min(line[0].X, line[1].X), x2 = max(line[0].X, line[1].X);
    SHORT y1 = min(line[0].Y, line[1].Y), y2 = max(line[0].Y, line[1].Y);
    COORD start = { .X = x1, .Y = y1 };
    setPosition(&start);
    setColor(color);
    Bool isHorizontal = y1 == y2;

    for (SHORT x = x1; x <= x2; x++) {
        for (SHORT y = y1; y <= y2; y++) {
            paintCell();

            if(!isHorizontal)
            {
                moveDown(1);
            }
        }
    }
}

void renderBox(const Box *box, const COORD *screenSize)
{
    if(box == NULL)
    {
        return;
    }

    COORD line[2];

    for(Bool i = 0; i < 4; i++)
    {
        line[0] = box->Vertices[i];
        line[1] = box->Vertices[(i + 1) % 4];
        renderLine(line, &box->SideColors[i], screenSize);
    }

    flush();
}

void renderContainerBox(const ContainerBox *containerBox, const COORD *screenSize)
{
    if(containerBox == NULL)
    {
        return;
    }

    renderBox(&containerBox->box, screenSize);

    for(Bool i = 0; i < containerBox->childrenCount; i++)
    {
        renderContainerBox(containerBox->children[i], screenSize);
    }
}

void renderListBox(const ListBox *listBox, const COORD *screenSize)
{
    if(listBox == NULL)
    {
        return;
    }

    renderBox(&listBox->box, screenSize);

    for(UINT i = 0; i < listBox->childrenCount; i++)
    {
        renderBox(&listBox->children[i], screenSize);
    }
}
