#pragma once
#include "../Platform/Platform.h"
#define CHANNEL_SIZE        6
#define CHANNEL_WIDTH       3
#define GAMUT_SPREAD        20
typedef unsigned char       Channel;
#define PALETTE_CACHE_SIZE  8192

typedef struct { 
    Channel R; Channel G; Channel B; 
    Channel _padding; //for 4 byte boundary
} Color;

typedef struct { 
    size_t CacheSize; char Cache[PALETTE_CACHE_SIZE]; 
} PaletteCache;

typedef enum { 
    OFF = 0, FAINT = 1, LIGHT = 2, MEDIUM = 3, DARK = 4, FULL = 5 
} Shades;

#define Color_FromShades(r, g, b) (Color) { \
    .R = (Channel) r, .G = (Channel) g, .B = (Channel) b  \
}

#define Color_Black            Color_FromShades(OFF,   OFF,   OFF  )
#define Color_Red(shade)       Color_FromShades(shade, OFF,   OFF  )
#define Color_Green(shade)     Color_FromShades(OFF,   shade, OFF  )
#define Color_Blue(shade)      Color_FromShades(OFF,   OFF,   shade)

void    Color_InitPaletteCache(PaletteCache *that);
Channel Color_GetPaletteIndex(Color color);