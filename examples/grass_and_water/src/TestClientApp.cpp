#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <Vector>
#include "cinder/Text.h"
#include "Camer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class TestApp : public AppNative {
  public:

	gl::Texture grass;
	gl::Texture ground;
	gl::Texture man;
	gl::Texture sand;
	gl::Texture snow;
	gl::Texture lava;
	gl::Texture lifecounter;

	//int CameraX;
	//int CameraY;
	int CameraStep;
	int life;

	//void drawGrid( float step=20.0f);

	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void render();
	void drawlife();

	Vec2f  position;
	Camer MyCam;
	gl::Texture		mTextTexture;
	Vec2f			mSize;
	Vec2f			mPosition;

	Font			mFont;
};

void TestApp::setup()
{
#if defined( CINDER_COCOA )
	mFont = Font( "Cochin-Italic", 32 );
#else
	mFont = Font( "Times New Roman", 32 );
#endif
	mSize = Vec2f( 100, 100 );
	render();
}

void TestApp::mouseDown( MouseEvent event )
{
	mPosition = event.getPos();

}

void TestApp::update()
{
}

void TestApp::draw()
{
	gl::setMatricesWindow( getWindowSize() );
	gl::enableAlphaBlending();
	gl::clear( Color( 0, 0, 0 ) );
	
	
	gl::Texture grass = loadImage( "img/grass_115.jpg");
	gl::Texture snow = loadImage( "img/img1349818584.jpg");
	
	gl::Texture ground = loadImage( "img/ground.jpg");
	gl::Texture man = loadImage( "img/man.png");
	gl::Texture sand = loadImage( "img/sand_35.jpg");
	gl::Texture lava = loadImage( "img/lava.jpg");
	gl::Texture lifecounter = loadImage( "img/life.jpg");

	MyCam.ChangeCameraCoordinates(getWindowWidth() / 2, getWindowHeight() / 2 );

	//CameraX = getWindowWidth() / 2;
	//CameraY = getWindowHeight() / 2;
	CameraStep = 100;
	life=10;

	for (int i = getWindowWidth() / 2; i<= MyCam.getCameraX()+ getWindowWidth(); i+=CameraStep)
	{
		
		for (int j=getWindowHeight() / 2; j<= MyCam.getCameraY()+getWindowHeight(); j+=CameraStep)
		{
			
			if (i%3==0)
				gl::draw( grass, Vec2f(i-MyCam.getCameraX(),j-MyCam.getCameraY()));
			else
				gl::draw( sand, Vec2f(i-MyCam.getCameraX(),j-MyCam.getCameraY()));
			
		}
	}
	if( mTextTexture )
		gl::draw( mTextTexture );
	for (int i = 0; i<= life; i++)
	{
		gl::draw(lifecounter, Vec2f(getWindowWidth()-20,getWindowHeight()-20-i*15));
	}
	gl::draw( man, Vec2f(MyCam.getCameraX() ,MyCam.getCameraY()));
}
void TestApp::render()
{
	string txt = "Text";
	TextBox tbox = TextBox().alignment( TextBox::RIGHT ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( txt );
	tbox.setColor( Color( 1.0f, 0.65f, 0.35f ) );
	tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
	mTextTexture = gl::Texture( tbox.render() );
}

CINDER_APP_NATIVE( TestApp, RendererGl )
