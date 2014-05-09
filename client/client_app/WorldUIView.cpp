#include "WorldUIView.h"
#include "Resources.h"
#include "ClientActions.h"

#include <map>

#include <boost/thread.hpp>

extern boost::mutex worldMutex;

map<string, Texture> sharedTextures;

WorldUIView::WorldUIView(UIRect _rect, Client *_client) {
	rect = _rect;
	client = _client;
	setName("world_view");

	minZoom = .4;
	maxZoom = 6;
	sbDefaultSize = 70;
		
	setZoom(1.0);
	xOffset = 0;
	yOffset = 0;
	dragging = false;	
		

	preloadTextures();

	enableKeyEventCallbacks();
}

void WorldUIView::mouseDown(MouseEvent &event){
	Utils::LOG("mouse_down");
	dragging = true;
	int x = event.getX();
	int y = event.getY();
	lastDragX = x;
	lastDragY = y;		
}

void WorldUIView::mouseDrag(MouseEvent &event){	
	if(dragging){
		int x = event.getX();
		int y = event.getY();				
			
		xOffset -= ((double)(x-lastDragX))*.8;
		yOffset -= ((double)(y-lastDragY))*.8;

		lastDragX = x;
		lastDragY = y;	
	}
}

void WorldUIView::mouseUpGlobal(MouseEvent &event){
	Utils::LOG("mouse_up");
	dragging = false;		
}

void WorldUIView::mouseWheel(MouseEvent &event){				
	float _zoom = zoom+event.getWheelIncrement()*.1;
	_zoom = _zoom<=minZoom?minZoom:_zoom;
	_zoom = _zoom>=maxZoom?maxZoom:_zoom;

	xOffset = ((float)xOffset)*(((float)sbDefaultSize*_zoom)/sbSize);
	yOffset = ((float)yOffset)*(((float)sbDefaultSize*_zoom)/sbSize);

	if(event.getWheelIncrement() > 0){
		xOffset += .1*((float) (lastMoveX - (rect.x +rect.width/2)))*(_zoom*sbDefaultSize)/sbSize;
		yOffset += .1*((float) (lastMoveY - (rect.y +rect.height/2)))*(_zoom*sbDefaultSize)/sbSize;
	}
	

	setZoom(_zoom);
}

void WorldUIView::keyDown( KeyEvent &event ){

	static clock_t last_pressed;
	if((clock() - last_pressed)/(double) CLOCKS_PER_SEC < .5)
		return;


	switch(event.getCode()){
	case KeyEvent::KEY_w:
		client->clientActions->sendControl("go_up");
		break;
	case KeyEvent::KEY_a:
		client->clientActions->sendControl("go_left");
		break;
	case KeyEvent::KEY_s:
		client->clientActions->sendControl("go_down");
		break;
	case KeyEvent::KEY_d:
		client->clientActions->sendControl("go_right");
		break;
	}

	last_pressed = clock();
}



void WorldUIView::setZoom(float _zoom) {
	zoom = _zoom;
	sbSize = (int)((float)sbDefaultSize*zoom);
}

void WorldUIView::drawBg(){
	color(Color(0, 140.0/255.0, 240.0/255.0));
	drawSolidRect(Rectf(rect.x, rect.y, rect.xEnd, rect.yEnd));
}	

void WorldUIView::drawBorder(){
	color(Color(0, 0, 0));
	lineWidth(5);
	drawStrokedRect(Rectf(rect.x, rect.y, rect.xEnd, rect.yEnd));
}	

void WorldUIView::drawParams(){
	color(Color(1, 1, 1));
	textDrawer->drawString("FPS: "+to_string(1/this->client->fps), Vec2f(25, 25));
	textDrawer->drawString("WPS: "+to_string(1/this->client->wps), Vec2f(25, 45));
}


void WorldUIView::preloadTextures(){
	Utils::LOG("Preloading textures");
	sharedTextures.insert(pair<string,Texture>("grass", loadImage(loadResource(RES_TX_GRASS))));
	sharedTextures.insert(pair<string,Texture>("sand", loadImage(loadResource(RES_TX_SAND))));
	sharedTextures.insert(pair<string,Texture>("snow", loadImage(loadResource(RES_TX_SNOW))));
	sharedTextures.insert(pair<string,Texture>("lava", loadImage(loadResource(RES_TX_LAVA))));
	sharedTextures.insert(pair<string,Texture>("Zombie", loadImage(loadResource(RES_TX_ZOMBIE))));
	sharedTextures.insert(pair<string,Texture>("Survivor", loadImage(loadResource(RES_TX_SURVIVOR))));
}


void WorldUIView::drawObjects(){	
	shared_ptr<World> world = client->world;
	for(vector<shared_ptr<WorldObject>>::iterator it = world->objects.begin(); it != world->objects.end(); it++){
		shared_ptr<WorldObject> wo = *it;
		COORDS crd = wo->getSurfaceBlock()->getCoords();
		UIRect woRect = UIRect(
			rect.x + rect.width/2 + crd.x*sbSize - sbSize/2 - xOffset,
			rect.y + rect.height/2 + crd.y*sbSize - sbSize/2 - yOffset,
			sbSize,
			sbSize
			);
		wo->draw(woRect);
	}
}

void WorldUIView::drawSurface(){	
	//Utils::LOG("DRAW SURFACE");
	int wXStart = (xOffset-rect.width/2)/sbSize;
	int wYStart = (yOffset-rect.height/2)/sbSize;
	int wWidth = rect.width/sbSize+3;
	int wHeight = rect.height/sbSize+3;
	COORDS crd(0, 0);

	for(int i = wXStart; i < wXStart+wWidth; i++) {
		for(int j = wYStart; j < wYStart+wHeight; j++){
			crd.x = i;
			crd.y = j;
			worldMutex.lock();
			shared_ptr<SurfaceBlock> sb = client->world->getSurfaceBlockByCoords(crd);
			worldMutex.unlock();

			if(sb != NULL){
				int wX = sb->getCoords().x;
				int wY = sb->getCoords().y;	

				UIRect sbRect = UIRect(
					rect.x + rect.width/2 + wX*sbSize - sbSize/2 - xOffset,
					rect.y + rect.height/2 + wY*sbSize - sbSize/2 - yOffset,
					sbSize,
					sbSize
					);

				sb->draw(sbRect);				
			}
		}
	}
}


	

void WorldUIView::update() {
	UIView::update();
}

void WorldUIView::draw() {	
	if(isVisible()){	
		drawBg();
		drawSurface();
		drawObjects();
		drawBorder();
		drawParams();

		UIView::draw();
	}		
	
}

