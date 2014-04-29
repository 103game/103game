#ifndef MAINUIVIEW_CLASS_DEF
#define MAINUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

class MainUIView: public UIView {

private:
	Client *client;

public:
	MainUIView (Client *_client) {
		client = _client;
	}

};

#endif