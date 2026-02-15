#pragma once

typedef unsigned char BYTE;
typedef struct _Color { BYTE R; BYTE G; BYTE B; } Color;

#define RED (Color) { .R = 255, .G = 0, .B = 0 }
#define GREEN (Color) { .R = 0, .G = 255, .B = 0 }
#define BLUE (Color) { .R = 0, .G = 0, .B = 255 }