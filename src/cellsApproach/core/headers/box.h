#pragma once
#include "../../headers/colors.h"
#include "../../platform/headers/include.h"
#define MAX_INPUT_COORDINATE 100
#define MAX_CHILD_CONTAINERS 50

typedef unsigned int UINT;
typedef struct _Box { COORD Vertices[4]; Color SideColors[4]; } Box;
Bool boxConstructor(Box *box, COORD vertices[static 4], Color sideColors[static 4]);

typedef struct _ListBox { Box box; UINT childrenCount; Box children[]; } ListBox;

ListBox* listBoxConstructor(
    COORD vertices[static 4], Color sideColors[static 4], UINT childrenCount
);

void listBoxDestructor(ListBox *listBox);

typedef struct _ContainerBox ContainerBox;

struct _ContainerBox { 
    Box box;
    Bool childrenCount;
    ContainerBox *children[MAX_CHILD_CONTAINERS];
};

ContainerBox* containerBoxConstructor(
    COORD vertices[static 4], Color sideColors[static 4]
);

Bool addChild(
    ContainerBox* containerBox, COORD vertices[static 4], Color sideColors[static 4]
);

void containerBoxDestructor(ContainerBox *containerBox);
