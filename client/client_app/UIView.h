#ifndef UIVIEW_CLASS_DEF
#define UIVIEW_CLASS_DEF

#include "Utils.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

#include "ciUI/ciUI.h"

#include <vector>

using namespace ci::gl;
using namespace std;

class UIRect {

public:
	long x, y, width, height, xEnd, yEnd;

	UIRect() {
		x = y = width = height = xEnd = yEnd = 0;
	}

	UIRect(long _x, long _y, long _w, long _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
		xEnd = x+width;
		yEnd = y+height;
	}

	bool inside(long _x, long _y){
		if(_x > x && _x < x+width && _y > y && _y < y+height)
			return true;
		return false;
	}

	string toString(){
		return "("+to_string(x)+", "+to_string(y)+", "+to_string(width)+", "+to_string(height)+")";
	}


};

class UIView {



private:
	bool visible;	
	bool interactionsEnabled;
	UIView *parentView;
	string viewId;
	string name;

	app::App *mApp;
	ci::signals::scoped_connection mCbMouseDown, mCbMouseDrag, mCbMouseUp, mCbMouseMove, mCbMouseWheel;
	ci::signals::scoped_connection mCbKeyDown, mCbKeyUp;

	
protected:
	gl::TextureFontRef textDrawer;

public:
	UIRect rect;
	vector<UIView*> subviews;

	

	UIView() {
		construct();		
	}

	UIView(UIRect _rect) {		
		construct(_rect);
	}

	void construct(UIRect _rect = UIRect()){
		rect = _rect;
		setParentView(NULL);
		mApp = app::App::get();		
		viewId = Utils::randomString(10);
		setName("view_"+viewId);

		setVisible(true);
		setInteractionsEnabled(true);

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		setFont( "Arial", 20 );		
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

	bool isThereMoreForegroundView(int x, int y, UIView *dont_check, bool check_self = false) {	
		
		if(check_self){
			bool after_dont_check = false;
			for(int i = 0; i < subviews.size(); i++){
				UIView *curSubview = subviews[i];
				if(curSubview->getViewId() == dont_check->getViewId()){
					after_dont_check = true;
					continue;
				}

				if((after_dont_check 
						|| !dont_check->isVisible()
						|| !dont_check->areInteractionsEnabled()
					)					
					&& curSubview->isVisible() && curSubview->areInteractionsEnabled()
				)

				{
					if(curSubview->rect.inside(x, y)){
						return true;
					}
				}
			}
		}

		if(this->getParentView() != NULL) {
			if(this->getParentView()->isThereMoreForegroundView(x, y, this, true)){
				return true;
			}
		}

		return false;		
	}

	virtual void mouseMove(MouseEvent &event) {}
	virtual void mouseDrag(MouseEvent &event) {}
	virtual void mouseDown(MouseEvent &event) {}
	virtual void mouseUp(MouseEvent &event) {}
	virtual void mouseWheel(MouseEvent &event) {}

	virtual void mouseMoveGlobal(MouseEvent &event) {}
	virtual void mouseDragGlobal(MouseEvent &event) {}
	virtual void mouseDownGlobal(MouseEvent &event) {}
	virtual void mouseUpGlobal(MouseEvent &event) {}
	virtual void mouseWheelGlobal(MouseEvent &event) {}

	virtual void keyDown( KeyEvent &event ) {}
	virtual void keyUp( KeyEvent &event ) {}

	bool fireMouseMove(MouseEvent event){
		int x = event.getX();
		int y = event.getY();

		mouseMoveGlobal(event);

		if(rect.inside(event.getX(), event.getY())){
			if(!isThereMoreForegroundView(x, y, this)){
				mouseMove(event);
			}
		}
		
		return false;
	}

	bool fireMouseDrag(MouseEvent event){
		int x = event.getX();
		int y = event.getY();

		mouseDragGlobal(event);

		if(rect.inside(x, y)){
			if(!isThereMoreForegroundView(x, y, this)){
				mouseDrag(event);			
			}			
		}

		return false;
	}

	bool fireMouseDown(MouseEvent event){
		int x = event.getX();
		int y = event.getY();

		mouseDownGlobal(event);

		if(rect.inside(event.getX(), event.getY())){
			if(!isThereMoreForegroundView(x, y, this)){
				mouseDown(event);
			}
		}

		return false;
	}

	bool fireMouseUp(MouseEvent event){
		int x = event.getX();
		int y = event.getY();

		mouseUpGlobal(event);

		if(rect.inside(event.getX(), event.getY())){
			if(!isThereMoreForegroundView(x, y, this)){
				mouseUp(event);
			}
		}

		return false;
	}

	bool fireMouseWheel(MouseEvent event){		
		int x = event.getX();
		int y = event.getY();

		mouseWheelGlobal(event);

		if(rect.inside(event.getX(), event.getY())){
			if(!isThereMoreForegroundView(x, y, this)){
				mouseWheel(event);		
			}
		}	

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
		mCbMouseWheel = mApp->getWindow()->getSignalMouseWheel().connect( std::bind( &UIView::fireMouseWheel, this, std::_1 ) );		
	}

	//Mouse Callbacks
	void disableMouseEventCallbacks()
	{
		mCbMouseDown.disconnect();
		mCbMouseUp.disconnect();
		mCbMouseMove.disconnect();
		mCbMouseDrag.disconnect();		
		mCbMouseWheel.disconnect();		
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
				curView->
				subviews.erase(subviews.begin()+i);				
			}
		}
	}

	void removeFromParentView () {
		parentView->removeSubivew(this);
	}

	void setVisible(bool _visible){
		visible = _visible;
		//setInteractionsEnabled(_visible);
	}
	void hide(){setVisible(false);}
	void show(){setVisible(true);}
	bool isVisible(){return visible;}

	string getViewId(){return viewId;}	

	string getName(){return name;}
	void setName(string _name){name = _name;}

	void setParentView(UIView *_parentView) {parentView = _parentView;}
	UIView* getParentView(){return parentView;}

	void setInteractionsEnabled(bool _enabled){
		interactionsEnabled = _enabled;
		if(_enabled){
			enableMouseEventCallbacks();			
		}else{
			disableMouseEventCallbacks();
		}
	}

	bool areInteractionsEnabled(){
		return interactionsEnabled;
	}

	void setFont(string name, float size){
		Font mFont = Font(name, size);
		textDrawer = gl::TextureFont::create( mFont );
	}

};

#endif