#pragma once
#include "box.h"
#include <math.h>

void renderBox(const Box *box, const COORD *screenSize);
void renderListBox(const ListBox *listBox, const COORD *screenSize);
void renderContainerBox(const ContainerBox *containerBox, const COORD *screenSize);
