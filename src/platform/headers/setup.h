#pragma once
#include "include.h"
#include "eventHandlers.h"
#define QUIT_COMMAND 27

typedef struct _STD { 
    HANDLE In; 
    HANDLE Out; 
    DWORD InMode;
    DWORD OutMode;
} STD;

STD getHandles();
COORD prepare(const STD *std);

int cleanup(const STD *std, const COORD *screenSize, MouseResetEvent *residueEvent);