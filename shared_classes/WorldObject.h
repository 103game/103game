#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

#include "DBObject.h"




class SurfaceBlock;
class COORDS;

class WorldObject: public DBObject {

	public:	

		shared_ptr<SurfaceBlock> surfaceBlock;

		WorldObject() {
			setClassName("WorldObject");
			setDbCollection("server.worldobjects");

			surfaceBlock = NULL;
		}

		static BSONObj objectToBSON(shared_ptr<WorldObject> obj);
			
		
		BSONObj toBSON(){
			BSONObjBuilder builder;
			builder
				.appendElements(DBObject::toBSON());
			return builder.obj();
		}

		void fromBSON(BSONObj obj) {
			DBObject::fromBSON(obj);			
		}


};

#endif