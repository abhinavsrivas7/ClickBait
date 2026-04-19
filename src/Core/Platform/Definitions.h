#pragma  once
#define  WIN32_LEAN_AND_MEAN
#define  WINVER        0x0A00
#ifdef   _WIN32_WINNT
#undef   _WIN32_WINNT
#endif
#define  _WIN32_WINNT  0x0A00
#define  true          1
#define  false         0
#define  QUIT_COMMAND  27
#define  SIXEL_HEIGHT  6
#define  SIXEL_OFFSET  63 //2^6 - 1
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void          *IoHandle;
typedef unsigned char Bool;
typedef unsigned long Word;
typedef struct        { short X; short Y; } Point;
typedef struct        { float X; float Y; } FloatPoint;
typedef struct        { Point TopLeft; Point Dimensions; } AABB;

typedef struct { 
    Point Cells; Point Pixels; FloatPoint CellsPerPixel; FloatPoint PixelsPerCell;
} ScreenSize;

typedef struct { 
    IoHandle In; IoHandle Out; Word InMode; Word OutMode; ScreenSize ScreenSize;
} Platform;

typedef unsigned char                      EventTypes;
#define PLATFORM_NO_EVENT                  (EventTypes) 0
#define PLATFORM_LEFT_MOUSE_CLICKED_EVENT  (EventTypes) 1
#define PLATFORM_RIGHT_MOUSE_CLICKED_EVENT (EventTypes) 2
#define PLATFORM_MOUSE_WHEELED_UP_EVENT    (EventTypes) 3
#define PLATFORM_MOUSE_WHEELED_DOWN_EVENT  (EventTypes) 4
#define PLATFORM_KEY_PRESSED_EVENT         (EventTypes) 5
#define PLATFORM_WINDOW_RESIZED_EVENT      (EventTypes) 6
#define PLATFORM_EXIT_EVENT                (EventTypes) 7
typedef struct { EventTypes Event; char Character; Point Position; } Event;