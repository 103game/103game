#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <Vector>
#include "cinder/Text.h"
#include "Camer.h"
#include "ObjProto.h"


using namespace ci;
using namespace ci::app;
using namespace std;

#define step 100

class TestApp : public AppNative {
  public:

	gl::Texture grass;
	gl::Texture ground;
	gl::Texture man;
	gl::Texture sand;
	gl::Texture snow;
	gl::Texture lava;
	gl::Texture lifecounter;
	gl::Texture black;
	gl::Texture object;

	ObjProto proto;
	int CameraStep;
	int life;

	//void drawGrid( float step=20.0f);

	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void render();
	void drawlife();
	void keyDown( KeyEvent event );

	Vec2f  position;
	Camer MyCam;
	gl::Texture		mTextTexture;
	Vec2f			mSize;
	Vec2f			mPosition;

	Font			mFont;
};

void TestApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );

	switch (event.getChar())
	{
	case('w'): {
		if (proto.getY()-step + MyCam.getCameraY() > getWindowHeight()/2)
		{
			proto.ChangeX(proto.getX()), proto.ChangeY(proto.getY()-step);
		}
		break;}	
	case('a'): {
		if (proto.getX()-step +MyCam.getCameraX()> getWindowWidth()/2)
			proto.ChangeX(proto.getX()-step), proto.ChangeY(proto.getY());
		break;}
	case('s'): {
		if (proto.getY()+step < getWindowHeight())
			proto.ChangeX(proto.getX()), proto.ChangeY(proto.getY()+step);
		break;}
	case('d'): {
		if(proto.getX()+step < getWindowWidth())
			proto.ChangeX(proto.getX()+step), proto.ChangeY(proto.getY());
		break;}
	default:
		break;
	}
}


void TestApp::setup()
{
#if defined( CINDER_COCOA )
	mFont = Font( "Cochin-Italic", 32 );
#else
	mFont = Font( "Times New Roman", 32 );
#endif
	mSize = Vec2f( 100, 100 );
	render();

proto.ChangeX(1);
proto.ChangeY(1);
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
	gl::Texture black = loadImage( "img/black.jpg");
	gl::Texture object = loadImage( "img/obj.jpg");

	

	
	CameraStep = 100;
	life=10;
	MyCam.ChangeCameraCoordinates(getWindowWidth() / 2, getWindowHeight() / 2 );
	for (int i = getWindowWidth() / 2; i<= MyCam.getCameraX()+ getWindowWidth(); i+=CameraStep)
	{
		
		for (int j=getWindowHeight() / 2; j<= MyCam.getCameraY()+getWindowHeight(); j+=CameraStep)
		{
				gl::draw( black, Vec2f(i-MyCam.getCameraX(),j-MyCam.getCameraY()));
			
		}
	}
	gl::draw( object, Vec2f(proto.getX() ,proto.getY()));
	if( mTextTexture )
		gl::draw( mTextTexture );
	for (int i = 0; i<= life; i++)
	{
		gl::draw(lifecounter, Vec2f(getWindowWidth()-20,getWindowHeight()-20-i*15));
	}
	
	
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
