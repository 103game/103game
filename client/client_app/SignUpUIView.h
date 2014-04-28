#ifndef SIGNUPUIVIEW_CLASS_DEF
#define SIGNUPUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"
#include "ClientActions.h"

class SignUpUIView : public UIView {

private:
	Client *client;

	ciUICanvas *gui;	
	ciUITextInput *emailTextField;
	ciUITextInput *nameTextField;
	ciUITextInput *passwordTextField;
	ciUITextInput *passwordRepeatTextField;
	ciUILabel *errorsLabel;

public:

	SignUpUIView(Client *_client) {


		client = _client;

		this->gui = new ciUICanvas(0, 0, getWindowWidth(), getWindowHeight());
		this->gui->setTheme(CI_UI_THEME_MINBLACK);
		this->gui->setFontSize(CI_UI_FONT_LARGE, 100);
		this->gui->setFontSize(CI_UI_FONT_MEDIUM, 24);

		ciUILabel *signUpTitle = new ciUILabel(100, 100, "signUpTitle", "SIGN UP", CI_UI_FONT_LARGE);
		signUpTitle->setColorFill(ColorA(0, 0, 0, .7));
		this->gui->addWidget(signUpTitle);

		emailTextField = new ciUITextInput(200, 300, 300, "suemail", "", CI_UI_FONT_MEDIUM);
		emailTextField->setPlaceholder("email");
		this->gui->addWidget(emailTextField);

		nameTextField = new ciUITextInput(200, 350, 300, "suname", "", CI_UI_FONT_MEDIUM);
		nameTextField->setPlaceholder("name");
		this->gui->addWidget(nameTextField);

		passwordTextField = new ciUITextInput(200, 400, 300, "supassword", "", CI_UI_FONT_MEDIUM);
		passwordTextField->setPlaceholder("password");
		passwordTextField->setPassword(true);
		this->gui->addWidget(passwordTextField);

		passwordRepeatTextField = new ciUITextInput(200, 450, 300, "supassword_repeat", "", CI_UI_FONT_MEDIUM);
		passwordRepeatTextField->setPlaceholder("repeat password");
		passwordRepeatTextField->setPassword(true);
		this->gui->addWidget(passwordRepeatTextField);

		errorsLabel = new ciUILabel(200, 500, "suerrors", "", CI_UI_FONT_MEDIUM);
		errorsLabel->setColorFill(ColorA(1, 0, 0, .8));		
		this->gui->addWidget(errorsLabel);

		this->gui->registerUIEvents(this, &SignUpUIView::signUpGuiEvent);
	}

	void SignUpUIView::signUpGuiEvent(ciUIEvent *event)
	{
		string name = event->widget->getName(); 
		if(name == "suemail" || name == "supassword" 
			|| name == "supassword_repeat" || name == "suname")
		{
			ciUITextInput *emailInput = (ciUITextInput *)event->widget;
			if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
				Utils::LOG("Go register");		
				this->client->clientActions->signUp(
					emailTextField->getTextString(),
					nameTextField->getTextString(),
					passwordTextField->getTextString(),
					passwordRepeatTextField->getTextString()					
					);
			}
		}	
	}

	void setErrorLabel(string str){
		errorsLabel->setLabel(str);
	}


	void draw() {
		if(this->isVisible()){
			this->gui->draw();
		}		
	}

	void update() {
		if(this->isVisible()){
			this->gui->update();
			this->gui->enable();
		}else{
			this->gui->disable();
		}
	}

};

#endif