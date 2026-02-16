#pragma once
#include "../../headers/colors.h"
#include "../../headers/bool.h"
#include "../../headers/common.h"
#define MAX_INPUT_COORDINATE 100
#define MAX_CHILD_CONTAINERS 50

typedef unsigned int UINT;
typedef struct _Vertex { UINT X; UINT Y; } Vertex;

typedef struct _Box { Vertex Vertices[4]; Color SideColors[4]; } Box;
Bool boxConstructor(Box *box, Vertex vertices[4], Color sideColors[4]);

typedef struct _ListBox { Box box; UINT childrenCount; Box children[]; } ListBox;
ListBox* listBoxConstructor(Vertex vertices[4], Color sideColors[4], UINT childrenCount);
void listBoxDestructor(ListBox *listBox);

typedef struct _ContainerBox ContainerBox;

struct _ContainerBox { 
    Box box;
    Bool childrenCount;
    ContainerBox *children[MAX_CHILD_CONTAINERS];
};

ContainerBox* containerBoxConstructor(Vertex vertices[4], Color sideColors[4]);
Bool addChild(ContainerBox* containerBox, Vertex vertices[4], Color sideColors[4]);
void containerBoxDestructor(ContainerBox *containerBox);
