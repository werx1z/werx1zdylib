// UI/GestureManager.mm
#import "GestureManager.h"
#include "Config.h"

@implementation GestureManager

+ (instancetype)sharedInstance {
    static GestureManager* instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[GestureManager alloc] init];
        [instance setupSwizzle];
    });
    return instance;
}

- (void)setupSwizzle {
    // Используем категорию для UIApplication
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Class appClass = [UIApplication class];
        Method original = class_getInstanceMethod(appClass, @selector(sendEvent:));
        Method swizzled = class_getInstanceMethod(appClass, @selector(swizzled_sendEvent:));
        
        // Добавляем метод если его нет
        if (!swizzled) {
            IMP swizzledImp = imp_implementationWithBlock(^(id self, UIEvent* event) {
                // Код будет в swizzled_sendEvent ниже
            });
            swizzled = class_addMethod(appClass, @selector(swizzled_sendEvent:), swizzledImp, "v@:@");
        }
        
        if (original && swizzled) {
            method_exchangeImplementations(original, swizzled);
            NSLog(@"[Gesture] UIApplication sendEvent: swizzled");
        }
    });
}

// Этот метод вызывается вместо оригинального sendEvent:
- (void)swizzled_sendEvent:(UIEvent *)event {
    // Перехватываем касания
    if (event.type == UIEventTypeTouches) {
        NSSet* touches = [event allTouches];
        if (touches.count == 3) {
            BOOL allBegan = YES;
            for (UITouch* touch in touches) {
                if (touch.phase != UITouchPhaseBegan) {
                    allBegan = NO;
                    break;
                }
            }
            if (allBegan) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    Config::menu_visible = !Config::menu_visible;
                    NSLog(@"[Gesture] Menu toggled via 3-finger touch!");
                });
            }
        }
    }
    
    // Вызываем оригинальный метод (через рекурсию, но с подменой)
    [self swizzled_sendEvent:event];
}

@end