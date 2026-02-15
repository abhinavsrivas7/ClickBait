#include "headers/graphics.h"
#define UTF_8_WIDTH 4

void setCell(
    const HANDLE stdOut,
    const COORD *position, 
    const Color bgColor,
    const char *character,
    const int len)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(stdOut, &info);
    printf("\x1b[%d;%dH", position->Y + 1, position->X + 1);
    printf("\x1b[48;2;%d;%d;%dm", bgColor.R, bgColor.G, bgColor.B);
    fwrite(character, 1, len, stdout);
    printf("\x1b[0m"); //Reset attributes
    fflush(stdout);
    SetConsoleCursorPosition(stdOut, info.dwCursorPosition);
}

void setCellBackground(const HANDLE stdOut, const COORD *position, const Color bgColor)
{
    int len;
    DWORD read;
    char utf8[UTF_8_WIDTH] = {0};
    WCHAR origChar;
    
    if(!ReadConsoleOutputCharacterW(stdOut, &origChar, 1, *position, &read) || read != 1)
    {
        origChar = L'\0';
        len = 1;
    }
    else
    {
        len = WideCharToMultiByte(CP_UTF8, 0, &origChar, 1, utf8, UTF_8_WIDTH, NULL, NULL);
    }
    
    setCell(stdOut, position, bgColor, utf8, len);
}

void resetCell(const HANDLE stdOut, const COORD *position)
{
    setCell(stdOut, position, (Color) { .R = 0, .G = 0, .B = 0 }, " ", 1);
}

void clearScreen(HANDLE stdOut, const COORD *screenSize)
{
    COORD topLeft = {0, 0};
    DWORD _;
    FillConsoleOutputCharacterW(stdOut, L' ', screenSize->X * screenSize->Y, topLeft, &_);
    SetConsoleCursorPosition(stdOut, topLeft);
}
