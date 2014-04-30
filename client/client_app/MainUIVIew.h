#ifndef MAINUIVIEW_CLASS_DEF
#define MAINUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

class MainUIView: public UIView {

private:
	Client *client;

public:
	MainUIView (UIRect _rect, Client *_client) {
		rect = _rect;
		client = _client;		
		setName("main_view");
	}

	void mouseDown(MouseEvent &event){
		Utils::LOG("Mouse down in MainView");
	}

};

#endif