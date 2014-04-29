#ifndef UIVIEW_CLASS_DEF
#define UIVIEW_CLASS_DEF

#include "Utils.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class UIView {

private:
	bool visible;
	vector<UIView*> subviews;
	UIView *parentView;
	string viewId;

public:

	UIView(){
		setVisible(true);
		viewId = Utils::randomString(10);
	}

	virtual void draw() {
		for(int i = 0; i < subviews.size(); i++){			
			subviews[i]->draw();
		}
	}

	virtual void update() {
		for(int i = 0; i < subviews.size(); i++){
			subviews[i]->update();
		}
	}
	
	void addSubview(UIView *view){
		view->parentView = this;
		subviews.push_back(view);
	}

	void removeSubivew(UIView *view) {
		for(int i = 0; i < subviews.size(); i++){
			UIView *curView = subviews[i];
			if(curView->viewId == view->viewId){
				subviews.erase(subviews.begin()+i);
			}
		}
	}

	void removeFromParentView () {
		parentView->removeSubivew(this);
	}

	void setVisible(bool _visible){visible = _visible;}
	bool isVisible(){return visible;}


};

#endif