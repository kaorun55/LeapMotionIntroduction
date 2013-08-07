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

        // スクリーンの幅と高さを取得する
        auto screen = controller.locatedScreens()[0];
        windowWidth = screen.widthPixels();
        windowHeight = screen.heightPixels();

        // 円ジェスチャーの認識半径を広げる
        Leap::Config config = controller.config();
        std::cout << "Gesture.Circle.MinRadius: " << config.getFloat( "Gesture.Circle.MinRadius" ) << std::endl;

        config.setFloat( "Gesture.Circle.MinRadius", 20 );
        config.save();

        TouchInput::Initialize();
    }

    // フレーム更新処理
    virtual void onFrame(const Leap::Controller& controller )
    {
        auto frame = controller.frame();

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

        // 最初のポインタを使う
        auto& pointable = pointables[0];
        Leap::Vector normalizedPosition = frame.interactionBox().normalizePoint( pointable.stabilizedTipPosition() );
        int x = (int)min( normalizedPosition.x * windowWidth, windowWidth - 1 );
        int y = (int)min( windowHeight - normalizedPosition.y * windowHeight, windowHeight - 1 );

        // ホバー
        if ( pointable.touchDistance() > 0 &&
             pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE) {
            touch.hover( x, y );
        }
        // タッチ
        else if( pointable.touchDistance() <= 0 ) {
            touch.touch( x, y );
        }
        // タッチしてない
        else {
            touch.up();
        }
    }

private:

    LeapTouch touch;

    int windowWidth;
    int windowHeight;
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

