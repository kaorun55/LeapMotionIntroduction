#include "Leap.h"

#include "LeapTouch.h"

class SampleListener : public Leap::Listener
{
public:

    // 初期化処理
    virtual void onInit(const Leap::Controller& controller )
	{
        // アプリケーションがバックグラウンドでもLeapからのフレームデータを受け取れるようにする
		controller.setPolicyFlags( Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES );
		controller.enableGesture( Leap::Gesture::Type::TYPE_CIRCLE );

        Leap::Config config = controller.config();
        std::cout << "Gesture.Circle.MinRadius: " << config.getFloat( "Gesture.Circle.MinRadius" ) << std::endl;

		TouchInput::Initialize();
	}
    
    // フレーム更新処理
    virtual void onFrame(const Leap::Controller& controller )
	{
		auto frame = controller.frame();

        // これinitでできるかな
		auto screens = controller.locatedScreens();
		if ( screens.empty() ) {
			return ;
		}

		auto screen = screens[0];
		auto windowWidth = screen.widthPixels();
		auto windowHeight = screen.heightPixels();

        ////// スタートメニューの部分だけ何とかしたい
		for ( auto g : frame.gestures() ) {
            if ( g.hands()[0].fingers().count() == 2 ) {
			    if ( g.type() == Leap::Gesture::Type::TYPE_CIRCLE ) {
				    if( g.state() == Leap::Gesture::State::STATE_STOP ) {
					    keybd_event( VK_LWIN, 0, 0, 0 );
					    keybd_event( VK_LWIN, 0, KEYEVENTF_KEYUP, 0 );
					    break;
				    }
			    }
            }
		}
        //////

		Leap::PointableList pointables = frame.pointables();
        if ( pointables.empty() ) {
            return;
        }

        auto& pointable = pointables[0];
		Leap::Vector normalizedPosition = frame.interactionBox().normalizePoint( pointable.stabilizedTipPosition() );
		int x = (int)min( normalizedPosition.x * windowWidth, windowWidth - 1 );
		int y = (int)min( windowHeight - normalizedPosition.y * windowHeight, windowHeight - 1 );

		// ホバー
		if(pointable.touchDistance() > 0 && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE) {
			touch.hover( x, y );
		}
		// タッチ
		else if(pointable.touchDistance() <= 0) {
			touch.touch( x, y );
		}
		// タッチしてない
		else {
			touch.up();
		}
	}

private:

    LeapTouch touch;
};

// main()
void main()
{
	Leap::Controller leap;
	SampleListener listener;
	leap.addListener( listener );

    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    leap.removeListener( listener );
}

