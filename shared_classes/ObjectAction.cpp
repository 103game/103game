#include "ObjectAction.h"
#include "WorldObject.h"

void ObjectAction::update(){
	if(finished)
		 return;

	if(!started){
		started = clock();
	}
	currentTime = (clock() - started)/(double) CLOCKS_PER_SEC;

	if(currentTime > duration){
		fin();				
		return;
	}
}

void ObjectAction::fin(){
	object->removeAction(shared_ptr<ObjectAction>(this));
	finished = true;
}