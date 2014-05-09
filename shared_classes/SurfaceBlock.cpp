#include "SurfaceBlock.h"

#include <vector>
#include "BSON.h"

#include "WorldObject.h"


using namespace std;



BSONObj SurfaceBlock::toBSON() {
	BSONObjBuilder builder;
	builder.appendElements(Serializable::toBSON())
		.append("coords", coords.toBSON())
		.append("surfaceType", surfaceType);

	if(object!=NULL){
		builder.append("object", WorldObject::objectToBSON(object));
	}

	return builder.obj();
}

void SurfaceBlock::fromBSON(BSONObj obj) {
	Serializable::fromBSON(obj);

	COORDS coords;
	coords.fromBSON(obj.getField("coords").Obj());
	setCoords(coords);

	setSurfaceType((SURFACE)obj.getIntField("surfaceType"));

	if(obj.hasField("object")){
		shared_ptr<WorldObject> wo = make_shared<WorldObject>();
		wo->fromBSON(obj.getField("object").Obj());
		setObject(wo);
	}else{
		setObject(NULL);
	}

}


bool SurfaceBlock::isEmpty(){
	return object==NULL;
}

void SurfaceBlock::clear(){
	//object->surfaceBlock = NULL;
	setObject(NULL);
}



