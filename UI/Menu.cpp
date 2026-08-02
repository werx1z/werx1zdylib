void Menu::render() {
    // Если меню только что переключилось — показываем всплывашку
    static bool was_visible = false;
    static float notification_timer = 0.0f;
    
    if (was_visible != Config::menu_visible) {
        was_visible = Config::menu_visible;
        notification_timer = 2.0f; // показываем 2 секунды
    }
    
    // Рисуем уведомление
    if (notification_timer > 0.0f) {
        notification_timer -= ImGui::GetIO().DeltaTime;
        ImDrawList* draw = ImGui::GetForegroundDrawList();
        const char* msg = Config::menu_visible ? "Menu Opened" : "Menu Closed";
        ImVec2 textSize = ImGui::CalcTextSize(msg);
        ImVec2 center(ImGui::GetIO().DisplaySize.x / 2 - textSize.x / 2, 100);
        
        // Фон с закруглениями
        draw->AddRectFilled(
            ImVec2(center.x - 20, center.y - 10),
            ImVec2(center.x + textSize.x + 20, center.y + textSize.y + 10),
            IM_COL32(0, 0, 0, (int)(200 * (notification_timer / 2.0f))),
            8.0f
        );
        draw->AddText(center, IM_COL32(255, 255, 255, (int)(255 * (notification_timer / 2.0f))), msg);
    }
    
    // ... остальной код рендера меню
}