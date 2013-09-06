//
//  main.cpp
//  TrackingHandFingerAndToolsConsole
//
//  Created by 中村 薫 on 2013/08/26.
//  Copyright (c) 2013年 中村 薫. All rights reserved.
//

#include <Leap.h>

class SampleListener : public Leap::Listener
{
  int handId;
  
  public :
  
  SampleListener()
  {
    handId = -1;
  }
  
  void onFrame( const Leap::Controller& leap )
  {
    auto frame = leap.frame();
    
#if 0
    // 今回のフレームで検出したすべての手、指、ツール
    Leap::HandList hands = frame.hands();
    Leap::FingerList fingers = frame.fingers();
    Leap::ToolList tools = frame.tools();
    Leap::PointableList pointables = frame.pointables();
    
    std::cout << "Frame Data : "
    << " Hands    : " << hands.count()
    << " Fingers  : " << fingers.count()
    << " Tools    : " << tools.count()
    << " Pointers : " << pointables.count()
    << std::endl;
#endif
    
#if 0
    // 手のIDから、同じ手を追跡し続ける
    if ( handId == -1 ) {
      handId = frame.hands()[0].id();
    }
    else {
      Leap::Hand hand = frame.hand( handId );
      handId = hand.id();
      
      // 手の情報を表示する
      std::cout << "ID : " << hand.id()
      << " 位置 : " << hand.palmPosition()
      << " 速度 : " << hand.palmVelocity()
      << " 法線 : " << hand.palmNormal()
      << " 向き : " << hand.direction()
      << std::endl;
    }
#endif
    
#if 0
    // 一番左、右、手前の手を取得する
    Leap::HandList hands = frame.hands();
    Leap::Hand leftMost = hands.leftmost();
    Leap::Hand rightMost = hands.leftmost();
    Leap::Hand frontMost = hands.frontmost();
    
    std::cout << " 左 : " << leftMost.palmPosition()
    << " 右 : " << rightMost.palmPosition()
    << " 手前 : " << frontMost.palmPosition()
    << std::endl;
#endif
    
#if 0
    // 手に属している指とツールを取得する
    for ( auto hand : frame.hands() ) {
      std::cout << "ID : " << hand.id()
      << " Pointers : " << hand.pointables().count()
      << " Fingers  : " << hand.fingers().count()
      << " Tools    : " << hand.tools().count()
      << std::endl;
    }
#endif
    
#if 1
    Leap::Finger finger = frame.fingers()[0];
    std::cout << "ID : " << finger.id()
    << " 位置 : " << finger.tipPosition()
    << " 速度 : " << finger.tipVelocity()
    << " 向き : " << finger.direction()
    << std::endl;
#endif
  }
};

int main(int argc, const char * argv[])
{
  SampleListener listener;
  Leap::Controller leap( listener );
  
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  return 0;
}
