#include "CompilerOptions.h"

#include "WorldObject.h"
#include "SurfaceBlock.h"

#include "Creatures.h"




BSONObj WorldObject::objectToBSON(shared_ptr<WorldObject> obj) {
	if(obj->getClassName() == "Zombie"){
		return static_pointer_cast<Zombie>(obj)->toBSON();
	}else if(obj->getClassName() == "Survivor"){
		return static_pointer_cast<Survivor>(obj)->toBSON();
	}
	return obj->toBSON();
}

shared_ptr<SurfaceBlock> WorldObject::getSurfaceBlock(){
	return surfaceBlock;
}

void WorldObject::setSurfaceBlock(shared_ptr<SurfaceBlock> _sb){
	surfaceBlock = _sb;
}



BSONObj WorldObject::toBSON(){
	
	BSONObjBuilder builder;
	builder.appendElements(DBObject::toBSON());	

	return builder.obj();
}

void WorldObject::fromBSON(BSONObj obj) {
	DBObject::fromBSON(obj);		

}