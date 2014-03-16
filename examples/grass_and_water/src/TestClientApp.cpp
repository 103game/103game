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
	gl::Texture texture;
	gl::Texture texture2;
	gl::Texture sand;


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
	gl::Texture texture = loadImage( "grass.jpg");
	gl::Texture texture2 = loadImage( "water.jpg");
	gl::Texture sand = loadImage( "ground.jpg");

	for (int i = 0; i<= getWindowWidth(); i+=50)
	{
		
		for (int j=0; j<= getWindowHeight(); j+=50)
		{
			if(i%3==0)
			{
				gl::draw( texture2, Vec2f(i,j));
			}
			else
			{
				gl::draw( texture, Vec2f(i,j));
			}
		}
	}
}

CINDER_APP_NATIVE( TestApp, RendererGl )
