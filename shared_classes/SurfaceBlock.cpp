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

	switch(getSurfaceType()){ //getting type of Surface and decide what to do
	case SURFACE_GRASS: //if the grass
		tx = sharedTextures.find("grass")->second; // install the grass
		break;
	case SURFACE_SAND: // //if the sand
		tx = sharedTextures.find("sand")->second; // install the sand
		break;
	case SURFACE_SNOW: //if the snow
		tx = sharedTextures.find("snow")->second; //install the snow
		break;
	case SURFACE_LAVA: //if the lava
		tx = sharedTextures.find("lava")->second; // install the lava
		break;
	default: //in not special cases
		tx = sharedTextures.find("grass")->second; // install the grass
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
			shared_ptr<Zombie> zmb = shared_ptr<Zombie>(new Zombie());
			zmb->fromBSON(worldObjBSON);			
			wo = zmb;
		}else if(cn == "Survivor"){
			shared_ptr<Survivor> srv = shared_ptr<Survivor>(new Survivor());
			srv->fromBSON(worldObjBSON);
			wo = srv;
		}else{
			Utils::ERR("UNEXPECTED CLASS - "+cn);
		}
		
		setObject(wo);
	}else{
		setObject(NULL);
	}

}

/* function isEmpty
	* returns 0, if it is empty
*/

bool SurfaceBlock::isEmpty(){
	return object==NULL;
}

void SurfaceBlock::clear(){
	//object->surfaceBlock = NULL;
	setObject(NULL);
}



