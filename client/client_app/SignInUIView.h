#ifndef SIGNINUIVIEW_CLASS_DEF
#define SIGNINUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"
#include "ClientActions.h"
#include "Resources.h"

class SignInUIView : public UIView {
	
private:
	Client *client;
	ciUICanvas *gui;
	ciUITextInput *emailTextField;
	ciUITextInput *passwordTextField;
	ciUILabel *errorsLabel;

	gl::Texture mImage;

public:

	SignInUIView(UIRect _rect,  Client *_client) {

		rect = _rect;
		client = _client;
		setName("signin_view");

		this->gui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);
		this->gui->setTheme(CI_UI_THEME_MINBLACK);
		this->gui->setFontSize(CI_UI_FONT_LARGE, 100);
		this->gui->setFontSize(CI_UI_FONT_MEDIUM, 24);

		mImage = loadImage(loadResource(RES_TX_ZOMBIE));
		ciUIImage *image = new ciUIImage(600, 200, 300, 300, &mImage, "");

		this->gui->addWidget(image);

		ciUILabel *signInTitle = new ciUILabel(100, 100, "signInTitle", "LOGIN", CI_UI_FONT_LARGE);
		signInTitle->setColorFill(ColorA(0, 0, 0, .7));
		this->gui->addWidget(signInTitle);

		emailTextField = new ciUITextInput(200, 300, 300, "siemail", "", CI_UI_FONT_MEDIUM);
		emailTextField->setPlaceholder("email");
		this->gui->addWidget(emailTextField);

		passwordTextField = new ciUITextInput(200, 350, 300, "sipassword", "", CI_UI_FONT_MEDIUM);
		passwordTextField->setPlaceholder("password");
		passwordTextField->setPassword(true);
		this->gui->addWidget(passwordTextField);

		errorsLabel = new ciUILabel(200, 400, "sierrors", "", CI_UI_FONT_MEDIUM);
		errorsLabel->setColorFill(ColorA(1, 0, 0, .8));		
		this->gui->addWidget(errorsLabel);

		this->gui->registerUIEvents(this, &SignInUIView::guiEvent);

		this->gui->setVisible(false);
	}

	void SignInUIView::guiEvent(ciUIEvent *event)
	{
		string name = event->widget->getName(); 
		if(name == "siemail" || name == "sipassword")
		{
			ciUITextInput *emailInput = (ciUITextInput *)event->widget;
			if(emailInput->getTriggerType() == CI_UI_TEXTINPUT_ON_ENTER){
				Utils::LOG("Go login");			
				this->client->clientActions->signIn(emailTextField->getTextString(), passwordTextField->getTextString());
			}
		}	
	}

	void setErrorLabel(string str){
		errorsLabel->setLabel(str);
	}

	void mouseDown(MouseEvent &event){
		Utils::LOG("Mouse down in SignInView");
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