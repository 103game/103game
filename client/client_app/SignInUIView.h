#ifndef SIGNINUIVIEW_CLASS_DEF
#define SIGNINUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"
#include "ClientActions.h"

class SignInUIView : public UIView {
	
private:
	Client *client;
	ciUICanvas *signInGui;
	ciUITextInput *signInEmailTextField;
	ciUITextInput *signInPasswordTextField;
	ciUILabel *signInErrorsLabel;

public:

	SignInUIView(UIRect _rect,  Client *_client) {

		rect = _rect;
		client = _client;

		this->signInGui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);
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

		this->signInGui->registerUIEvents(this, &SignInUIView::signInGuiEvent);

		this->signInGui->setVisible(false);
	}

	void SignInUIView::signInGuiEvent(ciUIEvent *event)
	{
		string name = event->widget->getName(); 
		if(name == "siemail" || name == "sipassword")
		{
			ciUITextInput *emailInput = (ciUITextInput *)event->widget;
			if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
				Utils::LOG("Go login");			
				this->client->clientActions->signIn(signInEmailTextField->getTextString(), signInPasswordTextField->getTextString());
			}
		}	
	}

	void setErrorLabel(string str){
		signInErrorsLabel->setLabel(str);
	}


	void draw() {
		if(this->isVisible()){
			this->signInGui->draw();
		}		
	}

	void update() {
		if(this->isVisible()){
			this->signInGui->update();
			this->signInGui->enable();
		}else{
			this->signInGui->disable();
		}
	}

};

#endif