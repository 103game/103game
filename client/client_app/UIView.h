#ifndef UIVIEW_CLASS_DEF
#define UIVIEW_CLASS_DEF

#include "Utils.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

#include <vector>


using namespace std;

class UIRect {

public:
	long x, y, width, height;

	UIRect() {
		x = y = width = height = 0;
	}

	UIRect(long _x, long _y, long _w, long _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}

	bool inside(long _x, long _y){
		if(_x > x && _x < x+width && _y > y && _y < y+height)
			return true;
		return false;
	}


};

class UIView {

private:
	bool visible;
	vector<UIView*> subviews;
	UIView *parentView;
	string viewId;

	app::App *mApp;
	ci::signals::scoped_connection mCbMouseDown, mCbMouseDrag, mCbMouseUp, mCbMouseMove;
	ci::signals::scoped_connection mCbKeyDown, mCbKeyUp;

public:
	UIRect rect;

	UIView() {
		rect = UIRect();
		mApp = app::App::get();
		setVisible(true);
		viewId = Utils::randomString(10);
	}

	UIView(UIRect _rect) {
		UIView::UIView();
		rect = _rect;
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

	virtual void mouseMove(MouseEvent &event) {}
	virtual void mouseDrag(MouseEvent &event) {}
	virtual void mouseDown(MouseEvent &event) {}
	virtual void mouseUp(MouseEvent &event) {}
	virtual void mouseWheel(MouseEvent &event) {}

	virtual void keyDown( KeyEvent &event ) {}
	virtual void keyUp( KeyEvent &event ) {}

	bool fireMouseMove(MouseEvent event){
		if(rect.inside(event.getX(), event.getY())){
			mouseMove(event);
		}
		
		return false;
	}

	bool fireMouseDrag(MouseEvent event){
		if(rect.inside(event.getX(), event.getY())){
			mouseDrag(event);			
		}

		return false;
	}

	bool fireMouseDown(MouseEvent event){
		if(rect.inside(event.getX(), event.getY())){
			mouseDown(event);
		}

		return false;
	}

	bool fireMouseUp(MouseEvent event){
		if(rect.inside(event.getX(), event.getY())){
			mouseUp(event);
		}

		return false;
	}

	bool fireMouseWheel(MouseEvent event){		
		mouseWheel(event);		
		return false;
	}

	bool fireKeyDown(KeyEvent event){
		keyDown(event);
		return false;
	}

	bool fireKeyUp(KeyEvent event){
		keyUp(event);
		return false;
	}


	void enableMouseEventCallbacks()
	{
		mCbMouseDown = mApp->getWindow()->getSignalMouseDown().connect( std::bind( &UIView::fireMouseDown, this, std::_1 ) );
		mCbMouseUp = mApp->getWindow()->getSignalMouseUp().connect( std::bind( &UIView::fireMouseUp, this, std::_1 ) );
		mCbMouseMove = mApp->getWindow()->getSignalMouseMove().connect( std::bind( &UIView::fireMouseMove, this, std::_1 ) );
		mCbMouseDrag = mApp->getWindow()->getSignalMouseDrag().connect( std::bind( &UIView::fireMouseDrag, this, std::_1 ) );
	}

	//Mouse Callbacks
	void disableMouseEventCallbacks()
	{
		mCbMouseDown.disconnect();
		mCbMouseUp.disconnect();
		mCbMouseMove.disconnect();
		mCbMouseDrag.disconnect();
	}	

	//KeyBoard Callbacks
	void enableKeyEventCallbacks()
	{
		mCbKeyDown = mApp->getWindow()->getSignalKeyDown().connect( std::bind( &UIView::fireKeyDown, this, std::_1 ) );
		mCbKeyUp = mApp->getWindow()->getSignalKeyUp().connect( std::bind( &UIView::fireKeyUp, this, std::_1 ) );
	}

	//KeyBoard Callbacks
	void disableKeyEventCallbacks()
	{
		mCbKeyDown.disconnect();
		mCbKeyUp.disconnect();       
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
	void hide(){visible = false;}
	void show(){visible = true;}
	bool isVisible(){return visible;}


};

#endif