// Вместо:
// ImGui::GetWindowDrawList()->AddRect(...)

// Используй:
ImDrawList* drawList = ImGui::GetWindowDrawList();
drawList->AddRect(
    ImVec2(top.x, top.y),
    ImVec2(bottom.x, bottom.y),
    color,
    1.0f,
    15,
    Config::esp_thickness
);

// Для ImColor:
ImColor health_color = ImColor(
    (1.0f - health_percent) * 255.0f,
    health_percent * 255.0f,
    0.0f,
    255.0f
);
