#ifndef WORLDUIVIEW_CLASS_DEF
#define WORLDUIVIEW_CLASS_DEF

#include "UIView.h"
#include "Client.h"

#include "World.h"

#include "SurfaceBlock.h"

#include <cinder/app/AppBasic.h>
#include <cinder/gl/gl.h>

using namespace ci::gl;


class WorldUIView: public UIView {

private:
	Client *client;	

	int sbSize;
	float zoom;
	double xOffset, yOffset;

	bool dragging;
	int lastDragX, lastDragY;

	map<string, Texture> textures;
	

	

public:

	int sbDefaultSize;
	float maxZoom;
	float minZoom;

	WorldUIView(UIRect _rect, Client *_client);
	void mouseDown(MouseEvent &event);
	void mouseDrag(MouseEvent &event);
	void mouseUpGlobal(MouseEvent &event);

	void mouseWheel(MouseEvent &event);


	void setZoom(float _zoom);

	
	void preloadTextures();
	
	void drawBg();
	void drawBorder();
	void drawSurfaceBlock(shared_ptr<SurfaceBlock> sb, UIRect sbRect);
	void drawSurface();

	void drawParams();
	

	void update();
	void draw();


};

#endif