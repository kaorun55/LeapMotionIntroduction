#include <iostream>
#include <Leap.h>

class SampleListrener : public Leap::Listener
{
public:

    virtual void onFocusGained(const Leap::Controller&) {
      std::cout << __FUNCTION__ << std::endl;
    }
    
    virtual void onFocusLost(const Leap::Controller&) {
      std::cout << __FUNCTION__ << std::endl;
    }

};

void GettingFramesByPolling()
{
  Leap::Controller leap;

  int64_t lastFrame = 0;
  while ( 1 ) {
    auto frame = leap.frame();
    if ( !leap.isConnected() ) {
      std::cout << "leap is not connected." << std::endl;
      continue;
    }

    if ( !leap.hasFocus() ) {
      std::cout << "application is not focus." << std::endl;
      continue;
    }

    auto currentFrameid = frame.id();
    if ( currentFrameid == lastFrame ) {
      continue;
    }

    lastFrame = currentFrameid;
  }
}

void GettingFramesWithCallbacks()
{
  SampleListrener listner;
  Leap::Controller leap;
  leap.addListener( listner );

  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get(); 

  leap.removeListener( listner );
}

void main()
{
#if 0
  // ポーリング
  GettingFramesByPolling();
#else
  // コールバック
  GettingFramesWithCallbacks();
#endif
}
