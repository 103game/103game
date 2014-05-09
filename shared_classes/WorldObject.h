#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

#include "DBObject.h"

#include "BSON.h"
#include "Utils.h"

#include "SurfaceBlock.h"

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
		vector<shared_ptr<ObjectAction>> actions;

		WorldObject() {
			setClassName("WorldObject");
			setDbCollection("server.worldobjects");			

			surfaceBlock = NULL;
		}

		static BSONObj objectToBSON(shared_ptr<WorldObject> obj);
			
		
		BSONObj toBSON(){
			BSONObjBuilder builder;
			builder.appendElements(DBObject::toBSON());
			return builder.obj();
		}

		void fromBSON(BSONObj obj) {
			DBObject::fromBSON(obj);			
		}

		shared_ptr<SurfaceBlock> getSurfaceBlock();

		void addAction(shared_ptr<ObjectAction> _action);
		void removeAction(shared_ptr<ObjectAction> _action);
		
		


};

#endif