#pragma once
#include "include.h"
#include "../../headers/colors.h"

void setCellBackground(
    const HANDLE stdOut, const COORD *position, const Color bgColor
);

void resetCell(const HANDLE stdOut, const COORD *position);
void clearScreen(HANDLE stdOut, const COORD *screenSize);