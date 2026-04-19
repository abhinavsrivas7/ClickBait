#pragma once
#include "../Platform/Platform.h"
#define CHANNEL_SIZE       6
#define CHANNEL_WIDTH      3
#define GAMUT_SPREAD       20
#define PALETTE_CACHE_SIZE 8192
#define CHANNEL_SQAURE     (CHANNEL_SIZE * CHANNEL_SIZE)
#define SHADE_OFF          0
#define SHADE_FAINT        1
#define SHADE_LIGHT        2
#define SHADE_MEDIUM       3
#define SHADE_DARK         4
#define SHADE_FULL         5
typedef unsigned char      Channel;

typedef struct { Channel R; Channel G; Channel B; } Color;

#define Color_FromShades(r, g, b) (Color) { \
    .R = (Channel) r, .G = (Channel) g, .B = (Channel) b  \
}

#define Color_GetPaletteIndex(color) (Channel) { \
    ((color.R * CHANNEL_SQAURE) + (color.G * CHANNEL_SIZE) + color.B)  \
}

#define Color_Black        Color_FromShades(SHADE_OFF, SHADE_OFF, SHADE_OFF  )
#define Color_Red(shade)   Color_FromShades(shade,     SHADE_OFF, SHADE_OFF  )
#define Color_Green(shade) Color_FromShades(SHADE_OFF, shade,     SHADE_OFF  )
#define Color_Blue(shade)  Color_FromShades(SHADE_OFF, SHADE_OFF, shade      )

typedef struct { size_t CacheSize; char Cache[PALETTE_CACHE_SIZE]; } PaletteCache;
void Color_InitPaletteCache(PaletteCache *that);