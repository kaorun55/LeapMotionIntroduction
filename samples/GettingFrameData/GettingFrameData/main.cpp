#include <iostream>
#include <Leap.h>

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

    bool focus = leap.hasFocus();
    if ( !focus ) {
      std::cout << "application is not focus." << std::endl;
      continue;
    }

    auto currentFrameid = frame.id();
    if ( currentFrameid == lastFrame ) {
      //std::cout << "processed frame id." << std::endl;
      continue;
    }

    //std::cout << "frame count : " << (currentFrameid - lastFrame) << std::endl;

    lastFrame = currentFrameid;
    
    // 指の位置の平均値を計算する
    int count = 0;
    Leap::Vector average = Leap::Vector();
    Leap::Finger fingerToAverage = frame.fingers()[0];
    for( int i = 0; i < 10; i++ ) {
        // フレーム履歴と指のIDで、過去の指の位置を取得する
        Leap::Finger fingerFromFrame = leap.frame(i).finger(fingerToAverage.id());
        if( fingerFromFrame.isValid() ) {
            average += fingerFromFrame.tipPosition();
            count++;
        }
    }
    average /= count;
    std::cout << "finger point : " << average << std::endl;
  }
}


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
#if 1
  // ポーリング
  GettingFramesByPolling();
#else
  // コールバック
  GettingFramesWithCallbacks();
#endif
}
