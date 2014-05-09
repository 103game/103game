#include "WorldUIView.h"
#include "Resources.h"
#include "ClientActions.h"

#include <map>

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
	textures.insert(pair<string,Texture>("grass", loadImage(loadResource(RES_TX_GRASS))));
	textures.insert(pair<string,Texture>("sand", loadImage(loadResource(RES_TX_SAND))));
	textures.insert(pair<string,Texture>("snow", loadImage(loadResource(RES_TX_SNOW))));
	textures.insert(pair<string,Texture>("lava", loadImage(loadResource(RES_TX_LAVA))));

	textures.insert(pair<string,Texture>("zombie", loadImage(loadResource(RES_TX_ZOMBIE))));
}


void WorldUIView::drawSurfaceBlock(shared_ptr<SurfaceBlock> sb, UIRect sbRect) {
	Texture tx;

	switch(sb->getSurfaceType()){
	case SURFACE_GRASS:
		tx = textures.find("grass")->second;
		break;
	case SURFACE_SAND:
		tx = textures.find("sand")->second;
		break;
	case SURFACE_SNOW:
		tx = textures.find("snow")->second;
		break;
	case SURFACE_LAVA:
		tx = textures.find("lava")->second;
		break;
	default:
		tx = textures.find("grass")->second;
	}

	

	// draw texture
	color(ColorA(1, 1, 1));
	gl::draw(tx, Rectf(sbRect.x, sbRect.y, sbRect.xEnd, sbRect.yEnd));

	// draw label
	if(zoom > 1.5){
		color(Color(1, 1, 1));
		string label = sb->getCoords().toString();		
		textDrawer->drawString(
			label, 
			Vec2f(sbRect.x+sbRect.width/2, sbRect.y+sbRect.width/2)
			);
	}	

	// draw object
	if(sb->getObject() != NULL){
		drawObject(sb->getObject(), sbRect);
	}
	
}

void WorldUIView::drawObject(shared_ptr<WorldObject> wo, UIRect woRect){
	Texture tx;

	if(wo->getClassName() == "Zombie"){
		tx = textures.find("zombie")->second;
	}else{
		tx = textures.find("zombie")->second;
	}

	// draw texture
	color(ColorA(1, 1, 1));
	gl::draw(tx, Rectf(woRect.x, woRect.y, woRect.xEnd, woRect.yEnd));
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
			shared_ptr<SurfaceBlock> sb = client->world->getSurfaceBlockByCoords(crd);

			if(sb != NULL){
				int wX = sb->getCoords().x;
				int wY = sb->getCoords().y;	

				UIRect sbRect = UIRect(
					rect.x + rect.width/2 + wX*sbSize - sbSize/2 - xOffset,
					rect.y + rect.height/2 + wY*sbSize - sbSize/2 - yOffset,
					sbSize,
					sbSize
					);

				drawSurfaceBlock(sb, sbRect);
			}
		}
	}
}


	

void WorldUIView::update() {
	UIView::update();
}

void WorldUIView::draw() {
	UIView::draw();
	if(isVisible()){	
		drawBg();
		drawSurface();
		drawBorder();
		drawParams();
	}		
}

