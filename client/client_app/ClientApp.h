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

	ciUICanvas *signInGui;
	ciUITextInput *signInEmailTextField;
	ciUITextInput *signInPasswordTextField;
	ciUILabel *signInErrorsLabel;
	void signInGuiEvent(ciUIEvent *event);

	ciUICanvas *signUpGui;
	ciUITextInput *signUpEmailTextField;
	ciUITextInput *signUpNameTextField;
	ciUITextInput *signUpPasswordTextField;
	ciUITextInput *signUpPasswordRepeatTextField;
	ciUILabel *signUpErrorsLabel;
	void signUpGuiEvent(ciUIEvent *event);

	

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