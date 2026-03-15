#include "CbEnvironment.h"
#define CB_MODE 1
#define NORMAL_MODE 0

static void CbEnvironment_SetTerminalBuffer(HANDLE stdOut, Bool cbMode)
{
    CbDrawBuffer_SetBufferMode(stdOut, ANSI);

    if(cbMode)
    {
        char command[] = "\033[?1049h";
        CbDrawBuffer_ImmediateFlush(stdOut, command, sizeof(command) - 1); 
    }
    else
    {
        char command[] = "\033[?1049l";
        CbDrawBuffer_ImmediateFlush(stdOut, command, sizeof(command) - 1); 
    }
}

static void CbEnvironment_ToggleCursor(const CbEnvironment *environment, Bool cbMode)
{
    assert(environment != NULL);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(environment->Out, &cursorInfo);
    cursorInfo.bVisible = !cbMode;
    SetConsoleCursorInfo(environment->Out, &cursorInfo);
}

static void CbEnvironment_TweakSettings(const CbEnvironment *environment, Bool cbMode)
{
    assert(environment != NULL);
    DWORD inMode = environment->InMode, outMode = environment->OutMode;

    if(cbMode)
    {
        inMode  |= ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS;
        
        inMode  &= ~(
            ENABLE_QUICK_EDIT_MODE | 
            ENABLE_LINE_INPUT      | 
            ENABLE_ECHO_INPUT      |
            ENABLE_VIRTUAL_TERMINAL_INPUT
        );

        outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    }
    
    SetConsoleMode(environment->In, inMode);
    SetConsoleMode(environment->Out, outMode);
    CbEnvironment_ToggleCursor(environment, cbMode);
    CbEnvironment_ClearScreen(environment->Out);
}

static CbEnvironment *CbEnvironment_Init()
{
    CbEnvironment *environment = malloc(sizeof(CbEnvironment));
    assert(environment != NULL);
    environment->In = GetStdHandle(STD_INPUT_HANDLE);
    environment->Out = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(environment->In, &environment->InMode);
    GetConsoleMode(environment->Out, &environment->OutMode);
    return environment;
}

CbEnvironment *CbEnvironment_Prepare()
{
    CbEnvironment *environment = CbEnvironment_Init();
    assert(environment != NULL);
    CbDrawBuffer_Init();
    CbEnvironment_SetTerminalBuffer(environment->Out, CB_MODE);
    CbScreen_Update(&environment->Screen, environment->In, environment->Out);
    CbEnvironment_TweakSettings(environment, CB_MODE);
    return environment;
}

void CbEnvironment_ClearScreen(HANDLE stdOut)
{
    CbDrawBuffer_SetBufferMode(stdOut, ANSI);
    CbDrawBuffer_Draw(stdOut, "\x1b[0m"); //Reset attributes
    CbDrawBuffer_Draw(stdOut, "\x1b[2J"); //Clear the entire screen buffer
    CbDrawBuffer_Draw(stdOut, "\x1b[1;1H"); //Move cursor to top-left
    CbDrawBuffer_FlushBuffer(stdOut);
}

void CbEnvironment_Cleanup(CbEnvironment *environment)
{
    assert(environment != NULL);
    CbEnvironment_TweakSettings(environment, NORMAL_MODE);
    CbEnvironment_SetTerminalBuffer(environment->Out, NORMAL_MODE);
    free(environment);
    CbDrawBuffer_Free();
}