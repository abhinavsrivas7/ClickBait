#pragma once
#include "include.h"
#include "../../headers/colors.h"

void setPosition(const COORD *position);
void moveDown(Bool rows);
void setColor(const Color *bgColor);
void paintCell();
void flush();

void setCellBackground(const COORD *position, const Color bgColor);
void clearScreen();