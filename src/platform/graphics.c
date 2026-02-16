#include "headers/graphics.h"

void setPosition(const COORD *position)
{
    printf("\x1b[%d;%dH", position->Y + 1, position->X + 1);
}

void moveDown(Bool rows)
{   //move down by rows and left by 1 to be exactly below. \x1b[1D moves left
    printf("\x1b[%dB\x1b[1D", rows); 
}

void setColor(const Color *bgColor)
{
    printf("\x1b[48;2;%d;%d;%dm", bgColor->R, bgColor->G, bgColor->B);
}

void paintCell() 
{
    printf(" ");
}

void flush()
{
    fflush(stdout);
}

void clearScreen()
{
    printf("\x1b[0m");  //Reset attributes
    printf("\x1b[2J");  //Clear the entire screen buffer
    printf("\x1b[1;1H");//Move cursor to top-left
    fflush(stdout);
}
