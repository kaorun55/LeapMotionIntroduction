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

        // スクリーンの幅と高さを取得する
        auto screen = controller.locatedScreens()[0];
        windowWidth = screen.widthPixels();
        windowHeight = screen.heightPixels();

        // 円ジェスチャーの認識を有効にする
        controller.enableGesture( Leap::Gesture::Type::TYPE_CIRCLE );

        // タッチ入力の初期化を行う
        TouchInput::Initialize();
    }

    // フレーム更新処理
    virtual void onFrame(const Leap::Controller& controller )
    {
        // 新しいフレームを取得する
        auto frame = controller.frame();

        // ポインタを取得する
        Leap::PointableList pointables = frame.pointables();
        if ( pointables.empty() ) {
            return;
        }

        // 最初のポインタの位置をタッチの位置とする
        auto& pointable = pointables[0];
        Leap::Vector normalizedPosition = frame.interactionBox()
                                            .normalizePoint( pointable.stabilizedTipPosition() );
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

        // 円のジェスチャーで Windows キーを押し、スタートメニューに戻る
        for ( auto g : frame.gestures() ) {
            if ( (g.type() == Leap::Gesture::Type::TYPE_CIRCLE) &&
                 (g.state() == Leap::Gesture::State::STATE_STOP) ) {
                keybd_event( VK_LWIN, 0, 0, 0 );
                keybd_event( VK_LWIN, 0, KEYEVENTF_KEYUP, 0 );
                break;
            }
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

