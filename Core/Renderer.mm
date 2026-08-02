// Core/Renderer.mm
#import "Renderer.h"
#import "ESP.h"
#import "SilentAim.h"
#import "Menu.h"
#import "Watermark.h"
#import "Config.h"
#include "imgui_impl_metal.h"

// ... остальной код

void Renderer::render() {
    if (!initialized) {
        setup_overlay();
        init_imgui();
    }
    
    ImGui_ImplMetal_NewFrame();
    ImGui::NewFrame();
    
    ESP::render();
    SilentAim::apply_aim();
    SilentAim::triggerbot();
    Menu::render();
    Watermark::render();
    
    ImGui::Render();
    
    id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
    if (drawable) {
        id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
        MTLRenderPassDescriptor* renderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
        renderPassDesc.colorAttachments[0].texture = drawable.texture;
        renderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
        renderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 0);
        renderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
        
        id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
        // Правильный вызов для нового API
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, encoder);
        [encoder endEncoding];
        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
    }
}
