// Core/SilentAim.cpp
#include "SilentAim.h"
#include "Standoff2Offsets.h"

Memory* SilentAim::mem = nullptr;

void SilentAim::init(Memory* memory) {
    mem = memory;
}

Vector3 SilentAim::calculate_angle(Vector3 source, Vector3 target) {
    Vector3 delta = {
        target.x - source.x,
        target.y - source.y,
        target.z - source.z
    };
    
    float length = sqrtf(delta.x*delta.x + delta.y*delta.y + delta.z*delta.z);
    if (length < 0.001f) return Vector3(0,0,0);
    
    Vector3 angles;
    angles.x = asinf(delta.z / length) * (180.0f / M_PI);
    angles.y = atan2f(delta.y, delta.x) * (180.0f / M_PI);
    angles.z = 0;
    
    return angles;
}

Vector3 SilentAim::clamp_angles(Vector3 angles) {
    if (angles.x > 89.0f) angles.x = 89.0f;
    if (angles.x < -89.0f) angles.x = -89.0f;
    if (angles.y > 180.0f) angles.y = -180.0f;
    if (angles.y < -180.0f) angles.y = 180.0f;
    return angles;
}

float SilentAim::get_fov(Vector3 source, Vector3 target) {
    Vector3 angle = calculate_angle(source, target);
    // Упрощённо: FOV = расстояние по углам от центра экрана
    return sqrtf(angle.x*angle.x + angle.y*angle.y);
}

Vector3 SilentAim::get_best_target() {
    Vector3 best_target(0,0,0);
    float best_fov = Config::silent_aim_fov;
    
    uintptr_t local = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_LOCAL_PLAYER);
    if (!local) return best_target;
    
    Vector3 local_pos(
        mem->read_float(local + Standoff2::OFFSET_POS_X),
        mem->read_float(local + Standoff2::OFFSET_POS_Y),
        mem->read_float(local + Standoff2::OFFSET_POS_Z)
    );
    
    auto& players = ESP::get_players();
    for (auto& p : players) {
        if (p.team == 0) continue; // зрители
        if (p.team == Standoff2::local_team) continue; // свои
        
        Vector3 target = p.position;
        target.z += 1.0f; // целься в грудь
        
        float fov = get_fov(local_pos, target);
        if (fov < best_fov) {
            best_fov = fov;
            best_target = target;
        }
    }
    
    return best_target;
}

void SilentAim::apply_aim() {
    if (!Config::silent_aim_enabled) return;
    
    uintptr_t local = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_LOCAL_PLAYER);
    if (!local) return;
    
    Vector3 local_pos(
        mem->read_float(local + Standoff2::OFFSET_POS_X),
        mem->read_float(local + Standoff2::OFFSET_POS_Y),
        mem->read_float(local + Standoff2::OFFSET_POS_Z)
    );
    
    Vector3 target = get_best_target();
    if (target.x == 0 && target.y == 0 && target.z == 0) return;
    
    Vector3 aim_angle = calculate_angle(local_pos, target);
    aim_angle = clamp_angles(aim_angle);
    
    // Пишем углы в память (Silent Aim — не двигает прицел визуально)
    mem->write_float(local + Standoff2::OFFSET_VIEW_ANGLES, aim_angle.x);
    mem->write_float(local + Standoff2::OFFSET_VIEW_ANGLES + 4, aim_angle.y);
    
    // Компенсация отдачи (если есть)
    if (Config::silent_aim_compensate_recoil) {
        float punch_x = mem->read_float(local + Standoff2::OFFSET_AIM_PUNCH);
        float punch_y = mem->read_float(local + Standoff2::OFFSET_AIM_PUNCH + 4);
        mem->write_float(local + Standoff2::OFFSET_VIEW_ANGLES, aim_angle.x - punch_x);
        mem->write_float(local + Standoff2::OFFSET_VIEW_ANGLES + 4, aim_angle.y - punch_y);
    }
}

void SilentAim::triggerbot() {
    if (!Config::triggerbot_enabled) return;
    
    uintptr_t local = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_LOCAL_PLAYER);
    if (!local) return;
    
    int crosshair_id = mem->read_int(Standoff2::CLIENT_BASE + Standoff2::OFFSET_CROSSHAIR_ID);
    if (crosshair_id <= 0 || crosshair_id > 64) return;
    
    uintptr_t entityList = mem->read_ptr(Standoff2::CLIENT_BASE + Standoff2::OFFSET_ENTITY_LIST);
    uintptr_t entity = mem->read_ptr(entityList + crosshair_id * 8);
    if (!entity) return;
    
    int health = mem->read_int(entity + Standoff2::OFFSET_HEALTH);
    if (health <= 0) return;
    
    int team = mem->read_int(entity + Standoff2::OFFSET_TEAM);
    int local_team = mem->read_int(local + Standoff2::OFFSET_TEAM);
    if (team == local_team) return;
    
    // Симулируем нажатие на экран (или записываем в оффсет стрельбы)
    // Заглушка — можно найти оффсет shooting и записать 1
}

bool SilentAim::is_key_pressed() {
    // Заглушка — в iOS можно использовать IOHID или UIEvent
    // Возвращаем true для теста
    return true;
}