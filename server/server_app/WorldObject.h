#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

#include "DBObject.h"

class SurfaceBlock;
class COORDS;

class WorldObject: public DBObject{

	public:	

		SurfaceBlock *surfaceBlock;

		WorldObject() {
			setClassName("WorldObject");
			setDbCollection("server.worldobjects");
			surfaceBlock = NULL;
		}

		static BSONObj objectToBSON(WorldObject *obj);
			

		BSONObj toBSON(){
			BSONObjBuilder builder;
			builder
				.appendElements(DBObject::toBSON());
			return builder.obj();
		}


};

#endif