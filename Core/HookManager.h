// Core/HookManager.h
#pragma once
#include <objc/runtime.h>
#include <objc/message.h>
#include <mach/mach.h>

class HookManager {
private:
    static IMP original_touchesBegan;
    static IMP original_present;
    static IMP original_draw;
    
    static void hooked_touchesBegan(id self, SEL cmd, NSSet* touches, UIEvent* event);
    static void hooked_present(id self, SEL cmd);
    static void hooked_draw(id self, SEL cmd);
    
public:
    static void setup_hooks();
    static void restore_hooks();
};