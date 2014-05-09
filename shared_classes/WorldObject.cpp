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

