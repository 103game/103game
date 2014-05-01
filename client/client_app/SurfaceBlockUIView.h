#ifndef SURFACEBLOCKUIVIEW_CLASS_DEF
#define SURFACEBLOCKUIVIEW_CLASS_DEF

#include "UIView.h"
#include "SurfaceBlock.h"

#include "Resources.h"




class SurfaceBlockUIView: public UIView {



private:
	shared_ptr<SurfaceBlock> sb;
	gl::TextureFontRef textureFont;
	
public:

	SurfaceBlockUIView (UIRect _rect, shared_ptr<SurfaceBlock> _sb) {		
		construct(_rect, _sb);
	}

	SurfaceBlockUIView(UIRect _rect){		
		construct(_rect);
	}

	SurfaceBlockUIView(){
		construct();
	}

	void construct(UIRect _rect = UIRect(0, 0, 0, 0), shared_ptr<SurfaceBlock> _sb = NULL) {
		sb = _sb;
		rect = _rect;

		Font mFont = Font( "Arial", rect.width/6 );
		textureFont = gl::TextureFont::create( mFont );
	}

	void mouseDown(MouseEvent &event){
		Utils::LOG("md on "+sb->getCoords().toString());
	}

	void update() {
		//UIView::update();
	}

	void drawTexture(){
		color(ColorA(1, 1, 1, 1));
		DataSourceRef txSrc;
		switch(sb->getSurfaceType()){
			case SURFACE_GRASS:
				txSrc = loadResource(RES_TX_GRASS);
				break;
			case SURFACE_SAND:
				txSrc = loadResource(RES_TX_SAND);
				break;
			case SURFACE_SNOW:
				txSrc = loadResource(RES_TX_SNOW);
				break;
			case SURFACE_LAVA:
				txSrc = loadResource(RES_TX_LAVA);
				break;
			default:
				txSrc = loadResource(RES_TX_GRASS);
		}

		Texture tx = loadImage(txSrc);

		gl::draw(tx, Rectf(rect.x, rect.y, rect.x+rect.width, rect.y+rect.height));
	}

	void drawBg(){
		color(ColorA(0, 0, 0, .5));		
		Rectf rect( rect.x, rect.y, rect.x+rect.width, rect.y+rect.height);
		drawSolidRect(rect);
	}

	void drawBorder(){
		color(ColorA(0, 0, 0, .8));		
		Rectf rect( rect.x, rect.y, rect.x+rect.width, rect.y+rect.height);
		lineWidth(2);
		drawStrokedRect(rect);
	}	

	void drawLabel(string str){
		color(Color(1, 1, 1));
		Vec2f strSize = textureFont->measureString(str);
		strSize.y =  textureFont->getAscent()+textureFont->getDescent();
		textureFont->drawString(
			str, 
			Vec2f(rect.x+rect.width/2-strSize.x/2, rect.y+rect.width/2+strSize.y/2)
			);
	}


	void draw() {
		UIView::draw();
		if(isVisible()){			
			if(sb != NULL){				
				drawTexture();
				if(rect.width > 50){
					drawLabel("("+to_string(sb->getCoords().x)+", "+to_string(sb->getCoords().y)+")");
				}				
			}else{
				drawBg();
				drawBorder();
				drawLabel("SurfaceBlock");						
			}
		}
	}

	void setRect(UIRect _rect){
		rect = _rect;		
	}

	shared_ptr<SurfaceBlock> getSurfaceBlock(){return sb;}
	void setSurfaceBlock(shared_ptr<SurfaceBlock> _sb){sb = _sb;}
};

#endif