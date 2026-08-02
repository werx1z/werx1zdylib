// UI/GestureManager.h
#pragma once
#import <UIKit/UIKit.h>

@interface GestureManager : NSObject

+ (instancetype)sharedInstance;
- (void)handleTouches:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;

@end