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

public:

	DebugConsoleUIView(Client *_client) {
		client = _client;

		gui = new ciUICanvas(0, 0, getWindowWidth(), 150);
		gui->setDrawBack(true);
		gui->setColorBack(ColorA(0, 0, 0, .3));
		gui->setFontSize(CI_UI_FONT_MEDIUM, 18);
		

		lbl = new ciUILabel(0, 0, "CONSOLE", CI_UI_FONT_MEDIUM);
		lbl->setColorFill(Color(1, 1, 1));
		gui->addWidget(lbl);
				
	}

	void update(){
		stringstream ss;

		/*logged.push_back("asdsd sad asdsadsa");
		logged.push_back("asdsd sad asdsadsa sad as");
		logged.push_back("asdsd sad asdsads sad sada");*/

		//Utils::LOG("logged size "+to_string(logged.size()));
		/*vector<string> lastLogged = Utils::getLastLogged(5);
		for(int i = 0; i < lastLogged.size(); i++){
			ss << lastLogged[i] << endl;
		}*/

		lbl->setLabel(ss.str());

		gui->update();
	}

	void draw() {
		gui->draw();
	}

};

#endif