// UI/Menu.h
#pragma once
#include "imgui.h"
#include "Config.h"

class Menu {
public:
    static void render();
    static void toggle() { Config::menu_visible = !Config::menu_visible; }
    
private:
    static void render_esp_tab();
    static void render_aim_tab();
    static void render_settings_tab();
    static void apply_style();
};