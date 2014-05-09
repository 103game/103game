#include "SurfaceBlock.h"

#include <vector>
#include "BSON.h"

#include "WorldObject.h"
#include "Creatures.h"

#ifdef CLIENT_APP
	extern map<string, Texture> sharedTextures;
#endif

using namespace std;

#ifdef CLIENT_APP
void SurfaceBlock::draw(UIRect sbRect){
	Texture tx;

	switch(getSurfaceType()){
	case SURFACE_GRASS:
		tx = sharedTextures.find("grass")->second;
		break;
	case SURFACE_SAND:
		tx = sharedTextures.find("sand")->second;
		break;
	case SURFACE_SNOW:
		tx = sharedTextures.find("snow")->second;
		break;
	case SURFACE_LAVA:
		tx = sharedTextures.find("lava")->second;
		break;
	default:
		tx = sharedTextures.find("grass")->second;
	}

	// draw texture
	color(ColorA(1, 1, 1));
	gl::draw(tx, Rectf(sbRect.x, sbRect.y, sbRect.xEnd, sbRect.yEnd));

	// draw object
	if(object != NULL){
		object->draw(sbRect);
	}
}
#endif


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
		BSONObj worldObjBSON = obj.getField("object").Obj();
		string cn = worldObjBSON.getStringField("class");

		shared_ptr<WorldObject> wo;

		if(cn == "Zombie"){
			wo = shared_ptr<Zombie>(new Zombie());
		}else if(cn == "Survivor"){
			wo = shared_ptr<Survivor>(new Survivor());
		}else{
			Utils::ERR("UNEXPECTED CLASS - "+cn);
		}

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



