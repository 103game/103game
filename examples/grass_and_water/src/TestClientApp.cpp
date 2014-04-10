#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <Vector>
#include <cinder/Utilities.h>
#include <cinder/Font.h>
#include "cinder/Text.h"
#include "Camer.h"
#include "ObjProto.h"
#include "StaticObj.h"
#include "TextInputBox.h"
#include "Cursor.h"
#include "Transform.h"


#include <string>

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
	gl::Texture black;
	gl::Texture object;
	gl::Texture treeimg;
	ObjProto proto;
	StaticObj tree;
	int CameraStep;
	int life;
	string pass;
	string userpass;

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
	TextInputBox *textInputBox;

	gl::Texture		mTextTexture;
	Vec2f			mSize;
	Vec2f			mPosition;

	Font			mFont;
	
};

void TestApp::keyDown( KeyEvent event )
{
	textInputBox->KeyDown(event);

	if( event.getChar() == 'f' ) {
		setFullScreen( ! isFullScreen() );
	}

	switch (event.getChar()) {
	case('w'): {
		if (proto.getY()-step + MyCam.getCameraY() > getWindowHeight()/2) {
			proto.setX(proto.getX()), proto.setY(proto.getY()-step);
		}
		else {
			tree.GoDown();
		}
		break;
			   }	
	case('a'): {
		if (proto.getX()-step +MyCam.getCameraX()> getWindowWidth()/2) {
			proto.setX(proto.getX()-step), proto.setY(proto.getY());
		}
		else {
			tree.GoRight();
		}
		break;
			   }
	case('s'): {
		if (proto.getY()+step < getWindowHeight()) {
			proto.setX(proto.getX()), proto.setY(proto.getY()+step);
		}
		else {
			tree.GoUp();
		}
		break;
			   }
	case('d'): {
		if(proto.getX()+step < getWindowWidth()) {
			proto.setX(proto.getX()+step), proto.setY(proto.getY());
		}
		else {
			tree.GoLeft();
		}
		break;
			   }
	default:
		break;
	}

}


void TestApp::setup() 
{
pass = "12345";
//userpass = myText.getText();
 textInputBox = new TextInputBox(Vec2f(100, 20), "Hello World 1 2 3 4 5 6 7 8 9 0!");


#if defined( CINDER_COCOA )
	mFont = Font( "Cochin-Italic", 32 );
#else
	mFont = Font( "Times New Roman", 32 );
#endif
	mSize = Vec2f( 100, 100 );
	//render();

proto.setX(1);
proto.setY(1);
proto.setLife(10);
tree.setX(200);
tree.setY(200);

}

void TestApp::mouseDown( MouseEvent event ) 
{
	mPosition = event.getPos();

}

void TestApp::update() 
{
	textInputBox->Update();
}

void TestApp::draw() 
{
	gl::setMatricesWindow( getWindowSize() );
	gl::enableAlphaBlending();
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::Texture grass = loadImage( "img/grass_115.jpg");
	gl::Texture snow = loadImage( "img/img1349818584.jpg");
	gl::Texture ground = loadImage( "img/ground.jpg");
	gl::Texture man = loadImage( "img/im.png");
	gl::Texture sand = loadImage( "img/sand_35.jpg");
	gl::Texture lava = loadImage( "img/lava.jpg");
	gl::Texture lifecounter = loadImage( "img/life.jpg");
	gl::Texture black = loadImage( "img/black.jpg");
	gl::Texture object = loadImage( "img/whitemen.jpg");
	gl::Texture treeimg = loadImage( "img/tree.png");
	

	
	CameraStep = 100;
	life=10;
	MyCam.setCameraCoordinates(getWindowWidth() / 2, getWindowHeight() / 2 );
	
	//gl::Texture sandim = loadImage( "img/sand.jpg");
	for (int i = getWindowWidth() / 2; i<= MyCam.getCameraX()+ getWindowWidth(); i+=CameraStep) 
	{
		
		for (int j=getWindowHeight() / 2; j<= MyCam.getCameraY()+getWindowHeight(); j+=CameraStep) 
		{
				gl::draw( sand, Vec2f(i-MyCam.getCameraX(),j-MyCam.getCameraY()));
			
		}
	}
	gl::draw( treeimg, Vec2f(tree.getX() ,tree.getY()));
	//tree.drawTree(100, 100);
	//Trees tree2;
	gl::draw( man, Vec2f(proto.getX() ,proto.getY()));
	/*if( mTextTexture )
		gl::draw( mTextTexture );*/
	for (int i = 0; i<= life; i++)
	{
		gl::draw(lifecounter, Vec2f(getWindowWidth()-20,getWindowHeight()-20-i*15));
	}
	
	
	
	//glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
       // glClear( GL_COLOR_BUFFER_BIT );
        gl::setMatricesWindow( getWindowSize() );
        gl::enableAlphaBlending( true );
        textInputBox->Render();
}
/*void TestApp::render()
{
	string txt = "Text";
	TextBox tbox = TextBox().alignment( TextBox::RIGHT ).font( mFont ).size( Vec2i( mSize.x, 100 ) );
	
	tbox.setColor( Color( 1.0f, 0.65f, 0.35f ) );
	tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
	mTextTexture = gl::Texture( tbox.render() );
	//tbox.appendText(userpass);
	//tbox.setText(userpass);
	userpass = tbox.getText();
}*/

CINDER_APP_NATIVE( TestApp, RendererGl )
