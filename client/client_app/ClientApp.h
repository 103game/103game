#ifndef CLIENTAPP_CLASS_DEF
#define CLIENTAPP_CLASS_DEF

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>
#include "Common.h"

#include "Client.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class ClientApp : public AppBasic
{
public:
	
	// main logic
	Client *client;

	// setting up and shutting down
	void prepareSettings( Settings *settings );
	void setup();

	// game loop
	void update();
	void draw();

	// application events	
	void keyDown( KeyEvent event );
protected:
	float getSecondsSinceMidnight();

};

void ClientApp::prepareSettings( Settings *settings )
{
	settings->setTitle("Analog Clock");
	settings->setWindowSize(300, 300);
}

void ClientApp::setup()
{	
	DBOUT("That's how we output useful information in windowed app (without console)");
		
	this->client = new Client(this);
}

// game loop

void ClientApp::update()
{
	// instead of in the update method, we will do all 'animation' in the draw method
}

void ClientApp::draw()
{
	// clear the window with a black background
	gl::clear( Color::black() ); 

	// get the center of the window
	Vec2f center = 0.5f * Vec2f( getWindowSize() );

	// set current drawing color to white
	gl::color( Color::white() );

	// draw the 12 hour digits
	for(int h=0;h<12;++h)
	{
		// store the current transformation,
		// so we can 'undo' the translation and rotation
		// to get ready for the next digit
		gl::pushModelView();

		// draw a rectangle, rotated around the clock's center
		gl::translate( center );
		gl::rotate( h * 30.0f ); // 30 degrees per hour
		gl::drawSolidRect( Rectf(-3, -110, 3, -90) );

		// restore the transformations
		gl::popModelView();
	}

	// get the number of seconds since midnight
	float seconds = getSecondsSinceMidnight();

	// draw the long hand for the minutes
	gl::pushModelView();
		gl::translate( center );
		gl::rotate( seconds * (360.0f / 3600.0f) ); // 360 degrees per 3600 seconds
		gl::drawSolidRect( Rectf(-2, -100, 2, 15) );
	gl::popModelView();

	// draw the short hand for the hours
	gl::pushModelView();
		gl::translate( center );
		gl::rotate( seconds * (30.0f / 3600.0f) ); // 30 degrees per 3600 seconds
		gl::drawSolidRect( Rectf(-2, -60, 2, 15) );
	gl::popModelView();

	// set the current drawing color to red
	gl::color( Color(1, 0, 0) );

	// draw the hand for the seconds
	gl::pushModelView();
		gl::translate( center );
		gl::rotate( seconds * (360.0f / 60.0f) ); // 360 degrees per 60 seconds
		gl::drawSolidRect( Rectf(-1, -100, 1, 15) );
		gl::drawSolidCircle( Vec2f::zero(), 6, 24 );
	gl::popModelView();
}

// key events

void ClientApp::keyDown( KeyEvent event )
{
	switch( event.getCode() )
	{
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	}
}

//

float ClientApp::getSecondsSinceMidnight()
{
	float seconds = 0.0f;

	// this code only works on Windows
#if defined( CINDER_MSW )
	SYSTEMTIME now;
	::GetLocalTime(&now);

	seconds = float(now.wHour * 3600.0 + now.wMinute * 60.0 + now.wSecond);
#endif	

	return seconds;
}


CINDER_APP_BASIC( ClientApp, RendererGl )

#endif