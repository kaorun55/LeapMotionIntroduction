#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "Leap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ScreenLocationApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
    int windowWidth;
    int windowHeight;
    Leap::Controller leap;
};

void ScreenLocationApp::setup()
{
    auto screen = leap.locatedScreens()[0];
    windowWidth = screen.widthPixels();
    windowHeight = screen.heightPixels();

    for ( auto screen : leap.locatedScreens() ) {
        std::stringstream ss;
        ss << screen.widthPixels() << ", " << screen.heightPixels() << std::endl;
        ss << screen << std::endl;
        ::OutputDebugStringA( ss.str().c_str() );
    }

    for ( auto device : leap.devices() ) {
        std::stringstream ss;
        ss << device << std::endl;
        ::OutputDebugStringA( ss.str().c_str() );
    }

    this->setWindowPos( 0, 0 );
    this->setWindowSize(windowWidth, windowHeight);
    this->setFrameRate(120);
    gl::enableAlphaBlending();
}

void ScreenLocationApp::mouseDown( MouseEvent event )
{
}

void ScreenLocationApp::update()
{
}

void ScreenLocationApp::draw()
{
    gl::clear( Color( .97, .93, .79 ) );
    Leap::PointableList pointables = leap.frame().pointables();
    Leap::InteractionBox iBox = leap.frame().interactionBox();

    //for( int p = 0; p < pointables.count(); p++ )
    if ( !pointables.empty() )
    {
        //Leap::Pointable pointable = pointables[p];
        Leap::Pointable pointable = pointables[0];

        // Touch Emulation
        //Leap::Vector normalizedPosition = iBox.normalizePoint(pointable.stabilizedTipPosition());

        //float x = normalizedPosition.x * windowWidth;
        //float y = windowHeight - normalizedPosition.y * windowHeight;
        ////////

        Leap::ScreenList screens = leap.locatedScreens();
        Leap::Screen screen = screens.closestScreenHit(pointable);

        // Find the pixel coordinates of an intersection point
        Leap::Vector normalizedCoordinates = screen.intersect(pointable, true, 1);

        float x = normalizedCoordinates.x * windowWidth;
        float y = windowHeight - normalizedCoordinates.y * windowHeight;
        ////////

        // Find the Leap Motion coordinates of a projection point
        //Leap::Vector screenProjection = screen.project(pointable.tipPosition(), false);
        //float x = screenProjection.x;
        //float y = screenProjection.y;
        {
            std::stringstream ss;
            ss << normalizedCoordinates << std::endl;
            ::OutputDebugStringA( ss.str().c_str() );
        }
        ////////

        // Find the distance between the pointing finger or tool and the screen
        //Leap::Vector intersection = screen.intersect(pointable, false);
        //Leap::Vector tipToScreen = intersection - pointable.tipPosition();
        //float pointingDistance = tipToScreen.magnitude();
        //{
        //    std::stringstream ss;
        //    ss << "distance : " << pointingDistance << std::endl;
        //    ::OutputDebugStringA( ss.str().c_str() );
        //}
        ////////

        // Find the distance between the pointing finger or tool and the screen
        //float perpendicularDistance = screen.distanceToPoint(pointable.tipPosition());
        //{
        //    std::stringstream ss;
        //    ss << "distance : " << perpendicularDistance << std::endl;
        //    ::OutputDebugStringA( ss.str().c_str() );
        //}
        ////////

        if(pointable.touchDistance() > 0 && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
        {
            gl::color(0, 1, 0, 1 - pointable.touchDistance());
        }
        else if(pointable.touchDistance() <= 0)
        {
            gl::color(1, 0, 0, -pointable.touchDistance());
        }
        else
        {
            gl::color(0, 0, 1, .05);
        }

        gl::drawSolidCircle(Vec2f(x,y), 40);
    }

    std::stringstream ss;
    for ( auto screen : leap.locatedScreens() ) {
        ss << screen.widthPixels() << ", " << screen.heightPixels() << std::endl;
        ss << screen << std::endl;
    }
    gl::drawString( ss.str(), Vec2f( 0, 0 ), ColorA( 0, 0, 0, 1 ) );
}

CINDER_APP_NATIVE( ScreenLocationApp, RendererGl )
