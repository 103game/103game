#include "CompilerOptions.h"

#include "WorldObject.h"
#include "SurfaceBlock.h"

#include "Creatures.h"

#include "ObjectAction.h"



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


void WorldObject::addAction(shared_ptr<ObjectAction> _action){	
	actions.push_back(_action);
}

void WorldObject::removeAction(shared_ptr<ObjectAction> _action){
	for(vector<shared_ptr<ObjectAction>>::iterator it = actions.begin(); it != actions.end(); it++){
		if((*it)->getId() == _action->getId()){
			actions.erase(it);
			break;
		}
	}
}
