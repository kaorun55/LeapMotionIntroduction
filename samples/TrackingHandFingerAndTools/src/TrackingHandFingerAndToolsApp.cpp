#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <cinder\Camera.h>

#include <Leap.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class TrackingHandFingerAndToolsApp : public AppNative {
public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

    cinder::Vec3f toVec3f( const Leap::Vector& vec ) {
        return cinder::Vec3f( vec.x, vec.y, vec.z );
    }

private:

    cinder::CameraPersp camera;
    Leap::Controller leap;
};

void TrackingHandFingerAndToolsApp::setup()
{
    camera.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
}

void TrackingHandFingerAndToolsApp::mouseDown( MouseEvent event )
{
}

void TrackingHandFingerAndToolsApp::update()
{
}

void TrackingHandFingerAndToolsApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
    
    camera.lookAt( cinder::Vec3f( 100, 100, 500 ), cinder::Vec3f::zero(), Vec3f::yAxis() );
    gl::setMatrices( camera );

    auto frame = leap.frame();
    auto hand = frame.hands()[0];
    if  (hand.isValid() ) {
        auto vec = toVec3f( hand.sphereCenter() );
        auto radius = hand.sphereRadius();
        gl::drawSphere( vec, radius, 50 );
    }

    for( int h = 0; h < frame.hands().count(); h++ ) {
        Leap::Hand leapHand = frame.hands()[h];

        Leap::Vector handXBasis =  leapHand.palmNormal().cross(leapHand.direction()).normalized();
        Leap::Vector handYBasis = -leapHand.palmNormal();
        Leap::Vector handZBasis = -leapHand.direction();
        Leap::Vector handOrigin =  leapHand.palmPosition();
        Leap::Matrix handTransform = Leap::Matrix(handXBasis, handYBasis, handZBasis, handOrigin);
        handTransform = handTransform.rigidInverse();

        for( int f = 0; f < leapHand.fingers().count(); f++ ) {
            Leap::Finger leapFinger = leapHand.fingers()[f];
            Leap::Vector transformedPosition = handTransform.transformPoint(leapFinger.tipPosition());
            Leap::Vector transformedDirection = handTransform.transformDirection(leapFinger.direction());
            // Do something with the transformed fingers

            gl::drawSphere( toVec3f( transformedPosition + leapHand.sphereCenter() ), 5 );
        }

        for( int t = 0; t < leapHand.tools().count(); t++ ) {
            Leap::Tool leapTool = leapHand.tools()[t];
            Leap::Vector transformedPosition = handTransform.transformPoint(leapTool.tipPosition());
            Leap::Vector transformedDirection = handTransform.transformDirection(leapTool.direction());
            // Do something with the transformed fingers

            gl::drawSphere( toVec3f( transformedPosition + leapHand.sphereCenter() ), 5 );
        }
    }
}

CINDER_APP_NATIVE( TrackingHandFingerAndToolsApp, RendererGl )
