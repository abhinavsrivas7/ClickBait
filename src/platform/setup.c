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
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(std->Out, &cursorInfo);
    cursorInfo.bVisible = false;                
    SetConsoleCursorInfo(std->Out, &cursorInfo);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(std->Out, &info);
    clearScreen();

    return (COORD) {
        .X = info.srWindow.Right - info.srWindow.Left + 1, 
        .Y = info.srWindow.Bottom - info.srWindow.Top + 1
    };
}

void cleanup(const STD *std)
{
    SetConsoleMode(std->In, std->InMode);
    SetConsoleMode(std->Out, std->OutMode);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(std->Out, &cursorInfo);
    cursorInfo.bVisible = true;                
    SetConsoleCursorInfo(std->Out, &cursorInfo);
    clearScreen();
}