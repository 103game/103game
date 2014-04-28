#include "CompilerOptions.h"
#include "ClientApp.h"

#include "Utils.h"

#include "Client.h"
#include "ClientActions.h"


void ClientApp::prepareSettings( Settings *settings )
{	
	settings->setWindowSize(1024, 768);
	settings->setResizable(false);
}

void ClientApp::setup()
{
	this->client = new Client(this);	

	

	this->signInView = new SignInUIView(this->client);
	this->signUpView = new SignUpUIView(this->client);
	this->debugConsoleView = new DebugConsoleUIView(this->client);

	this->setUIState(UI_STATE_SIGNUP);	
}

void ClientApp::update()
{	
	this->client->mainLoop();
		
	this->signInView->update();
	this->signUpView->update();
	this->debugConsoleView->update();
}

void ClientApp::draw()
{
	gl::clear( Color( 1.0, 1.0, 1.0) ); 

	this->signInView->draw();

	this->signUpView->draw();

	this->debugConsoleView->draw();
	
	
	
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


void ClientApp::setUIState(UI_STATE newState) {
	Utils::LOG("UIState changed to "+std::to_string(newState));
	this->UIState = newState;

	this->signInView->setVisible(false);
	this->signUpView->setVisible(false);

	switch(newState){
		case UI_STATE_SIGNIN:
			this->signInView->setVisible(true);
			break;
		case UI_STATE_SIGNUP:
			this->signUpView->setVisible(true);
			break;
	}
}


CINDER_APP_BASIC( ClientApp, RendererGl )
