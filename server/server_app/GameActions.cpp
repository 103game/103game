#include "GameActions.h"

#include "Server.h"
#include "World.h"

#include "JSONMessage.h"
#include "Creatures.h"

#include "ObjectAction.h"
#include "ObjectActionMove.h"

void GameActions::updateObjectActions() {
	shared_ptr<World> world = server->world;

	for(vector<shared_ptr<WorldObject>>::iterator it = world->objects.begin(); it != world->objects.end(); it++){
		shared_ptr<WorldObject> wo = (*it);
		for(vector<shared_ptr<ObjectAction>>::iterator jt = wo->actions.begin(); jt != wo->actions.end(); jt++){
			if((*jt)->isFinished()){
				wo->actions.erase(jt);
				break;
			}				
			(*jt)->update();
		}
	}

}


void GameActions::control(JSONMessage msg) {
	string ctrl = msg.getParams().getStringField("control");

	shared_ptr<World> world = server->world;
	auto user = msg.getUser();
	shared_ptr<WorldObject> creature = world->getWorldObjectById(user->getCreatureId());	

	if(creature == NULL || !world->isOnMap(creature)){
		return;
	}

	Utils::LOG(creature->toBSON().jsonString());
	Utils::LOG(creature->getSurfaceBlock()->toBSON().jsonString());

	if(ctrl == "go_up"){
		creature->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(creature, 1, DIRECTION_UP)));
	}else if(ctrl == "go_down"){
		creature->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(creature, 1, DIRECTION_DOWN)));
	}else if(ctrl == "go_right"){
		creature->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(creature, 1,DIRECTION_RIGHT)));
	}else if(ctrl == "go_left"){
		creature->addAction(shared_ptr<ObjectActionMove>(new ObjectActionMove(creature, 1, DIRECTION_LEFT)));
	}

}

