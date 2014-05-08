#ifndef ALERTUIVIEW_CLASS_DEF
#define ALERTUIVIEW_CLASS_DEF


#include "UIView.h"
#include "Client.h"





using namespace std;

class AlertUIView: public UIView {

private:
	Client *client;
	ciUICanvas *gui;

	function<void(void)> okCallback;
	function<void(void)> cancelCallback;


public:

	AlertUIView( UIRect _rect, Client *_client) {
				
		client = _client;
		rect = _rect;
		okCallback = nullptr;
		cancelCallback = nullptr;

		setName("alert_uiview");
		 
		gui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);

		gui->setTheme(CI_UI_THEME_MINBLACK);
		
		gui->setDrawBack(true);
		gui->setColorBack(ColorA(0, 0, 0, .5));

		gui->setFontSize(CI_UI_FONT_LARGE, 60);
		gui->setFontSize(CI_UI_FONT_MEDIUM, 25);
		
	}

	void setAlert(string title, string text, function<void(void)>_okCallback = nullptr, function<void(void)>_cancelCallback = nullptr) {
		

		ciUILabel *ttl = new ciUILabel(0, 0, "", CI_UI_FONT_LARGE);				
		ttl->setRect(rect.width/2, rect.height/2-100, rect.width/3, 40);
		gui->addWidget(ttl);
		ttl->setLabel(title);
		

		ciUILabel *txt = new ciUILabel(0, 0, "", CI_UI_FONT_MEDIUM);		
		txt->setRect(rect.width/2, rect.height/2, rect.width/3, 200);
		gui->addWidget(txt);
		txt->setLabel(text);

		ciUIButton *okButton = new ciUIButton(rect.width/2-125, rect.height/2+300, 90, 40, false, "Ok", CI_UI_FONT_LARGE);				
		gui->addWidget(okButton);
		ciUIButton *cancelButton = new ciUIButton(rect.width/2+5, rect.height/2+300, 90, 40, false, "Cancel", CI_UI_FONT_LARGE);
		gui->addWidget(cancelButton);	
		gui->registerUIEvents(this, &AlertUIView::guiEvent);

		this->setVisible(true);
		
	}

	void guiEvent(ciUIEvent *event)
	{
		string name = event->widget->getName(); 
		if(name == "Ok"){
			if(okCallback != nullptr){
				okCallback();
			}			
			this->setVisible(false);
		}
		else if(name == "Cancel"){
			if(cancelCallback != nullptr){
				cancelCallback();
			}

			this->setVisible(false);
		}
	}


	void update(){
		
	}

	void mouseDown(MouseEvent &event){
		
	}	


	void draw() {	
		if(isVisible()){
			gui->draw();		
		}
	}	

	void setOkCallback(function<void(void)> _cb){okCallback = _cb;}
	void setCancelCallback(function<void(void)> _cb){cancelCallback = _cb;}

};

#endif