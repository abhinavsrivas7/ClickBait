#include "Platform.h"

void Platform_DumpToConsole(const Platform *that, const char *buf, const  size_t size)
{
    Word written;
    WriteConsoleA(that->Out, buf, (Word) size, &written, NULL);
}

void Platform_ToggleCursor(const Platform *that, const Bool enable)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(that->Out, &cursorInfo);
    cursorInfo.bVisible = enable;
    SetConsoleCursorInfo(that->Out, &cursorInfo);
}

void Platform_ClearScreen(const Platform *that)
{
    Platform_DumpToConsole(that, CommandAndSize(ExitSixelMode));
    Platform_DumpToConsole(that, CommandAndSize(ClearScreen));
}