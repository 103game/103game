#ifndef WORLDUIVIEW_CLASS_DEF
#define WORLDUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

#include "SurfaceBlock.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

using namespace ci::gl;

class WorldUIView: public UIView {

private:
	Client *client;	
	ciUICanvas *gui;

public:

	WorldUIView(UIRect _rect, Client *_client) {
		shared_ptr<SurfaceBlock> sb = shared_ptr<SurfaceBlock>(
				new SurfaceBlock(COORDS(0, 0), SURFACE_GRASS)
			);

		this->gui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);
		this->gui->setTheme(CI_UI_THEME_MINBLACK);
		this->gui->setFontSize(CI_UI_FONT_LARGE, 100);
		this->gui->setFontSize(CI_UI_FONT_MEDIUM, 24);

		ciUILabel *signUpTitle = new ciUILabel(100, 100, "signUpTitle", "WORLD", CI_UI_FONT_LARGE);
		signUpTitle->setColorFill(ColorA(0, 0, 0, .7));
		this->gui->addWidget(signUpTitle);
		
	}

	void update() {
		gui->update();
	}

	void draw() {
		gui->draw();
		drawSolidCircle(Vec2f(200, 200), 100);
	}


};

#endif