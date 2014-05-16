#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

#include "DBObject.h"

#include "BSON.h"
#include "Utils.h"

#include "SurfaceBlock.h"

#ifdef CLIENT_APP
	#include <cinder/app/AppBasic.h>
	#include <cinder/gl/gl.h>
	#include "Resources.h"
	#include "UIView.h"
	using namespace ci::gl;

	extern map<string, Texture> sharedTextures;
#endif



class ObjectAction;


class COORDS;


typedef enum{
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_RIGHT,
	DIRECTION_LEFT
} DIRECTION;

class WorldObject: public DBObject {

	public:	

		shared_ptr<SurfaceBlock> surfaceBlock;		



		WorldObject() {
			setClassName("WorldObject");
			setDbCollection("server.worldobjects");			

			surfaceBlock = NULL;

		}

		shared_ptr<WorldObject> getThisFromSb(){
			if(surfaceBlock){
				return this->surfaceBlock->getObject();
			}

			return NULL;
		}

		static BSONObj objectToBSON(shared_ptr<WorldObject> obj);
			
		
		BSONObj toBSON();
		void fromBSON(BSONObj obj);

		shared_ptr<SurfaceBlock> getSurfaceBlock();
		void setSurfaceBlock(shared_ptr<SurfaceBlock> _sb);


#ifdef SERVER_APP
		virtual void update(){}		
#endif
		
		
#ifdef CLIENT_APP
		virtual void draw(UIRect rect){
			color(ColorA(1, 1, 1));		
			Utils::LOG(rect.toString());
			gl::draw(sharedTextures.find(getClassName())->second, Rectf(rect.x, rect.y, rect.xEnd, rect.yEnd));
		}
#endif


};

#endif