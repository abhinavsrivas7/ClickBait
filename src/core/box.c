#include "headers/box.h"

Box* boxConstructor(Vertex vertices[4], Color sideColors[4])
{
    Box *box = malloc(sizeof(Box));
    
    if(box == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < 4; i++)
    {
        if(!validateInputVertex(&vertices[i]))
        {
            free(box);
            return NULL;
        }

        box->Vertices[i] = vertices[i];
        box->SideColors[i] = sideColors[i];
    }

    return box;
}

ContainerBox* containerBoxConstructor(Vertex vertices[4], Color sideColors[4])
{
    Box *box = boxConstructor(vertices, sideColors);

    if(box == NULL)
    {
        return NULL;
    }

    ContainerBox *containerBox = malloc(sizeof(ContainerBox));

    if(containerBox == NULL)
    {
        free(box);
        return NULL;
    }

    containerBox->box = box;
    containerBox->childrenCount = 0;
    return containerBox;
}

ListBox* listBoxConstructor(
    Vertex vertices[4], Color sideColors[4], unsigned int childrenCount)
{
    Box *box = boxConstructor(vertices, sideColors);

    if(box == NULL)
    {
        return NULL;
    }

    ListBox *listBox = malloc(sizeof(ListBox) + (childrenCount * sizeof(Box)));

    if(listBox == NULL)
    {
        free(box);
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
void containerBoxDestructor(ContainerBox *containerBox);
void listBoxDestructor(ListBox *containerBox);