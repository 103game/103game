#include "WorldObject.h"
#include "SurfaceBlock.h"

#include "Creatures.h"




BSONObj WorldObject::objectToBSON(WorldObject *obj) {
	if(obj->getClassName() == "Zombie"){
		return ((Zombie*)obj)->toBSON();
	}else if(obj->getClassName() == "Survivor"){
		return ((Survivor*)obj)->toBSON();
	}
	return obj->toBSON();
}



