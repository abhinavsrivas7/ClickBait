#pragma once
#include "vertex.h"
#include "../../headers/colors.h"
#define MAX_CHILD_CONTAINERS 50

typedef struct _Box { Vertex Vertices[4]; Color SideColors[4]; } Box;
typedef struct _ContainerBox ContainerBox;

struct _ContainerBox { 
    Box *box;
    ContainerBox *children[MAX_CHILD_CONTAINERS];
    unsigned int childrenCount;
};

typedef struct _ListBox {
    Box *box;
    Box *children;
    unsigned int childrenCount;
} ListBox;

Box* boxConstructor(Vertex vertices[4], Color sideColors[4]);
ContainerBox* containerBoxConstructor(Vertex vertices[4], Color sideColors[4]);

ListBox* listBoxConstructor(
    Vertex vertices[4], Color sideColors[4], unsigned int childrenCount
);

void containerBoxDestructor(ContainerBox *containerBox);
void listBoxDestructor(ListBox *containerBox);