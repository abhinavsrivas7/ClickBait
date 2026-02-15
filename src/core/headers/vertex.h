#pragma once
#include "../../headers/bool.h"
#include "../../headers/common.h"
#define MAX_INPUT_COORDINATE 100

typedef struct _Vertex {
    unsigned int X;
    unsigned int Y;
} Vertex;


Bool validateInputVertex(const Vertex *vertex);