#ifndef WORLDOBJECT_CLASS_DEF
#define WORLDOBJECT_CLASS_DEF

#include "DBObject.h"

#include "BSON.h"
#include "Utils.h"


class SurfaceBlock;
class COORDS;

class WorldObject: public DBObject {

	public:	

		shared_ptr<SurfaceBlock> surfaceBlock;
		string id;

		WorldObject() {
			setClassName("WorldObject");
			setDbCollection("server.worldobjects");
			id = Utils::randomString(16);

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


		shared_ptr<SurfaceBlock> getSurfaceBlock(){return surfaceBlock;}


};

#endif