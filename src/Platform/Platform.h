#pragma  once
#include "Definitions.h"
#include "Commands.h"

// LIFECYCLE
void  Platform_InitForStartup(Platform *that);
void  Platform_RestoreForShutdown(Platform *that);

// WRAPPERS
void  Platform_ToggleCursor(const Platform *that, const Bool enable);
void  Platform_DumpToConsole(const Platform *that, const char *buf, const size_t size);
void  Platform_ClearScreen(const Platform *that);

// SCREEN SIZE
void  Platform_UpdateScreenSize(Platform *that);
Point Platform_FindScreenCenter(const Platform *that);
Point Platform_PixelToCell(const Platform *that, const Point pixel);
Point Platform_CellToTopLeftPixel(const Platform *that, const Point cell);

// EVENTS
Event Platform_PollForEvents(const Platform *that);