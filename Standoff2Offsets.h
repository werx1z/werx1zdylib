// Standoff2Offsets.h
#pragma once
#include <stdint.h>

namespace Standoff2 {
    // Базовый адрес игры (определяется динамически)
    static uintptr_t CLIENT_BASE = 0x100000000;
    
    // Основные оффсеты (проверь по своему дампу!)
    static const uintptr_t OFFSET_ENTITY_LIST = 0x2A1B8C0;
    static const uintptr_t OFFSET_LOCAL_PLAYER = 0x2A1B8C8;
    static const uintptr_t OFFSET_ENTITY_COUNT = 0x2A1B8D0;
    static const uintptr_t OFFSET_VIEW_MATRIX = 0x2B0C00;
    static const uintptr_t OFFSET_GAME_STATE = 0x2A1B8E0;
    
    // Оффсеты внутри структуры игрока
    static const uintptr_t OFFSET_POS_X = 0x2C0;
    static const uintptr_t OFFSET_POS_Y = 0x2C4;
    static const uintptr_t OFFSET_POS_Z = 0x2C8;
    static const uintptr_t OFFSET_VIEW_ANGLES = 0x2D0;
    static const uintptr_t OFFSET_AIM_PUNCH = 0x2E0;
    static const uintptr_t OFFSET_HEALTH = 0x3A0;
    static const uintptr_t OFFSET_ARMOR = 0x3A4;
    static const uintptr_t OFFSET_TEAM = 0x3B0;
    static const uintptr_t OFFSET_IS_DEAD = 0x3B4;
    static const uintptr_t OFFSET_NAME = 0x3C0;
    static const uintptr_t OFFSET_WEAPON_ID = 0x3E0;
    static const uintptr_t OFFSET_CROSSHAIR_ID = 0x2A1B900;
}