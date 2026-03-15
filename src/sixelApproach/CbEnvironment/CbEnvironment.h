#pragma once
#include "../common.h"
#include "../CbScreen/CbScreen.h"

typedef struct {
    HANDLE In; 
    HANDLE Out; 
    DWORD InMode;
    DWORD OutMode;
    CbScreen Screen;
} CbEnvironment;

CbEnvironment *CbEnvironment_Prepare();
void CbEnvironment_Cleanup(CbEnvironment *std);
void CbEnvironment_ClearScreen();