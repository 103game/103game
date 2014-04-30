#ifndef SURFACEBLOCKUIVIEW_CLASS_DEF
#define SURFACEBLOCKUIVIEW_CLASS_DEF

#include "UIView.h"
#include "SurfaceBlock.h"

class SurfaceBlockUIView: public UIView {



private:
	SurfaceBlock *sb;
	ciUICanvas *gui;
	ciUILabel *label;

public:

	SurfaceBlockUIView (UIRect _rect, SurfaceBlock * _sb) {
		SurfaceBlockUIView::SurfaceBlockUIView(_rect);
		sb = _sb;
	}

	SurfaceBlockUIView(UIRect _rect){
		sb = NULL;
		rect = _rect;
		gui = new ciUICanvas(rect.x, rect.y, rect.width, rect.height);
		gui->setColorBack(Color(.2, .2, .2));
		
		int fontSize = (int)rect.width/6;
		gui->setFontSize(CI_UI_FONT_MEDIUM, fontSize);
		label = new ciUILabel(0, rect.height/2-fontSize/2, rect.width, "sbLabel", "SurfaceBlock", CI_UI_FONT_MEDIUM);		
		label->setColorFill(Color(1, 1, 1));
		gui->addWidget(label);
		
	}

	

	void update() {
		if(sb != NULL){

		}else{

		}
		
		gui->update();
	}

	void draw() {
		if(isVisible()){
			gui->draw();
		}
	}
};

#endif