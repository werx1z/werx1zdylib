// Core/HookManager.cpp
#include "HookManager.h"
#include "Renderer.h"
#include "GestureManager.h"
#include <Foundation/Foundation.h>

IMP HookManager::original_touchesBegan = nullptr;
IMP HookManager::original_present = nullptr;
IMP HookManager::original_draw = nullptr;

void HookManager::hooked_touchesBegan(id self, SEL cmd, NSSet* touches, UIEvent* event) {
    // Отправляем касания в GestureManager
    [[GestureManager sharedInstance] handleTouches:touches withEvent:event];
    
    // Вызываем оригинальный метод
    if (original_touchesBegan) {
        ((void(*)(id, SEL, NSSet*, UIEvent*))original_touchesBegan)(self, cmd, touches, event);
    }
}

void HookManager::hooked_present(id self, SEL cmd) {
    if (original_present) {
        ((void(*)(id, SEL))original_present)(self, cmd);
    }
    Renderer::getInstance()->render();
}

void HookManager::hooked_draw(id self, SEL cmd) {
    if (original_draw) {
        ((void(*)(id, SEL))original_draw)(self, cmd);
    }
}

void HookManager::setup_hooks() {
    // ===== ХУК НА КАСАНИЯ =====
    // Находим главное окно и его класс
    UIWindow* keyWindow = [UIApplication sharedApplication].keyWindow;
    if (!keyWindow) {
        keyWindow = [UIApplication sharedApplication].windows.firstObject;
    }
    
    Class windowClass = [keyWindow class];
    SEL touchesSEL = @selector(touchesBegan:withEvent:);
    Method origTouches = class_getInstanceMethod(windowClass, touchesSEL);
    
    if (origTouches) {
        original_touchesBegan = method_getImplementation(origTouches);
        method_setImplementation(origTouches, (IMP)hooked_touchesBegan);
        NSLog(@"[Hook] touchesBegan:withEvent: hooked");
    } else {
        // Если метод не найден — добавляем свой
        class_addMethod(windowClass, touchesSEL, (IMP)hooked_touchesBegan, "v@:@");
        NSLog(@"[Hook] touchesBegan:withEvent: added");
    }
    
    // ===== ХУК НА METAL =====
    Class metalLayerClass = NSClassFromString(@"CAMetalLayer");
    if (metalLayerClass) {
        SEL presentSEL = @selector(presentDrawable:);
        Method origMethod = class_getInstanceMethod(metalLayerClass, presentSEL);
        if (origMethod) {
            original_present = method_getImplementation(origMethod);
            method_setImplementation(origMethod, (IMP)hooked_present);
            NSLog(@"[Hook] CAMetalLayer presentDrawable: hooked");
        }
    }
    
    // ===== ХУК НА VIEW CONTROLLER =====
    UIViewController* rootVC = keyWindow.rootViewController;
    if (rootVC) {
        SEL drawSEL = @selector(viewWillLayoutSubviews);
        Method drawMethod = class_getInstanceMethod([rootVC class], drawSEL);
        if (drawMethod) {
            original_draw = method_getImplementation(drawMethod);
            method_setImplementation(drawMethod, (IMP)hooked_draw);
            NSLog(@"[Hook] ViewController viewWillLayoutSubviews hooked");
        }
    }
}

void HookManager::restore_hooks() {
    // Восстанавливаем оригинальные функции
    if (original_touchesBegan) {
        UIWindow* keyWindow = [UIApplication sharedApplication].keyWindow;
        Class windowClass = [keyWindow class];
        SEL touchesSEL = @selector(touchesBegan:withEvent:);
        Method origMethod = class_getInstanceMethod(windowClass, touchesSEL);
        method_setImplementation(origMethod, original_touchesBegan);
        original_touchesBegan = nullptr;
    }
    
    if (original_present) {
        Class metalLayerClass = NSClassFromString(@"CAMetalLayer");
        SEL presentSEL = @selector(presentDrawable:);
        Method origMethod = class_getInstanceMethod(metalLayerClass, presentSEL);
        method_setImplementation(origMethod, original_present);
        original_present = nullptr;
    }
}