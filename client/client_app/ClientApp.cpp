
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

	// SIGNIN FORM
	{	
		this->signInGui = new ciUICanvas(0, 0, getWindowWidth(), getWindowHeight());
		this->signInGui->setTheme(CI_UI_THEME_MINBLACK);
		this->signInGui->setFontSize(CI_UI_FONT_LARGE, 100);
		this->signInGui->setFontSize(CI_UI_FONT_MEDIUM, 24);


		ciUILabel *signInTitle = new ciUILabel(100, 100, "signInTitle", "LOGIN", CI_UI_FONT_LARGE);
		signInTitle->setColorFill(ColorA(0, 0, 0, .7));
		this->signInGui->addWidget(signInTitle);

		signInEmailTextField = new ciUITextInput(200, 300, 300, "siemail", "", CI_UI_FONT_MEDIUM);
		signInEmailTextField->setPlaceholder("email");
		this->signInGui->addWidget(signInEmailTextField);

		signInPasswordTextField = new ciUITextInput(200, 350, 300, "sipassword", "", CI_UI_FONT_MEDIUM);
		signInPasswordTextField->setPlaceholder("password");
		signInPasswordTextField->setPassword(true);
		this->signInGui->addWidget(signInPasswordTextField);

		signInErrorsLabel = new ciUILabel(200, 400, "sierrors", "", CI_UI_FONT_MEDIUM);
		signInErrorsLabel->setColorFill(ColorA(1, 0, 0, .8));		
		this->signInGui->addWidget(signInErrorsLabel);

		this->signInGui->registerUIEvents(this, &ClientApp::signInGuiEvent);
	}

	// SIGNUP FORM
	{
		this->signUpGui = new ciUICanvas(0, 0, getWindowWidth(), getWindowHeight());
		this->signUpGui->setTheme(CI_UI_THEME_MINBLACK);
		this->signUpGui->setFontSize(CI_UI_FONT_LARGE, 100);
		this->signUpGui->setFontSize(CI_UI_FONT_MEDIUM, 24);

		ciUILabel *signUpTitle = new ciUILabel(100, 100, "signUpTitle", "SIGN UP", CI_UI_FONT_LARGE);
		signUpTitle->setColorFill(ColorA(0, 0, 0, .7));
		this->signUpGui->addWidget(signUpTitle);

		signUpEmailTextField = new ciUITextInput(200, 300, 300, "suemail", "", CI_UI_FONT_MEDIUM);
		signUpEmailTextField->setPlaceholder("email");
		this->signUpGui->addWidget(signUpEmailTextField);

		signUpNameTextField = new ciUITextInput(200, 350, 300, "suname", "", CI_UI_FONT_MEDIUM);
		signUpNameTextField->setPlaceholder("name");
		this->signUpGui->addWidget(signUpNameTextField);

		signUpPasswordTextField = new ciUITextInput(200, 400, 300, "supassword", "", CI_UI_FONT_MEDIUM);
		signUpPasswordTextField->setPlaceholder("password");
		signUpPasswordTextField->setPassword(true);
		this->signUpGui->addWidget(signUpPasswordTextField);

		signUpPasswordRepeatTextField = new ciUITextInput(200, 450, 300, "supassword_repeat", "", CI_UI_FONT_MEDIUM);
		signUpPasswordRepeatTextField->setPlaceholder("repeat password");
		signUpPasswordRepeatTextField->setPassword(true);
		this->signUpGui->addWidget(signUpPasswordRepeatTextField);

		signUpErrorsLabel = new ciUILabel(200, 500, "suerrors", "", CI_UI_FONT_MEDIUM);
		signUpErrorsLabel->setColorFill(ColorA(1, 0, 0, .8));		
		this->signUpGui->addWidget(signUpErrorsLabel);

		this->signUpGui->registerUIEvents(this, &ClientApp::signUpGuiEvent);		
	}

	this->setUIState(UI_STATE_SIGNUP);
	
}

void ClientApp::update()
{	
	this->client->mainLoop();
	
	signInGui->update();			
	signUpGui->update();		
	
}

void ClientApp::draw()
{
	gl::clear( Color( 1.0, 1.0, 1.0) ); 

	signInGui->draw();	
	signUpGui->draw();
	
	
	
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

void ClientApp::signUpGuiEvent(ciUIEvent *event)
{
	string name = event->widget->getName(); 
	if(name == "suemail" || name == "supassword" 
		|| name == "supassword_repeat" || name == "suname")
	{
		ciUITextInput *emailInput = (ciUITextInput *)event->widget;
		if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
			Utils::log("Go register");		
			this->client->clientActions->signUp(
					signUpEmailTextField->getTextString(),
					signUpNameTextField->getTextString(),
					signUpPasswordTextField->getTextString(),
					signUpPasswordRepeatTextField->getTextString()					
				);
		}
	}	
}

void ClientApp::signInGuiEvent(ciUIEvent *event)
{
	string name = event->widget->getName(); 
	if(name == "siemail" || name == "sipassword")
	{
		ciUITextInput *emailInput = (ciUITextInput *)event->widget;
		if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
			Utils::log("Go login");			
			this->client->clientActions->signIn(signInEmailTextField->getTextString(), signInPasswordTextField->getTextString());
		}
	}	
}

void ClientApp::setUIState(UI_STATE newState) {
	Utils::log("UIState changed to "+std::to_string(newState));
	this->UIState = newState;

	this->signInGui->setVisible(false);
	this->signUpGui->setVisible(false);

	switch(newState){
		case UI_STATE_SIGNIN:
			this->signInGui->setVisible(true);
			break;
		case UI_STATE_SIGNUP:
			this->signUpGui->setVisible(true);
			break;
	}
}


CINDER_APP_BASIC( ClientApp, RendererGl )
