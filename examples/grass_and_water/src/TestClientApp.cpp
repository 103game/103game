#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <Vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class TestApp : public AppNative {
  public:
	gl::Texture grass;
	gl::Texture water;
	gl::Texture ground;


	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void TestApp::setup()
{
	
}

void TestApp::mouseDown( MouseEvent event )
{
}

void TestApp::update()
{
}

void TestApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	gl::Texture grass = loadImage( "img/grass_15.jpg");
	gl::Texture water = loadImage( "img/water.jpg");
	gl::Texture ground = loadImage( "img/ground.jpg");

	for (int i = 0; i<= getWindowWidth(); i+=180)
	{
		
		for (int j=0; j<= getWindowHeight(); j+=180)
		{
			
				gl::draw( grass, Vec2f(i,j));
			
		}
	}
}

CINDER_APP_NATIVE( TestApp, RendererGl )
