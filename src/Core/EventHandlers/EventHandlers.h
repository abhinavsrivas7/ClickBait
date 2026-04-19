#pragma once
#include "../Platform/Platform.h"

typedef void (*EventHandler) (Event);

typedef struct {
    AABB    Region;
    EventTypes   On;
    EventHandler Handler;
} EventRegistration;

typedef struct {
    EventRegistration *Registrations;
    size_t Count;
} EventRegistry;
