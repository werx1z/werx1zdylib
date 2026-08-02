// main.mm
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <dlfcn.h>
#include "Renderer.h"
#include "HookManager.h"
#include "Config.h"
#include "GestureManager.h"

Renderer* g_renderer = nullptr;

__attribute__((constructor))
void init_dylib() {
    NSLog(@"[Standoff2] Injecting .dylib...");
    
    // Инициализируем GestureManager (он сам подпишется на события)
    [GestureManager sharedInstance];
    
    // Создаём рендерер
    g_renderer = Renderer::getInstance();
    g_renderer->init_imgui();
    g_renderer->setup_overlay();
    
    // Устанавливаем хуки (теперь без F2)
    HookManager::setup_hooks();
    
    // CADisplayLink для рендеринга
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:g_renderer selector:@selector(render_frame)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    
    NSLog(@"[Standoff2] Injection complete! Use 3-finger touch to toggle menu");
}

__attribute__((destructor))
void cleanup_dylib() {
    NSLog(@"[Standoff2] Cleaning up...");
    HookManager::restore_hooks();
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplIOS_Shutdown();
    ImGui::DestroyContext();
}