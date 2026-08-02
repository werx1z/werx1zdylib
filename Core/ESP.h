// Core/ESP.h
#pragma once
#include <vector>
#include <string>
#include <cmath>
#include "imgui.h"
#include "Standoff2Offsets.h"
#include "Memory.h"

struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    float distance(const Vector3& other) {
        return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2) + powf(z - other.z, 2));
    }
};

struct Vector2 {
    float x, y;
    bool visible;
    
    Vector2() : x(0), y(0), visible(false) {}
    Vector2(float x, float y) : x(x), y(y), visible(true) {}
};

struct Player {
    uintptr_t address;
    Vector3 position;
    Vector3 head_position;
    Vector2 screen_pos;
    Vector2 screen_head;
    int health;
    int armor;
    int team;
    bool dead;
    char name[32];
    float distance;
    bool visible;
};

class ESP {
private:
    static Memory* mem;
    static Vector3 local_position;
    static int local_team;
    static std::vector<Player> players;
    
    static bool world_to_screen(Vector3 world, Vector2& screen);
    static void draw_box(Vector2 top, Vector2 bottom, ImColor color);
    static void draw_health_bar(Player& p);
    static void draw_name(Player& p);
    static void draw_distance(Player& p);
    static void draw_skeleton(Player& p);
    static void draw_head_circle(Player& p);
    
public:
    static void init(Memory* memory);
    static void update_players();
    static void render();
    static std::vector<Player>& get_players() { return players; }
};