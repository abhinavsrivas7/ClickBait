#pragma once
#define true 1
#define false 0
#define QUIT_COMMAND 27
#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char Bool;

static inline void CbFlush() { fflush(stdout); }