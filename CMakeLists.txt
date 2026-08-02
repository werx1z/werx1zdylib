cmake_minimum_required(VERSION 3.15)
project(Standoff2ESP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc -std=c++17 -O2 -Wall")

# Определяем iOS
if(IOS OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IOS TRUE)
    set(CMAKE_OSX_SYSROOT "iphoneos")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
    message(STATUS "Building for iOS")
endif()

# Путь к ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ImGui")

# Исходники
set(SOURCES
    main.mm
    Core/Memory.cpp
    Core/ESP.cpp
    Core/SilentAim.cpp
    Core/HookManager.cpp
    Core/Renderer.mm
    UI/Menu.cpp
    UI/Watermark.cpp
    UI/GestureManager.mm
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_metal.mm
)

# Хедеры
set(HEADERS
    Core/Memory.h
    Core/ESP.h
    Core/SilentAim.h
    Core/HookManager.h
    Core/Renderer.h
    UI/Config.h
    UI/Menu.h
    UI/Watermark.h
    UI/GestureManager.h
    Standoff2Offsets.h
)

# Создаём библиотеку
add_library(Standoff2ESP SHARED ${SOURCES} ${HEADERS})

target_include_directories(Standoff2ESP PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(Standoff2ESP
    "-framework Foundation"
    "-framework UIKit"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework CoreGraphics"
    "-framework IOKit"
    "-framework CoreFoundation"
    "-lc++"
)

# Установка свойств
set_target_properties(Standoff2ESP PROPERTIES
    SUFFIX ".dylib"
    PREFIX ""
    LINK_FLAGS "-dynamiclib -undefined dynamic_lookup"
)

# Для Xcode
if(CMAKE_GENERATOR STREQUAL "Xcode")
    set_target_properties(Standoff2ESP PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "12.0"
        XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO"
    )
endif()

# Вывод информации
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "iOS target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
message(STATUS "ImGui path: ${IMGUI_DIR}")