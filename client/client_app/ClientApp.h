#ifndef CLIENTAPP_CLASS_DEF
#define CLIENTAPP_CLASS_DEF




#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

#include "SignInUIView.h"
#include "SignUpUIView.h"
#include "DebugConsoleUIView.h"
#include "MainUIVIew.h"
#include "WorldUIView.h"
#include "HUDUIView.h"

#include "AlertUIView.h"

class Client;

using namespace ci;
using namespace ci::app;
using namespace std;


typedef enum {
	UI_STATE_SIGNUP,
	UI_STATE_SIGNIN,
	UI_STATE_GAME
} UI_STATE;


class ClientApp : public AppBasic
{
public:
	
	UI_STATE UIState;

	// main logic
	Client *client;

	int windW, windH;


	MainUIView *mainView;

	SignInUIView *signInView;
	SignUpUIView *signUpView;
	DebugConsoleUIView *debugConsoleView;
	WorldUIView *wv;
	HUDUIView *hudView;

	AlertUIView *alertView;



	

	// setting up and shutting down
	void prepareSettings( Settings *settings );
	void setup();

	// game loop
	void update();
	void draw();

	// application events	
	void keyDown( KeyEvent event );
	
	void setUIState(UI_STATE newState);

};


#endif