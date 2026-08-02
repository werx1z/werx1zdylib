// Core/ESP.cpp
#include "ESP.h"
#include "Config.h"
#include <cstdio>

Memory* ESP::mem = nullptr;
Vector3 ESP::local_position;
int ESP::local_team = 0;
std::vector<Player> ESP::players;

void ESP::init(Memory* memory) {
    mem = memory;
}

bool ESP::world_to_screen(Vector3 world, Vector2& screen) {
    uintptr_t viewMatrixPtr = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_VIEW_MATRIX);
    if (!viewMatrixPtr) return false;
    
    float* viewMatrix = (float*)viewMatrixPtr;
    
    float w = viewMatrix[3] * world.x + viewMatrix[7] * world.y + viewMatrix[11] * world.z + viewMatrix[15];
    if (w < 0.001f) return false;
    
    float x = viewMatrix[0] * world.x + viewMatrix[4] * world.y + viewMatrix[8] * world.z + viewMatrix[12];
    float y = viewMatrix[1] * world.x + viewMatrix[5] * world.y + viewMatrix[9] * world.z + viewMatrix[13];
    
    x /= w;
    y /= w;
    
    // Разрешение экрана iPhone (можно получить динамически)
    const float SCREEN_WIDTH = 1170.0f;
    const float SCREEN_HEIGHT = 2532.0f;
    
    screen.x = (x + 1.0f) * 0.5f * SCREEN_WIDTH;
    screen.y = (1.0f - y) * 0.5f * SCREEN_HEIGHT;
    screen.visible = true;
    
    return true;
}

void ESP::draw_box(Vector2 top, Vector2 bottom, ImColor color) {
    if (!Config::esp_box) return;
    ImGui::GetWindowDrawList()->AddRect(
        ImVec2(top.x, top.y),
        ImVec2(bottom.x, bottom.y),
        color,
        1.0f,
        15,
        Config::esp_thickness
    );
}

void ESP::draw_health_bar(Player& p) {
    if (!Config::esp_health) return;
    
    float bar_width = 4.0f;
    float bar_height = p.screen_pos.y - p.screen_head.y;
    float health_percent = p.health / 100.0f;
    
    ImColor bg_color = ImColor(0, 0, 0, 180);
    ImColor health_color = ImColor(
        (1.0f - health_percent) * 255,
        health_percent * 255,
        0
    );
    
    float bar_x = p.screen_pos.x - 10;
    float bar_y = p.screen_head.y;
    
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(bar_x, bar_y),
        ImVec2(bar_x + bar_width, bar_y + bar_height),
        bg_color
    );
    
    float health_height = bar_height * health_percent;
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(bar_x, bar_y + bar_height - health_height),
        ImVec2(bar_x + bar_width, bar_y + bar_height),
        health_color
    );
}

void ESP::draw_name(Player& p) {
    if (!Config::esp_name) return;
    ImGui::GetWindowDrawList()->AddText(
        ImVec2(p.screen_pos.x - 30, p.screen_head.y - 25),
        ImColor(255, 255, 255, 255),
        p.name
    );
}

void ESP::draw_distance(Player& p) {
    if (!Config::esp_distance) return;
    char dist_text[32];
    snprintf(dist_text, sizeof(dist_text), "%.1fm", p.distance);
    ImGui::GetWindowDrawList()->AddText(
        ImVec2(p.screen_pos.x - 15, p.screen_pos.y + 5),
        ImColor(200, 200, 200, 255),
        dist_text
    );
}

void ESP::draw_skeleton(Player& p) {
    if (!Config::esp_skeleton) return;
    // Для скелета нужны оффсеты костей — упрощённо рисуем линии
    // В реальности нужно читать bone matrix из памяти игры
    // Это заглушка — можно расширить
}

void ESP::draw_head_circle(Player& p) {
    if (!Config::esp_head) return;
    ImGui::GetWindowDrawList()->AddCircle(
        ImVec2(p.screen_head.x, p.screen_head.y),
        8.0f,
        ImColor(255, 0, 0, 255),
        12,
        2.0f
    );
}

void ESP::update_players() {
    players.clear();
    
    uintptr_t entityList = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_ENTITY_LIST);
    if (!entityList) return;
    
    int count = mem->read_int(Standoff2::CLIENT_BASE + Standoff2::OFFSET_ENTITY_COUNT);
    if (count > 64) count = 64;
    
    uintptr_t local = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_LOCAL_PLAYER);
    if (!local) return;
    
    // Получаем позицию локального игрока
    local_position.x = mem->read_float(local + Standoff2::OFFSET_POS_X);
    local_position.y = mem->read_float(local + Standoff2::OFFSET_POS_Y);
    local_position.z = mem->read_float(local + Standoff2::OFFSET_POS_Z);
    local_team = mem->read_int(local + Standoff2::OFFSET_TEAM);
    
    for (int i = 0; i < count; i++) {
        uintptr_t entity = mem->read_ptr(entityList + i * 8);
        if (!entity || entity == local) continue;
        
        Player p;
        p.address = entity;
        
        p.position.x = mem->read_float(entity + Standoff2::OFFSET_POS_X);
        p.position.y = mem->read_float(entity + Standoff2::OFFSET_POS_Y);
        p.position.z = mem->read_float(entity + Standoff2::OFFSET_POS_Z);
        
        // Голова примерно на 1.7 выше тела
        p.head_position = p.position;
        p.head_position.z += 1.7f;
        
        if (!world_to_screen(p.position, p.screen_pos)) continue;
        if (!world_to_screen(p.head_position, p.screen_head)) continue;
        
        p.health = mem->read_int(entity + Standoff2::OFFSET_HEALTH);
        p.armor = mem->read_int(entity + Standoff2::OFFSET_ARMOR);
        p.team = mem->read_int(entity + Standoff2::OFFSET_TEAM);
        p.dead = mem->read_bool(entity + Standoff2::OFFSET_IS_DEAD);
        mem->read_string(entity + Standoff2::OFFSET_NAME, p.name, 32);
        p.distance = local_position.distance(p.position);
        
        if (p.health > 0 && !p.dead) {
            players.push_back(p);
        }
    }
}

void ESP::render() {
    if (!Config::esp_enabled) return;
    
    update_players();
    
    for (auto& p : players) {
        bool is_enemy = (p.team != local_team && p.team != 0);
        ImColor color = is_enemy ? Config::enemy_color : Config::team_color;
        
        // Бокс
        if (Config::esp_box) {
            float height = p.screen_pos.y - p.screen_head.y;
            float width = height * 0.35f;
            Vector2 top(p.screen_head.x - width/2, p.screen_head.y);
            Vector2 bottom(p.screen_head.x + width/2, p.screen_pos.y);
            draw_box(top, bottom, color);
        }
        
        // Здоровье
        draw_health_bar(p);
        
        // Имя
        draw_name(p);
        
        // Дистанция
        draw_distance(p);
        
        // Голова (круг)
        draw_head_circle(p);
        
        // Скелет (заглушка)
        draw_skeleton(p);
    }
}