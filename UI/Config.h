// UI/Config.h
#pragma once
#include "imgui.h"

struct Config {
    // ESP
    static bool esp_enabled;
    static bool esp_box;
    static bool esp_health;
    static bool esp_distance;
    static bool esp_name;
    static bool esp_skeleton;
    static bool esp_head;
    static float esp_thickness;
    static ImColor enemy_color;
    static ImColor team_color;
    
    // Silent Aim
    static bool silent_aim_enabled;
    static float silent_aim_fov;
    static bool silent_aim_visible;
    static bool silent_aim_compensate_recoil;
    
    // Triggerbot
    static bool triggerbot_enabled;
    
    // Menu
    static bool menu_visible;
    static float menu_opacity;
    static bool watermark_enabled;
    static bool show_fps;
};

// Инициализация статических переменных
bool Config::esp_enabled = true;
bool Config::esp_box = true;
bool Config::esp_health = true;
bool Config::esp_distance = true;
bool Config::esp_name = true;
bool Config::esp_skeleton = false;
bool Config::esp_head = true;
float Config::esp_thickness = 1.5f;
ImColor Config::enemy_color = ImColor(255, 0, 0, 255);
ImColor Config::team_color = ImColor(0, 255, 0, 255);

bool Config::silent_aim_enabled = true;
float Config::silent_aim_fov = 30.0f;
bool Config::silent_aim_visible = false;
bool Config::silent_aim_compensate_recoil = true;

bool Config::triggerbot_enabled = false;

bool Config::menu_visible = true;
float Config::menu_opacity = 0.85f;
bool Config::watermark_enabled = true;
bool Config::show_fps = true;