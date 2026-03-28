#pragma once

#define EnterSixelMode              "\033Pq\"1;1"
#define ExitSixelMode               "\033\\"
#define MoveDownByOneStrip          '-'
#define RequestScreenSize           "\033[14t\033[18t"
#define SwitchToAlternateBuffer     "\033[?1049h"
#define SwitchToOriginalBuffer      "\033[?1049l"
#define ClearScreen                 "\x1b[0m\x1b[2J\x1b[1;1H"
#define CommandAndSize(command)     ("" command), sizeof(command)

#define MoveRightBy(columns)        ("!%d?"), (columns)
#define NavigateToCell(x, y)        ("\033[%d;%dH"), (y), (x)
#define UseColor(index)             ("#%d"), (index)
#define SetColor(index, r, g, b)    ("#%d;2;%d;%d;%d"), (index), (r), (g), (b)