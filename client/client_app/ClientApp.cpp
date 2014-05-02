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
	windW = getWindowWidth();
	windH = getWindowHeight();
	

	this->mainView = new MainUIView(UIRect(0, 0, windW, windH), this->client);

	

	this->signInView = new SignInUIView(UIRect(0, 0, windW, windH), this->client);
	this->signUpView = new SignUpUIView(UIRect(0, 0, windW, windH), this->client);
	this->debugConsoleView = new DebugConsoleUIView(UIRect(0, 0, windW, windH), this->client);
	this->debugConsoleView->hide();

	this->mainView->addSubview(this->signInView);
	this->mainView->addSubview(this->signUpView);


	wv = new WorldUIView(UIRect(0, 0, windW, windH), this->client);

	this->mainView->addSubview(wv);
	this->mainView->addSubview(this->debugConsoleView);

	

	this->setUIState(UI_STATE_GAME);	

//	this->signInView->removeFromParentView();
}

void ClientApp::update()
{	
	windW = getWindowWidth();
	windH = getWindowHeight();

	this->client->mainLoop();
		
	this->mainView->update();
}

void ClientApp::draw()
{
	gl::clear( Color( 1.0, 1.0, 1.0) ); 

	this->mainView->draw();
	
	this->client->fps = (clock() - this->client->last_frame_update)/(double) CLOCKS_PER_SEC;
	this->client->last_frame_update = clock();
	
}

// key events
void ClientApp::keyDown( KeyEvent event )
{
	switch( event.getCode() )
	{
		case KeyEvent::KEY_ESCAPE:
			quit();
			break;
		case KeyEvent::KEY_BACKQUOTE:
			if(debugConsoleView->isVisible())
				this->debugConsoleView->hide();
			else
				this->debugConsoleView->show();
			break;
	}
}


void ClientApp::setUIState(UI_STATE newState) {
	Utils::LOG("UIState changed to "+std::to_string(newState));
	this->UIState = newState;

	this->signInView->setVisible(false);
	this->signUpView->setVisible(false);
	this->wv->setVisible(false);

	switch(newState){
		case UI_STATE_SIGNIN:
			this->signInView->setVisible(true);
			break;
		case UI_STATE_SIGNUP:
			this->signUpView->setVisible(true);
			break;
		case UI_STATE_GAME:
			this->wv->setVisible(true);
			break;
	}
}


CINDER_APP_BASIC( ClientApp, RendererGl )
