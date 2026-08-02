// Core/Renderer.h
#pragma once
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <UIKit/UIKit.h>
#include "imgui.h"
#include "imgui_impl_metal.h"
#include "imgui_impl_ios.h"

class Renderer {
private:
    id<MTLDevice> device;
    id<MTLCommandQueue> commandQueue;
    CAMetalLayer* metalLayer;
    UIWindow* window;
    bool initialized;
    
    static Renderer* instance;
    
public:
    Renderer();
    ~Renderer();
    
    static Renderer* getInstance();
    void init_imgui();
    void render();
    void setup_overlay();
    void apply_custom_style();
    
    // Для хуков
    void render_frame();
};