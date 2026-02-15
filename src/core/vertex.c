#include "headers/vertex.h"

BOOL validateInputVertex(const Vertex *vertex)
{
    return vertex->X >= 0 
        && vertex->Y >= 0 
        && vertex->X <= MAX_INPUT_COORDINATE
        && vertex->Y <= MAX_INPUT_COORDINATE;
}