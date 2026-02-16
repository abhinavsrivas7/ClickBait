#pragma once
#include "box.h"
#include "../../platform/headers/include.h"
#include <math.h>

void renderBox(const Box *box, const HANDLE stdOut, COORD *screenSize);
void renderContainerBox(const ContainerBox *containerBox, const HANDLE stdOut, COORD *screenSize);
void renderListBox(const ListBox *listBox, const HANDLE stdOut, COORD *screenSize);