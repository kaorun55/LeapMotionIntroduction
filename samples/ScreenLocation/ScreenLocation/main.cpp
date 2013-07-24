//
//  main.cpp
//  ScreenLocation
//
//  Created by 中村 薫 on 2013/07/24.
//  Copyright (c) 2013年 中村 薫. All rights reserved.
//

#include <iostream>

#include <Leap.h>
#include <LeapMath.h>

using namespace Leap;

int main(int argc, const char * argv[])
{
  Controller controller;
  Frame frame = controller.frame();
  if (frame.pointables().count() > 0) {
    Pointable pointable = frame.pointables()[0];
    ScreenList screens = controller.calibratedScreens();
    Screen screen = screens.closestScreenHit(pointable);
  }
  
  return 0;
}

