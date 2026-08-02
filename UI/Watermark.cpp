// UI/Watermark.cpp
#include "Watermark.h"
#include <cstdio>

float Watermark::fps = 0;
clock_t Watermark::last_time = 0;
int Watermark::frame_count = 0;

void Watermark::update_fps() {
    frame_count++;
    clock_t now = clock();
    if ((now - last_time) / CLOCKS_PER_SEC >= 1.0f) {
        fps = frame_count;
        frame_count = 0;
        last_time = now;
    }
}

void Watermark::render() {
    if (!Config::watermark_enabled) return;
    
    update_fps();
    
    ImDrawList* draw = ImGui::GetForegroundDrawList();
    
    // Красивый водяной знак с градиентом
    ImVec2 pos(20, 20);
    ImVec2 size(220, 35);
    
    // Фон с закруглениями
    draw->AddRectFilled(
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(0, 0, 0, 200),
        10.0f,
        ImDrawFlags_RoundCornersAll
    );
    
    // Рамка с градиентом
    draw->AddRect(
        pos,
        ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(150, 50, 200, 255),
        10.0f,
        ImDrawFlags_RoundCornersAll,
        1.5f
    );
    
    // Текст
    char text[128];
    if (Config::show_fps) {
        snprintf(text, sizeof(text), "Standoff 2 PRO | %.0f FPS", fps);
    } else {
        snprintf(text, sizeof(text), "Standoff 2 PRO");
    }
    
    draw->AddText(
        ImVec2(pos.x + 12, pos.y + 8),
        IM_COL32(255, 255, 255, 255),
        text
    );
    
    // Декоративный элемент (точка с подсветкой)
    draw->AddCircleFilled(
        ImVec2(pos.x + size.x - 12, pos.y + 17),
        4,
        IM_COL32(200, 100, 255, 255)
    );
    
    // Градиентная подсветка справа
    draw->AddRectFilledMultiColor(
        ImVec2(pos.x + size.x - 40, pos.y),
        ImVec2(pos.x + size.x, pos.y + size.y),
        IM_COL32(0, 0, 0, 0),
        IM_COL32(150, 50, 200, 80),
        IM_COL32(150, 50, 200, 80),
        IM_COL32(0, 0, 0, 0)
    );
}