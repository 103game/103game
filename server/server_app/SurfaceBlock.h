#ifndef SURFACEBLOCK_CLASS_DEF
#define SURFACEBLOCK_CLASS_DEF

#include <vector>
#include "WorldObject.h"

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
};

class SurfaceBlock: Serializable{

private:
	SURFACE surfaceType;
	COORDS coords;			

public: 

	WorldObject *object;

	SurfaceBlock(COORDS _coords) {
		setClassName("SurfaceBlock");		
		coords = _coords;
		surfaceType = SURFACE_GRASS;	
		object = NULL;
	}
	

	BSONObj toBSON() {
		BSONObjBuilder builder;
		builder.appendElements(Serializable::toBSON())
			.append("coords", coords.toBSON())
			.append("surfaceType", surfaceType)
			.append("object", WorldObject::objectToBSON(object));
		return builder.obj();
	}

	bool isEmpty(){
		return object==NULL;
	}

	void clear(){
		object->surfaceBlock = NULL;
		setObject(NULL);
	}

	WorldObject *getObject(){return object;}
	void setObject(WorldObject *_obj){object = _obj;}

	COORDS getCoords(){return coords;}
	void setCoords(COORDS _coords){coords = _coords;}
	



		
};

#endif