#include "Leap.h"

#include "TouchInput.h"

class LeapTouch
{
private:

	bool enable;

public:

	TouchInput input;

	enum State
	{
		Up,
		Hover,
		Touch,
	} state;

	LeapTouch()
	{
		state = Up;
	}

	void hover( int x, int y )
	{
		if ( state == Touch ) {
			input.Up();
		}
		else {
			input.Hover( x, y );
		}

		state = Hover;
	}

	void touch( int x, int y )
	{
		if ( state == Touch ) {
			input.Drag( (int)x, (int)y );
		}
		else {
			input.Down( (int)x, (int)y );
		}

		state = Touch;
	}

	void up()
	{
		if ( state != Up ) {
			input.HoverUp();
		}

		state = Up;
	}
};

class SampleListener : public Leap::Listener
{
public:

    virtual void onInit(const Leap::Controller& controller )
	{
		controller.setPolicyFlags( Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES );
		controller.enableGesture( Leap::Gesture::Type::TYPE_CIRCLE );

        Leap::Config config = controller.config();
        std::cout << "Gesture.Circle.MinRadius: " << config.getFloat( "Gesture.Circle.MinRadius" ) << std::endl;


		TouchInput::Initialize();
	}
    
    virtual void onFrame(const Leap::Controller& controller )
	{
		auto frame = controller.frame();

		auto screens = controller.locatedScreens();
		if ( screens.empty() ) {
			return ;
		}

		auto screen = screens[0];
		auto windowWidth = screen.widthPixels();
		auto windowHeight = screen.heightPixels();

		Leap::PointableList pointables = frame.pointables();
		Leap::InteractionBox iBox = frame.interactionBox();

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

        if ( pointables.empty() ) {
            return;
        }

        auto& pointable = pointables[0];
		Leap::Vector normalizedPosition = iBox.normalizePoint( pointable.stabilizedTipPosition() );
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

void main()
{
	Leap::Controller leap;
	SampleListener listener;
	leap.addListener( listener );

    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();

    leap.removeListener( listener );
}

