#pragma once 

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

