
#include "ClientApp.h"

#include "Utils.h"

#include "Client.h"
#include "ClientActions.h"


void ClientApp::prepareSettings( Settings *settings )
{	
	settings->setWindowSize(1024, 768);
}

void ClientApp::setup()
{
	this->client = new Client(this);
	this->setUIState(UI_STATE_LOGIN);

	this->loginGui = new ciUICanvas(0, 0, getWindowWidth(), getWindowHeight());
	this->loginGui->setTheme(CI_UI_THEME_MINBLACK);
	this->loginGui->setFontSize(CI_UI_FONT_LARGE, 100);
	this->loginGui->setFontSize(CI_UI_FONT_MEDIUM, 24);


	ciUILabel *loginTitle = new ciUILabel(100, 200, "loginTitle", "LOGIN", CI_UI_FONT_LARGE);
	loginTitle->setColorFill(Color(0, 0, 0));
	this->loginGui->addWidget(loginTitle);

	loginEmailTextField = new ciUITextInput(100, 300, 300, "email", "", CI_UI_FONT_MEDIUM);
	loginEmailTextField->setPlaceholder("email");
	this->loginGui->addWidget(loginEmailTextField);

	loginPasswordTextField = new ciUITextInput(100, 350, 300, "password", "", CI_UI_FONT_MEDIUM);
	loginPasswordTextField->setPlaceholder("password");
	loginPasswordTextField->setPassword(true);
	this->loginGui->addWidget(loginPasswordTextField);

	loginErrorsLabel = new ciUILabel(100, 400, "errors", "Errors", CI_UI_FONT_MEDIUM);
	loginErrorsLabel->setColorFill(ColorA(1, 0, 0, .8));		
	this->loginGui->addWidget(loginErrorsLabel);

	this->loginGui->registerUIEvents(this, &ClientApp::loginGuiEvent);
}

void ClientApp::update()
{	
	this->client->mainLoop();
	loginGui->update();
}

void ClientApp::draw()
{
	gl::clear( Color( 1.0, 1.0, 1.0) ); 

	if(UIState == UI_STATE_LOGIN){		
		loginGui->draw();
	}
	
	
}

// key events
void ClientApp::keyDown( KeyEvent event )
{
	/*switch( event.getCode() )
	{
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	}*/
}

void ClientApp::loginGuiEvent(ciUIEvent *event)
{
	string name = event->widget->getName(); 
	if(name == "email" || name == "password")
	{
		ciUITextInput *emailInput = (ciUITextInput *)event->widget;
		if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
			Utils::log("Go");			
			this->client->clientActions->signIn(loginEmailTextField->getTextString(), loginPasswordTextField->getTextString());
		}
	}	
}

void ClientApp::setUIState(UI_STATE newState) {
	Utils::log("UIState changed to "+std::to_string(newState));
	this->UIState = newState;
}


CINDER_APP_BASIC( ClientApp, RendererGl )
