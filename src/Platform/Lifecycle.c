#include "Platform.h"

static void Platform_ChangeIoModes(const Platform *that, const Bool isStartup)
{
    Word inMode = that->InMode, outMode = that->OutMode;
    
    if(isStartup)
    {
        inMode |= ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS;
        
        inMode &= ~(
            ENABLE_QUICK_EDIT_MODE | 
            ENABLE_LINE_INPUT      | 
            ENABLE_ECHO_INPUT      |
            ENABLE_VIRTUAL_TERMINAL_INPUT
        );

        outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    }
    
    SetConsoleMode(that->In, inMode);
    SetConsoleMode(that->Out, outMode);
    Platform_ToggleCursor(that, !isStartup);
    Platform_ClearScreen(that);
}

static void Platform_SwitchTerminalBuffer(const Platform *that, const Bool isStartup)
{
    Platform_DumpToConsole(that, CommandAndSize(ExitSixelMode));
    
    if(isStartup)
    {
        Platform_DumpToConsole(that, CommandAndSize(SwitchToAlternateBuffer));
    }
    else
    {
        Platform_DumpToConsole(that, CommandAndSize(SwitchToOriginalBuffer));
    }
}

void Platform_InitForStartup(Platform *that)
{
    that->In = GetStdHandle(STD_INPUT_HANDLE);
    that->Out = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(that->In, &that->InMode);
    GetConsoleMode(that->Out, &that->OutMode);
    Platform_SwitchTerminalBuffer(that, true);
    Platform_ChangeIoModes(that, true);
    Platform_UpdateScreenSize(that);
}

void Platform_RestoreForShutdown(Platform *that)
{
    Platform_ChangeIoModes(that, false);
    Platform_SwitchTerminalBuffer(that, false);
}