// UI/Watermark.h
#pragma once
#include "imgui.h"
#include "Config.h"
#include <time.h>
#include <string>

class Watermark {
private:
    static float fps;
    static clock_t last_time;
    static int frame_count;
    
public:
    static void render();
    static void update_fps();
};