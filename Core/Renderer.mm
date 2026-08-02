// Core/Renderer.mm
#import "Renderer.h"
#import "ESP.h"
#import "SilentAim.h"
#import "Menu.h"
#import "Watermark.h"
#import "Memory.h"
#import "Config.h"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer() : initialized(false) {
    instance = this;
    device = MTLCreateSystemDefaultDevice();
    commandQueue = [device newCommandQueue];
}

Renderer::~Renderer() {
    // Cleanup
}

Renderer* Renderer::getInstance() {
    if (!instance) {
        instance = new Renderer();
    }
    return instance;
}

void Renderer::setup_overlay() {
    UIApplication* app = [UIApplication sharedApplication];
    window = app.keyWindow;
    if (!window) {
        window = app.windows.firstObject;
    }
    
    metalLayer = [CAMetalLayer layer];
    metalLayer.device = device;
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.frame = window.bounds;
    metalLayer.opaque = NO;
    metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), (CGFloat[]){0, 0, 0, 0});
    
    [window.layer addSublayer:metalLayer];
    [window bringSubviewToFront:window];
    
    NSLog(@"[Renderer] Overlay setup complete");
}

void Renderer::init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    
    ImGui_ImplMetal_Init(device);
    ImGui_ImplIOS_Init(window);
    
    apply_custom_style();
    
    // Инициализируем ESP и Silent Aim
    ESP::init(new Memory());
    SilentAim::init(new Memory());
    
    initialized = true;
    NSLog(@"[Renderer] ImGui initialized");
}

void Renderer::apply_custom_style() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.10f, 0.40f, 1.0f);
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.Alpha = 0.95f;
}

void Renderer::render() {
    if (!initialized) {
        init_imgui();
        setup_overlay();
    }
    
    // Новый кадр ImGui
    ImGui_ImplMetal_NewFrame();
    ImGui_ImplIOS_NewFrame();
    ImGui::NewFrame();
    
    // Рендерим ESP
    ESP::render();
    
    // Применяем Silent Aim
    SilentAim::apply_aim();
    SilentAim::triggerbot();
    
    // Рендерим меню
    Menu::render();
    
    // Рендерим водяной знак
    Watermark::render();
    
    // Рендерим ImGui
    ImGui::Render();
    
    // Отправляем на экран через Metal
    id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
    if (drawable) {
        id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
        ImDrawData* drawData = ImGui::GetDrawData();
        
        // Рендерим ImGui через Metal
        ImGui_ImplMetal_RenderDrawData(drawData, commandBuffer, drawable);
        
        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
    }
}

void Renderer::render_frame() {
    render();
}