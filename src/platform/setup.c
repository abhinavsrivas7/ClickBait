#include "headers/setup.h"
#include "headers/graphics.h"

STD getHandles()
{
    DWORD inMode, outMode;
    const HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    const HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(in, &inMode);
    GetConsoleMode(out, &outMode);
	return (STD) { .In = in, .Out = out, .InMode = inMode, .OutMode = outMode };
}

COORD prepare(const STD *std)
{
    DWORD inMode = std->InMode;
    inMode |= ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT | ENABLE_EXTENDED_FLAGS;
    inMode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(std->In, inMode);
    SetConsoleMode(std->Out, std->OutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(std->Out, &info);
    
    COORD screenSize = {
        .X = info.srWindow.Right - info.srWindow.Left + 1, 
        .Y = info.srWindow.Bottom - info.srWindow.Top + 1
    };

    clearScreen(std->Out, &screenSize);
    return screenSize;
}

int cleanup(const STD *std, const COORD *screenSize, MouseResetEvent *residueEvent)
{
    SetConsoleMode(std->In, std->InMode);
    SetConsoleMode(std->Out, std->OutMode);

    if(residueEvent->assigned)
    {
        resetCell(std->Out, &residueEvent->existing.dwMousePosition);
    }

    clearScreen(std->Out, screenSize);
    return 0;
}