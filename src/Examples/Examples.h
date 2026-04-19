#pragma once
#include "../Core/Renderer/Renderer.h"

#define CLICKBAIT 46
#define CIRCLE_RADIUS 100
#define BOX_SIDE 504
#define BOX_HALF_SIDE (BOX_SIDE / 2)

int EventLoop();
void ExecuteExample(Context *context, Point center);

//EXAMPLES
void DrawBox(Context *context, Point center);