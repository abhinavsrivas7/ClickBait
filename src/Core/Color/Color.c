#include "Color.h"

void Color_InitPaletteCache(PaletteCache *that)
{
    that->CacheSize = 0; Channel index = 0;

    for(Channel r = 0; r < CHANNEL_SIZE; r++)
    {
        for(Channel g = 0; g < CHANNEL_SIZE; g++)
        {
            for(Channel b = 0; b < CHANNEL_SIZE; b++, index++)
            {   
                that->CacheSize += snprintf(
                    that->Cache + that->CacheSize, 
                    PALETTE_CACHE_SIZE - that->CacheSize, 
                    SetColor(index, r * GAMUT_SPREAD, g * GAMUT_SPREAD, b * GAMUT_SPREAD)
                );
            }
        }
    }
}
