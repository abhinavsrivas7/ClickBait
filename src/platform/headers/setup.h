#pragma once
#include "include.h"
#include "eventHandlers.h"

typedef struct _STD { 
    HANDLE In; 
    HANDLE Out; 
    DWORD InMode;
    DWORD OutMode;
} STD;

STD getHandles();
COORD prepare(const STD *std);
void cleanup(const STD *std);