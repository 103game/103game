#ifndef DEBUGCONSOLEUIVIEW_CLASS_DEF
#define DEBUGCONSOLEUIVIEW_CLASS_DEF


#include <string>
#include <sstream>

#include "Client.h"




using namespace std;

class DebugConsoleUIView: public UIView {

private:
	Client *client;
	ciUICanvas *gui;
	ciUILabel *lbl;

	bool debug_show;

public:

	DebugConsoleUIView(UIRect _rect, Client *_client) {

		rect = _rect;
		client = _client;

		gui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);
		gui->setDrawBack(true);
		gui->setColorBack(ColorA(0, 0, 0, .5));
		gui->setFontSize(CI_UI_FONT_MEDIUM, 25);
		gui->setEmbedded(true);
		

		lbl = new ciUILabel(10, 10, "", CI_UI_FONT_MEDIUM);
		lbl->setColorFill(Color(1, 1, 1));		
		lbl->setColorOutline(Color(1, 1, 1));
		gui->addWidget(lbl);
				
	}

	void update(){
		if(debug_show){
			stringstream ss;

			vector<string> lastLogged = Utils::getLastLogged(30);
			reverse(lastLogged.begin(), lastLogged.end());

			for(int i = 0; i < lastLogged.size(); i++){
				ss << lastLogged[i] << endl;
			}

			lbl->setLabel(ss.str());

			gui->update();
		}		
	}

	void draw() {
		if(debug_show){
			gui->draw();
		}	
	}

	bool isOpened(){
		return debug_show;
	}

	void show(){
		debug_show = true;
	}

	void hide(){
		debug_show = false;
	}

};

#endif