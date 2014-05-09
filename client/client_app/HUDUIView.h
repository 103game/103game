#ifndef HUDUIVIEW_CLASS_DEF
#define HUDUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

#define LB_WIDTH 100
#define LB_HEIGHT 20

class HUDUIView: public UIView {

	Client *client;

public:

	HUDUIView(UIRect _rect,  Client *_client){
		rect = _rect;
		client = _client;
		setInteractionsEnabled(false);
	}
	

	void drawLifebar(){
		if(!client->userCreature)
			return;
			

		int life = client->userCreature->getLife();
		color(ColorA(0, 0, 0, .8));
		
		lineWidth(3);
		UIRect lbRect = UIRect(rect.x + rect.width-20-LB_WIDTH, rect.y+20, LB_WIDTH, LB_HEIGHT);
		drawStrokedRect(Rectf(lbRect.x, lbRect.y, lbRect.xEnd, lbRect.yEnd));
		lbRect = UIRect(rect.x + rect.width-20-LB_WIDTH, rect.y+20, LB_WIDTH*(life/100.0), LB_HEIGHT);	

		if(life > 30){
			color(Color(0, 1, 0));
		}else{
			color(Color(1, 0, 0));
		}
		
		drawSolidRect(Rectf(lbRect.x, lbRect.y, lbRect.xEnd, lbRect.yEnd));

	}

	void draw(){
		if(isVisible()){
			drawLifebar();
		}
	}

	void update(){

	}

};


#endif