#include "WorldUIView.h"
#include "Resources.h"

#include <map>

WorldUIView::WorldUIView(UIRect _rect, Client *_client) {
	rect = _rect;
	client = _client;

	minZoom = .1;
	maxZoom = 6;
	sbDefaultSize = 70;
		
	setZoom(1.0);
	xOffset = 0;
	yOffset = 0;
	dragging = false;	
		

	world = shared_ptr<World>(new World());

	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 130; j++){
			world->sbMap.insert(
				pair<COORDS, shared_ptr<SurfaceBlock>>(
					COORDS(i, j),
					shared_ptr<SurfaceBlock>(new SurfaceBlock(COORDS(i, j), SURFACE_GRASS))
					)
				);
		}
	}

	Utils::LOG(rect.toString());

	preloadTextures();

	/*addSubview(new SurfaceBlockUIView(
				UIRect(300, 300, 150, 150),
				sb
		));*/		
		
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
	float _zoom = zoom+event.getWheelIncrement()*.2;
	Utils::LOG("new zoom "+to_string(_zoom));
	_zoom = _zoom<=minZoom?minZoom:_zoom;
	_zoom = _zoom>=maxZoom?maxZoom:_zoom;
	setZoom(_zoom);

}




void WorldUIView::setZoom(float _zoom) {
	zoom = _zoom;
	sbSize = (int)((float)sbDefaultSize*zoom);
}

void WorldUIView::drawBg(){
	color(Color(0, 140.0/255.0, 240.0/255.0));
	drawSolidRect(Rectf(rect.x, rect.y, rect.xEnd, rect.yEnd));
}	

/*	void drawLoading(string str){
	color(Color(0, 0, 0));
	Vec2f strSize = textDrawer->measureString(str);		
	textDrawer->drawString(
		str, 
		Vec2f(rect.x+rect.width/2-strSize.x/2, rect.y+rect.width/2+strSize.y/2)
		);
}*/

void WorldUIView::preloadTextures(){
	Utils::LOG("Preloading textures");
	textures.insert(pair<string,Texture>("grass", loadImage(loadResource(RES_TX_GRASS))));
	textures.insert(pair<string,Texture>("sand", loadImage(loadResource(RES_TX_SAND))));
	textures.insert(pair<string,Texture>("snow", loadImage(loadResource(RES_TX_SNOW))));
	textures.insert(pair<string,Texture>("lava", loadImage(loadResource(RES_TX_LAVA))));
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
}

void WorldUIView::drawSurface(){	

	int wXStart = (xOffset-rect.width/2)/sbSize;
	int wYStart = (yOffset-rect.height/2)/sbSize;
	int wWidth = rect.width*1.5/sbSize;
	int wHeight = rect.height*1.5/sbSize;
		
	for(int i = wXStart; i < wXStart+wWidth; i++) {
		for(int j = wYStart; j < wYStart+wHeight; j++){
			COORDS crd(i, j);
			shared_ptr<SurfaceBlock> sb = world->getSurfaceBlockByCoords(crd);

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
	}		
}

