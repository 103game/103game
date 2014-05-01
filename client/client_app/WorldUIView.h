#ifndef WORLDUIVIEW_CLASS_DEF
#define WORLDUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

#include "World.h"

#include "SurfaceBlock.h"
#include "SurfaceBlockUIView.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

using namespace ci::gl;

#include <boost/thread/thread.hpp>


class WorldUIView: public UIView {

private:
	Client *client;	

	shared_ptr<World> world;

	int sbSize;
	float zoom;
	double xOffset, yOffset;

	bool dragging;
	int lastDragX, lastDragY;

	gl::TextureFontRef textureFont;

	vector<shared_ptr<SurfaceBlockUIView>> availableSbDrawingViews;	

public:

	int sbDefaultSize;
	float maxZoom;
	float minZoom;

	WorldUIView(UIRect _rect, Client *_client) {
		rect = _rect;
		client = _client;

		minZoom = 1;
		maxZoom = 6;
		sbDefaultSize = 70;
		
		setZoom(1.0);
		xOffset = 0;
		yOffset = 0;
		dragging = false;

		Font mFont = Font( "Arial", rect.width/6 );
		textureFont = gl::TextureFont::create( mFont );
		

		world = shared_ptr<World>(new World());

		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				world->surfaceBlocks.push_back(shared_ptr<SurfaceBlock>(new SurfaceBlock(COORDS(i, j))));
			}
		}

		/*addSubview(new SurfaceBlockUIView(
					UIRect(300, 300, 150, 150),
					sb
			));*/		
		
	}

	void mouseDown(MouseEvent &event){
		Utils::LOG("mouse_down");
		dragging = true;
		int x = event.getX();
		int y = event.getY();
		lastDragX = x;
		lastDragY = y;		
	}

	void mouseDrag(MouseEvent &event){	
		if(dragging){
			int x = event.getX();
			int y = event.getY();				
			
			xOffset -= ((double)(x-lastDragX))*.8;
			yOffset -= ((double)(y-lastDragY))*.8;

			lastDragX = x;
			lastDragY = y;	
		}
	}

	void mouseUpGlobal(MouseEvent &event){
		Utils::LOG("mouse_up");
		dragging = false;		
	}

	void mouseWheel(MouseEvent &event){				
		float _zoom = zoom+event.getWheelIncrement()*.2;
		Utils::LOG("new zoom "+to_string(_zoom));
		_zoom = _zoom<=minZoom?minZoom:_zoom;
		_zoom = _zoom>=maxZoom?maxZoom:_zoom;
		setZoom(_zoom);
	}




	void setZoom(float _zoom) {
		zoom = _zoom;
		sbSize = (int)((float)sbDefaultSize*zoom);
	}

	void drawBg(){
		color(Color(.8, .8, .8));
		drawSolidRect(Rectf(rect.x, rect.y, rect.xEnd, rect.yEnd));
	}	

	void drawLoading(string str){
		color(Color(0, 0, 0));
		Vec2f strSize = textureFont->measureString(str);		
		textureFont->drawString(
			str, 
			Vec2f(rect.x+rect.width/2-strSize.x/2, rect.y+rect.width/2+strSize.y/2)
			);
	}


	void drawSurface(){		

		int curMaxBlocks = 9;//(int)((float)rect.width*rect.height)/((float)sbDefaultSize*minZoom*sbDefaultSize*minZoom);		
		if(availableSbDrawingViews.size() < curMaxBlocks){			
			while(availableSbDrawingViews.size() < curMaxBlocks){
				Utils::LOG("Adding sbuiview ("+to_string(availableSbDrawingViews.size()+1)+")");
				auto sbui = shared_ptr<SurfaceBlockUIView>(new SurfaceBlockUIView());
				sbui->setInteractionsEnabled(false);
				availableSbDrawingViews.push_back(sbui);
			}
		}
		
			
		

		int usedSbViews = 0;
		
		for(int i = 0; i < world->surfaceBlocks.size(); i++) {
			
			shared_ptr<SurfaceBlock> sb = world->surfaceBlocks[i];
			int wX = sb->getCoords().x;
			int wY = sb->getCoords().y;						

			if(
				rect.x + rect.width/2 + wX*sbSize - sbSize/2 - xOffset + sbSize >= rect.x
				&& rect.x + rect.width/2 + wX*sbSize - sbSize/2 - xOffset - sbSize <= rect.x + rect.width
				&& rect.y + rect.width/2 + wY*sbSize - sbSize/2 - yOffset + sbSize >= rect.y
				&& rect.y + rect.width/2 + wY*sbSize - sbSize/2 - yOffset -2*sbSize <= rect.y + rect.height
			){
				//Utils::LOG("Available sbDrawViews: "+to_string(availableSbDrawingViews.size()-usedSbViews));
				//Utils::LOG("draw "+to_string(wX)+","+to_string(wY));
				UIRect sbViewRect = UIRect(
						rect.x + rect.width/2 + wX*sbSize - sbSize/2 - xOffset,
						rect.y + rect.height/2 + wY*sbSize - sbSize/2 - yOffset,
						sbSize,
						sbSize
					);
				if(usedSbViews < availableSbDrawingViews.size()){
					shared_ptr<SurfaceBlockUIView> sbView = availableSbDrawingViews[usedSbViews++];
					sbView->setSurfaceBlock(sb);
					sbView->setRect(sbViewRect);					
					sbView->draw();
				}				
			}
		}
	}


	

	void update() {
		UIView::update();
	}

	void draw() {
		UIView::draw();
		if(isVisible()){	
			drawBg();
			drawSurface();
		}		
	}


};

#endif