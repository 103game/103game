#ifndef SURFACEBLOCK_CLASS_DEF
#define SURFACEBLOCK_CLASS_DEF

#include <vector>
#include "WorldObject.h"

#include "BSON.h"

using namespace std;

typedef enum {
	SURFACE_GRASS, SURFACE_SAND, SURFACE_LAVA,
	SURFACE_SNOW, SURFACE_ROCK, SURFACE_DIRT
} SURFACE ;


class COORDS: Serializable {
public:
	long x;
	long y;

	COORDS(){
		setClassName("COORDS");
		x = 0;
		y = 0;
	}

	COORDS(long _x, long _y){
		setClassName("COORDS");
		x = _x;
		y = _y;
	}
	
	bool operator==(const COORDS& coords) {		
		return (coords.x == x) && (coords.y == y);
	}

	BSONObj toBSON(){
		BSONObjBuilder builder;

		builder.appendElements(Serializable::toBSON())
			.append("x", x)
			.append("y", y);
		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);
		x = obj.getIntField("x");
		y = obj.getIntField("y");		
	}
};

class SurfaceBlock: Serializable{

private:
	SURFACE surfaceType;
	COORDS coords;			

	shared_ptr<WorldObject> object;

public:

	SurfaceBlock(COORDS _coords, SURFACE _surfaceType) {
		SurfaceBlock::SurfaceBlock(_coords);
		setSurfaceType(_surfaceType);
	}

	SurfaceBlock(COORDS _coords) {
		setClassName("SurfaceBlock");		
		coords = _coords;
		surfaceType = SURFACE_GRASS;	
		object = NULL;
		setSurfaceType(SURFACE_GRASS);
	}
	

	BSONObj toBSON() {
		BSONObjBuilder builder;
		builder.appendElements(Serializable::toBSON())
			.append("coords", coords.toBSON())
			.append("surfaceType", surfaceType);

		if(object!=NULL){
			builder.append("object", WorldObject::objectToBSON(object));
		}
			
		return builder.obj();
	}

	void fromBSON(BSONObj obj) {
		Serializable::fromBSON(obj);

		COORDS coords;
		coords.fromBSON(obj);
		setCoords(coords);

		setSurfaceType((SURFACE)obj.getIntField("surfaceType"));
				
		if(obj.hasField("object")){
			shared_ptr<WorldObject> wo = make_shared<WorldObject>();
			wo->fromBSON(obj);
			setObject(wo);
		}else{
			setObject(NULL);
		}
		
	}


	bool isEmpty(){
		return object==NULL;
	}

	void clear(){
		object->surfaceBlock = NULL;
		setObject(NULL);
	}

	shared_ptr<WorldObject> getObject(){return object;}
	void setObject(shared_ptr<WorldObject> _obj){object = _obj;}

	COORDS getCoords(){return coords;}
	void setCoords(COORDS _coords){coords = _coords;}

	SURFACE getSurfaceType(){return surfaceType;}
	void setSurfaceType(SURFACE _surfaceType){surfaceType = _surfaceType;}
	



		
};

#endif