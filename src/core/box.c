#include "headers/box.h"

Bool boxConstructor(Box *box, Vertex vertices[4], Color sideColors[4])
{
    for(int i = 0; i < 4; i++)
    {
        if(!validateInputVertex(&vertices[i]))
        {
            return false;
        }

        box->Vertices[i] = vertices[i];
        box->SideColors[i] = sideColors[i];
    }

    return true;
}

ContainerBox* containerBoxConstructor(Vertex vertices[4], Color sideColors[4])
{
    Box box = {0};

    if(!boxConstructor(&box, vertices, sideColors))
    {
        return NULL;
    }

    ContainerBox *containerBox = malloc(sizeof(ContainerBox));

    if(containerBox == NULL)
    {
        return NULL;
    }

    containerBox->box = box;
    containerBox->childrenCount = 0;
    return containerBox;
}

Bool addChild(ContainerBox* containerBox, Vertex vertices[4], Color sideColors[4])
{
    if(containerBox->childrenCount >= MAX_CHILD_CONTAINERS)
    {
        return false;
    }

    ContainerBox* child = containerBoxConstructor(vertices, sideColors);

    if(child == NULL)
    {
        return false;
    }

    containerBox->children[containerBox->childrenCount++] = child;
    return true;
}

ListBox* listBoxConstructor(Vertex vertices[4], Color sideColors[4], unsigned int childrenCount)
{
    Box box = {0};

    if(!boxConstructor(&box, vertices, sideColors))
    {
        return NULL;
    }

    ListBox *listBox = malloc(sizeof(ListBox) + (childrenCount * sizeof(Box)));

    if(listBox == NULL)
    {
        return NULL;
    }

    listBox->box = box;
    listBox->childrenCount = childrenCount;

    for(int i = 0; i < childrenCount; i++)
    {
        listBox->children[i] = (Box){0};
    }

    return listBox;
}

void containerBoxDestructor(ContainerBox *containerBox)
{
    if(containerBox == NULL)
    {
        return;
    }

    for(int i = 0; i < containerBox->childrenCount; i++)
    {
        containerBoxDestructor(containerBox->children[i]);
    }

    free(containerBox);
}

void listBoxDestructor(ListBox *listBox)
{
    free(listBox);
}