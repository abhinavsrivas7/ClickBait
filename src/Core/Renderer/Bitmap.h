#include <stdint.h>
#define MAGIC_BMP       0x4D42
#define BUCKET_COUNT    51
#define FLOOR_FACTOR    25
#define PIXEL_BIT_SIZE  24
#define PIXEL_BYTE_SIZE 3
#define UNCOMPRESSED    0
#define FILE_PATH       "assets/cb.bmp"

#pragma pack(push, 1)

typedef struct {
    uint16_t Type;        // Magic identifier: "BM" (0x4D42)
    uint32_t Size;        // File size in bytes
    uint16_t Reserved1;   // Not used
    uint16_t Reserved2;   // Not used
    uint32_t Offset;      // The exact byte index where the pixel array starts
} FileHeader;

typedef struct {
    uint32_t Size;         // Info Header size in bytes
    int32_t  Width;        // Image width
    int32_t  Height;       // Image height 
    uint16_t Planes;       // Number of color planes 
    uint16_t BitsPerPixel; // 24 for standard RGB, 32 for RGBA
    uint32_t Compression;  // 0 = Uncompressed
    uint32_t ImageSize;    // Size of the raw pixel data
    int32_t  XPixelsPerM;  // (ignore)
    int32_t  YPixelsPerM;  // (ignore)
    uint32_t ColorsUsed;   // Number of colors in palette (0 for 24-bit)
    uint32_t ColorsImp;    // (ignore)
} InfoHeader;

#pragma pack(pop)