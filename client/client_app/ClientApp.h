#ifndef CLIENTAPP_CLASS_DEF
#define CLIENTAPP_CLASS_DEF

#pragma warning(disable: 4305)



#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

class Client;

using namespace ci;
using namespace ci::app;
using namespace std;


typedef enum {
	UI_STATE_LOGIN,
	UI_STATE_GAME
} UI_STATE;


class ClientApp : public AppBasic
{
public:
	
	UI_STATE UIState;

	// main logic
	Client *client;

	ciUICanvas *loginGui;
	ciUITextInput *loginEmailTextField;
	ciUITextInput *loginPasswordTextField;
	ciUILabel *loginErrorsLabel;

	

	// setting up and shutting down
	void prepareSettings( Settings *settings );
	void setup();

	// game loop
	void update();
	void draw();

	// application events	
	void keyDown( KeyEvent event );
	void loginGuiEvent(ciUIEvent *event);
	void setUIState(UI_STATE newState);

};


#endif