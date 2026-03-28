#pragma once
#include "../Platform/Platform.h"

typedef void (*EventHandler) (Event);

typedef struct {
    Rectangle    Region;
    EventTypes   On;
    EventHandler Handler;
} EventRegistration;

typedef struct {
    EventRegistration *Registrations;
    size_t Count;
} EventRegistry;