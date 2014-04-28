#ifndef UIVIEW_CLASS_DEF
#define UIVIEW_CLASS_DEF

#include "Utils.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class UIView {

private:
	bool visible;

public:

	UIView(){
		setVisible(true);
	}

	virtual void draw() = 0;
	virtual void update() = 0;

	void setVisible(bool _visible){visible = _visible;}
	bool isVisible(){return visible;}

};

#endif